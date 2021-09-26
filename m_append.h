#ifndef M_APPEND_H
#define M_APPEND_H

#include <QWidget>
#include <QMessageBox>
#include <mongoc/mongoc.h>
#include <QtDebug>

namespace Ui {
class M_append;
}

class M_append : public QWidget
{
    Q_OBJECT

public:
    explicit M_append(QString recId ,QWidget *parent = nullptr);
    ~M_append();
    void start();
    void showed(QString);//顯示紀錄
    void append(QVariantMap);//新增紀錄
    void updated(QVariantMap);//更新紀錄
    void deleted(); //刪除紀錄
    void backIndex(); //返回主頁

signals:
   void backIndex_detected();

private slots:
    void on_toolButton_store_clicked();

    void on_toolButton_index_clicked();

private:
    Ui::M_append *ui;
    QMessageBox msgBox;
};

#endif // M_APPEND_H
