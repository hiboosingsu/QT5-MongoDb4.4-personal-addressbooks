#include "kc_utility.h"
#include "kc_dialog.h"

Kc_utility::Kc_utility(QObject *parent) : QObject(parent)
{

}
QString Kc_utility::readIniSpecified(QString iGroup,QString iVariant)
{   //讀取指定的ini資料(參數名);
    QSettings settings(iniPath,QSettings::IniFormat);
    settings.beginGroup(iGroup);
    QString iValue = settings.value(iVariant).toString();
    settings.endGroup();
    return iValue;
}
void Kc_utility::updateIniSpecified(QString iGroup,QString iVariant,QString iValue)
{   //讀取指定的ini資料(參數名);
    QSettings settings(iniPath,QSettings::IniFormat);
    settings.beginGroup(iGroup);
    settings.setValue(iVariant,iValue);
    settings.endGroup();
}
bool Kc_utility::confirm(QString &conMsg)
{   //詢問確認

    kc_dialog *kDialog = new kc_dialog(conMsg);
    int dialogCode = kDialog->exec();
    if(dialogCode == QDialog::Accepted) {

    }
    if(dialogCode == QDialog::Rejected) { // NoButton clicked
        return false;
    }

    return true;
}
