#ifndef M_INDEX_H
#define M_INDEX_H

#include <QWidget>
#include <QMessageBox>
#include <mongoc/mongoc.h>
#include <QtDebug>
#include "kc_utility.h"
#include "m_append.h"

namespace Ui {
class M_index;
}

class M_index : public QWidget
{
    Q_OBJECT

public:
    explicit M_index(QWidget *parent = nullptr);
    ~M_index();
    Kc_utility ut;
    void start();
    void deleted(QString); //刪除指定紀錄

signals:
   void updated_detected(const QString&);

public slots:
   void on_pushButton_append_clicked();

private slots:
    void tableWidget_doubleClicked(const QModelIndex &index);//偵測表格雙擊

    void on_pushButton_delete_clicked();



private:
    Ui::M_index *ui;
    QMessageBox msgBox;
    int denominators;//判別資料數,計算分母總數
    int progresiveStep=0;//進度條分子累進數

};

#endif // M_INDEX_H
