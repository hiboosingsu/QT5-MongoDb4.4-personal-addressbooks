#include "m_append.h"
#include "ui_m_append.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

M_append::M_append(QString recId ,QWidget *parent) : QWidget(parent), ui(new Ui::M_append)
{
    ui->setupUi(this);
    msgBox.setWindowTitle(tr("系統訊息顯示"));
    msgBox.setStyleSheet("QLabel{min-width:380 px; font-size: 12pt;font-family:AR PL UKai TW;color:red;} QPushButton{ width:64px; font-size: 12px; }");

    ui->lineEdit_id->setText(recId);
    if(recId.isEmpty() || recId.isNull()){

    }else{
       showed(recId);
    }

}

M_append::~M_append()
{
    delete ui;
}
void M_append::showed(QString oUid)
{
    ui->label_status->setText("修改");
    ui->label_status->setStyleSheet("QLabel{color:red;}");
    mongoc_database_t *database;
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_oid_t oid;
    bson_t *filter = NULL;
    bson_t *opts = NULL;
    char *str;
    mongoc_init ();
    client = mongoc_client_new("mongodb://localhost");
    database = mongoc_client_get_database (client, "mydb");
    collection = mongoc_client_get_collection (client, "mydb", "mycoll");
    /* build a query to execute */
     bson_oid_init_from_string (&oid, oUid.toUtf8());
    filter = BCON_NEW ("_id", BCON_OID (&oid));
    opts = BCON_NEW ("limit", BCON_INT64 (1));
    cursor = mongoc_collection_find_with_opts (collection,filter,opts,NULL);
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);
        QByteArray jsonData = str;
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonObject object = document.object();
        QVariantMap singleRec;//輸出一維資料陣列

        //全部欄位輸出
        QJsonObject::iterator i;
        for (i = object.begin(); i != object.end(); ++i) {
            if (i.value().isObject()) {

            } else if (i.value().isArray()) {

            } else {
              QString key = i.key();
              singleRec.insert(key, i.value().toString());

              if(key=="name"){
                  ui->lineEdit_name->setText(i.value().toString());
              }else if(key=="phone"){
                  ui->lineEdit_phone->setText(i.value().toString());

              }else if(key=="memo"){
                  ui->textEdit->setText(i.value().toString());
              }
            }
        }
        bson_free (str);
    }
    bson_destroy (opts);
    mongoc_collection_destroy (collection);
    mongoc_database_destroy (database);
    mongoc_client_destroy (client);
    bson_destroy (filter);

    mongoc_cleanup ();
}

void M_append::on_toolButton_store_clicked()
{
    QString i=ui->lineEdit_id->text();
    //檢查有否輸入姓名
    QString n=ui->lineEdit_name->text();
    if(n.isEmpty()){
        msgBox.setText(tr("沒有輸入姓名!!"));
        msgBox.exec();
        return ;
    }
    //檢查有否輸入電話
    QString p=ui->lineEdit_phone->text();
    if(p.isEmpty()){
        msgBox.setText(tr("沒有輸入電話!!"));
        msgBox.exec();
        return ;
    }
    //輸入備註
    QString m=ui->textEdit->toPlainText();
    QVariantMap vData;


    vData["name"]=n;
    vData["phone"]=p;
    vData["memo"]=m;
    if(i.isEmpty() || i.isNull()){
        append(vData);
    }else{
        vData["oid"]=i;
        updated(vData);
    }
    backIndex();

}
void M_append::backIndex()
{
   emit backIndex_detected();
}
void M_append::append(QVariantMap xData)
{

    mongoc_client_t *client;
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_t *doc;

    mongoc_init ();
    client = mongoc_client_new("mongodb://localhost");
    collection = mongoc_client_get_collection (client, "mydb", "mycoll");
    /* build a doc to execute */
    doc = bson_new();
    xData.remove("oid");
    
    BSON_APPEND_UTF8(doc,"name",xData["name"].toString().toLocal8Bit());
    BSON_APPEND_UTF8(doc,"phone",xData["phone"].toString().toLocal8Bit());
    BSON_APPEND_UTF8(doc,"memo",xData["memo"].toString().toLocal8Bit());

//如果欄位很多，可使用for語法拆解分裝
//    QString ts;
//   for(QVariantMap::const_iterator iter = xData.constBegin(); iter != xData.constEnd(); ++iter) {
//        ts = iter.key();
//        qDebug() << "fn:" << ts << " data:" << xData[ts].toString();
//        BSON_APPEND_UTF8(doc,ts.toLocal8Bit(),xData[ts].toString().toLocal8Bit());
//    }

    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
          qDebug() << "Insert failed:" << error.message << "\n";
    }

    bson_destroy (doc);
    mongoc_collection_destroy (collection);
    mongoc_client_destroy (client);
    mongoc_cleanup ();
}
void M_append::updated(QVariantMap xData)
{
    mongoc_database_t *database;
    mongoc_client_t *client;
    mongoc_collection_t *collection;

    bson_error_t error;
    bson_oid_t oid;
    bson_t *update = NULL;
    bson_t *filter = NULL;

    mongoc_init ();
    client = mongoc_client_new("mongodb://localhost");
    database = mongoc_client_get_database (client, "mydb");
    collection = mongoc_client_get_collection (client, "mydb", "mycoll");
    /* build a query to execute */
    bson_oid_init_from_string (&oid, xData["oid"].toString().toUtf8());
    filter = BCON_NEW ("_id", BCON_OID (&oid));
    xData.remove("oid");
    update = BCON_NEW ("$set","{",
                       "name", BCON_UTF8(xData["name"].toString().toLocal8Bit()),
                       "phone", BCON_UTF8(xData["phone"].toString().toLocal8Bit()),
                       "memo", BCON_UTF8(xData["memo"].toString().toLocal8Bit()),
            "}");
    if (!mongoc_collection_update_one (collection, filter, update, NULL, NULL, &error)) {
       qDebug() << "Updated failed:" << error.message << "\n";
    }
    bson_destroy (update);
    
//如果欄位很多，可使用for語法拆解分裝    
//    QString ts;
//    for(QVariantMap::const_iterator iter = xData.constBegin(); iter != xData.constEnd(); ++iter) {
//        ts = iter.key();
     //   qDebug() << "fn:" << ts << " data:" << xData[ts].toString();
//        update = BCON_NEW ("$set","{",BCON_UTF8(ts.toLocal8Bit()),BCON_UTF8(xData[ts].toString().toLocal8Bit()),"}");
//         if (!mongoc_collection_update_one (collection, filter, update, NULL, NULL, &error)) {
//                  qDebug() << "Updated failed:" << error.message << "\n";
//         }
//         bson_destroy (update);
//    }

    mongoc_collection_destroy (collection);
    mongoc_database_destroy (database);
    mongoc_client_destroy (client);
    bson_destroy (filter);

    mongoc_cleanup ();
}

void M_append::on_toolButton_index_clicked()
{
   emit backIndex_detected();
}

