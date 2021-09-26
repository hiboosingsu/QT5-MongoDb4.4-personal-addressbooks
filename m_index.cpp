#include "m_index.h"
#include "ui_m_index.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>



M_index::M_index(QWidget *parent) : QWidget(parent), ui(new Ui::M_index)
{
    ui->setupUi(this);
    connect(ui->tableWidget, SIGNAL(doubleClicked(const QModelIndex &)),this, SLOT(tableWidget_doubleClicked(const QModelIndex &)));
    start();
}

M_index::~M_index()
{
    delete ui;
}
void M_index::start()
{
    mongoc_database_t *database;
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    int64_t count;
    bson_t *filter;
    bson_t *opts;
    char *str;

    mongoc_init ();
    client = mongoc_client_new("mongodb://localhost");
    database = mongoc_client_get_database (client, "mydb");
    collection = mongoc_client_get_collection (client, "mydb", "mycoll");
    /* build a query to execute */
    filter = bson_new ();
    opts = bson_new ();
    cursor = mongoc_collection_find_with_opts (collection,filter,opts,NULL);
    count = mongoc_collection_estimated_document_count(
          collection, opts, NULL, NULL, &error);
    qDebug() << "COUNT" << count;

    //設定表格欄位名稱
    QStringList horzHeaders;
    horzHeaders << "ID" << "姓名" << "PHONE" << "MEMO"  ;
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(horzHeaders);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//點擊擴及整行
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//Disable editing
    ui->tableWidget->verticalHeader()->setVisible(false);//隱藏垂直欄位0

    ui->tableWidget->horizontalHeader()->resizeSection(0,210);//設置第0列寬,ID
    ui->tableWidget->horizontalHeader()->resizeSection(1,210);//設置第1列寬,名稱
    ui->tableWidget->horizontalHeader()->resizeSection(2,210);//設置第2列寬,PHONE
    ui->tableWidget->horizontalHeader()->resizeSection(3,214);//設置第3列寬,MEMO


    int mi=0;
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);

        QByteArray jsonData = str;
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonObject object = document.object();
        ui->tableWidget->insertRow(mi);
        //全部欄位輸出
        QJsonObject::iterator i;
        for (i = object.begin(); i != object.end(); ++i) {
            if (i.value().isObject()) {
                QTableWidgetItem *checkBox = new QTableWidgetItem();
                checkBox->setText(i.value().toObject()["$oid"].toString());
                checkBox->setCheckState(Qt::Unchecked);
                checkBox->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(mi, 0, checkBox);
            //    qDebug() << mi  << "=>" << i.value().toObject()["$oid"].toString();
            } else {
                QString key = i.key();
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setText(i.value().toString());
                if(key=="name"){
                    ui->tableWidget->setItem(mi, 1, item);
                }else if(key=="phone"){
                    //item->setTextAlignment(Qt::AlignCenter); //設定文字置中
                    ui->tableWidget->setItem(mi, 2, item);
                }else if(key=="memo"){

                    ui->tableWidget->setItem(mi, 3, item);
                }
              //  qDebug() << mi <<  key << "=>" << i.value().toString();
            }

        }
        mi++;
        bson_free (str);

    }
    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (collection);
    mongoc_database_destroy (database);
    mongoc_client_destroy (client);
    bson_destroy (filter);

    mongoc_cleanup ();
}
//偵測表格雙擊
void M_index::tableWidget_doubleClicked(const QModelIndex &index)
{
    if (index.isValid() ) {
        int row = index.row();
        //取得row id value
        QString rowId = index.sibling(row, 0).data().toString();

        emit updated_detected(rowId);


    }
}

void M_index::deleted(QString oUid)
{
    mongoc_database_t *database;
    mongoc_client_t *client;
    mongoc_collection_t *collection;

    bson_error_t error;
    bson_oid_t oid;
    bson_t *filter;

    mongoc_init ();
    client = mongoc_client_new("mongodb://localhost");
    database = mongoc_client_get_database (client, "mydb");
    collection = mongoc_client_get_collection (client, "mydb", "mycoll");
    /* build a query to execute */
    filter = bson_new ();
    bson_oid_init_from_string (&oid, oUid.toUtf8());
    BSON_APPEND_OID (filter, "_id", &oid);
   // BSON_APPEND_UTF8 (filter, "hello", "world");
    if (!mongoc_collection_delete_one (collection, filter, NULL, NULL, &error)) {
          qDebug() << "Delete failed:" << error.message << "\n";
    }


    mongoc_collection_destroy (collection);
    mongoc_database_destroy (database);
    mongoc_client_destroy (client);
    bson_destroy (filter);

    mongoc_cleanup ();
}

void M_index::on_pushButton_delete_clicked()
{
    QString conMsg="確定刪除所勾選的資料？！";

    bool con = ut.confirm(conMsg);
    if(con){
        //計算分母總數
        denominators = 0;

        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            if(ui->tableWidget ->item(i, 0)->checkState() == Qt::Checked) //選中
            {
                denominators ++;
            }
        }
        if(denominators == 0 )
        {
            QMessageBox::information(this, "Info", "無資料被選擇！");
            return;
        }

        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            if(ui->tableWidget ->item(i, 0)->checkState() == Qt::Checked) //選中
            {
                QString vId=ui->tableWidget->item(i,0)->text();

                QVariantMap vMatch;
                vMatch["oid"]=vId;
                deleted (vId);
                //   qDebug() << "Delete row:" << vId << "\n";
            }
        }
        //清除表內容
        int twlb = ui->tableWidget->rowCount();
        while (twlb > -1){
            ui->tableWidget->removeRow(twlb);
            twlb --;
        }
        start();
    }
}

//新增資料
void M_index::on_pushButton_append_clicked()
{
    emit updated_detected(nullptr);

}
