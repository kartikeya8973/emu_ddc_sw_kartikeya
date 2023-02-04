#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "securitywindow.h"
#include "servicedatawindow.h"
#include "maintenancewindow.h"
#include "systemdeviceswindow.h"
#include "systeminfowindow.h"
#include "systemsettingswindow.h"
#include "lineeditdialog.h"

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

    bool verify_password();
    void route_ops(bool ongoing);

private slots:
    void showTime();
    void on_pushButton_menu_clicked();

    void on_pushButton_sysinfo_clicked();

    void on_pushButton_devices_clicked();

    void on_pushButton_service_data_clicked();

    void on_pushButton_settings_clicked();

    void on_pushButton_maintenance_clicked();

    void on_pushButton_security_clicked();

    void on_pushButton_menu_3_clicked();

    void on_pushButton_menu_2_clicked();

    void receive_password(QString password);

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

signals:

    void start_route();
    void stop_route();

private:
    Ui::MenuWindow *ui;
    LineEditDialog *passwordDialog;
    system_menu_ops_t system_menu_op;
};

#endif // MENUWINDOW_H
