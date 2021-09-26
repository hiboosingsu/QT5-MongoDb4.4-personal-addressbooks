#include "kc_dialog.h"
#include "ui_kc_dialog.h"

kc_dialog::kc_dialog(QString conMsg ,QWidget *parent) : QDialog(parent), ui(new Ui::kc_dialog)
{
    ui->setupUi(this);
    ui->label->setText(conMsg);
    this->setWindowTitle("確認詢問！！");

}

kc_dialog::~kc_dialog()
{
    delete ui;
}
