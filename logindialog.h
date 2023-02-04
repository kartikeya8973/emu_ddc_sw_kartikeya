#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include "mainwindow.h"
#include "menuwindow.h"
#include "customle.h"
#include "common.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    void set_title(QString title);
    void setskip(bool);
    void reload();
    QString getlevel(access_level_t);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void lock_ui();
    void pass();

private:
    QString password="";
    Ui::LoginDialog *ui;
    bool skipkb=false;
    customle *le;
    bool skip=false;
};

#endif // LOGINDIALOG_H
