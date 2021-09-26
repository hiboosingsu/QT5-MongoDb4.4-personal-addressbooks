#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void appendDetected(QString);
    void returnHome();

private slots:
    void on_index_triggered();

    void on_append_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
