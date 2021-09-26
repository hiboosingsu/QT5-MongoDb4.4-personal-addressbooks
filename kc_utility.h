#ifndef KC_UTILITY_H
#define KC_UTILITY_H

#include <QObject>
#include <QApplication>
#include <QSettings>

class Kc_utility : public QObject
{
    Q_OBJECT

public:
    explicit Kc_utility(QObject *parent = nullptr);
    QString readIniSpecified(QString,QString); //取出指定的ini資料(參數名);
    void updateIniSpecified(QString,QString,QString); //寫入指定的ini資料(區段名,參數名,參數);

    QString iniPath = QApplication::applicationDirPath() + "/system.ini";
    bool confirm(QString&);//詢問確認

signals:

private:
    int nValue;
    QSettings settings;

};

#endif // KC_UTILITY_H
