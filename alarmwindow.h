#ifndef ALARMWINDOW_H
#define ALARMWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include "lineeditdialog.h"
#include "common.h"
#include "logindialog.h"
#include "user.h"
namespace Ui {
class AlarmWindow;
}

class AlarmWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AlarmWindow(QWidget *parent = nullptr);
    ~AlarmWindow();
    void verify_password();
    void setNum(QList <int> *list);

signals:
    void acknowledge(int);

private slots:
    void on_pushButton_clicked();
    void timeout();
    void pass();
    void timeout_announcement();


private:
    Ui::AlarmWindow *ui;
    QTimer *timer;
    QTimer *anTimer;
    bool toggle=true;
    LineEditDialog *passwordDialog;
    int num;
};

#endif // ALARMWINDOW_H
