#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include "customle.h"
#include "common.h"
#include "user.h"
namespace Ui {
class UserDialog;
}

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDialog(QWidget *parent = nullptr);
    ~UserDialog();
    void setUser(user);
    void setTitle(QString);

private slots:
    void on_buttonBox_accepted();
    void entered_un(QString str);
    void entered_pw(QString str);

    void on_buttonBox_rejected();


signals:
    void user_entered(user);

private:
    Ui::UserDialog *ui;
    customle *le_un;
    customle *le_pw;
    user usr;
};

#endif // USERDIALOG_H
