#ifndef SYSTEMSETTINGSWINDOW_H
#define SYSTEMSETTINGSWINDOW_H

#include <QMainWindow>
#include "common.h"
#include "logindialog.h"
#include "lineeditdialog.h"
#include "userdialog.h"
//#include "wifisettingsform.h"
#include "menuwindow.h"
#include "systemconfig.h"
#include "mainwindow.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QNetworkInterface>
namespace Ui {
class SystemSettingsWindow;
}

class SystemSettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SystemSettingsWindow(QWidget *parent = nullptr);
    ~SystemSettingsWindow();

    void render_user_table();
    void render_config();
    bool verify_password();
    void showInfoBox(QMessageBox::Icon icon,QString title, QString msg, int to);

private slots:
    void showTime();
    void highlight_row(int row);
    void on_pushButton_menu_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_19_toggled(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_8_clicked();

    void receive_password(QString);

    void receive_text(QString);

    void on_pushButton_2_clicked();

    void receive_user(user);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void iTimerTimeout();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_18_clicked();

signals:
    void settings_updated();

private:
    Ui::SystemSettingsWindow *ui;
    int scrlIdx=0;
    int selectedIndex=0;
    int selectedCol=0;
    int index=0;
    settings_t settings;
    QStandardItemModel *uModel;
    LineEditDialog *passwordDialog;
    LineEditDialog *lineEditDialog;
    setting_ops_t setting_op;
    UserDialog *userDialog;
    QMessageBox *infoBox;
    QTimer *iTimer;
    int dCnt;    

    QNetworkInterface eth;
    QNetworkInterface wifi;

    QMessageBox *msgBox;
    QTimer *popupTimer;
    double speedVal;
};

#endif // SYSTEMSETTINGSWINDOW_H
