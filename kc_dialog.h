#ifndef KC_DIALOG_H
#define KC_DIALOG_H

#include <QDialog>

namespace Ui {
class kc_dialog;
}

class kc_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit kc_dialog(QString conMsg ,QWidget *parent = nullptr);
    ~kc_dialog();

private:
    Ui::kc_dialog *ui;
};

#endif // KC_DIALOG_H
