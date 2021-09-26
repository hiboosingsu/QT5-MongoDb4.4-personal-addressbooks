#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "m_index.h"
#include "m_append.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//資料總攬
void MainWindow::on_index_triggered()
{
    M_index *mi = new M_index();
   // mi->show();
    //雙擊按鈕偵測

    setCentralWidget(mi);
    connect(mi, SIGNAL(updated_detected(QString)), this, SLOT(appendDetected(QString)));
}

//新增資訊
void MainWindow::on_append_triggered()
{
    M_append *ma = new M_append(nullptr);
   // ma->show();

    setCentralWidget(ma);
    connect(ma, SIGNAL(backIndex_detected()), this, SLOT(on_index_triggered()));
}
void MainWindow::returnHome()
{
    on_index_triggered();
}
void MainWindow::appendDetected(QString recId)
{

    M_append *ma = new M_append(recId);

    setCentralWidget(ma);
    connect(ma, SIGNAL(backIndex_detected()), this, SLOT(on_index_triggered()));
}
