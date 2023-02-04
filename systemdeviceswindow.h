#ifndef SYSTEMDEVICESWINDOW_H
#define SYSTEMDEVICESWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QProgressDialog>
//#include "device.h"
#include "slave.h"
#include "slavesyncthread.h"
#include "logindialog.h"
#include "devicetestdialog.h"
#include <QSqlTableModel>
#include <QTableView>
#include <QScrollBar>
#include <stdlib.h>
#include "gpio_utils.h"

namespace Ui {
class SystemDevicesWindow;
}

class SystemDevicesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SystemDevicesWindow(QWidget *parent = nullptr);
    ~SystemDevicesWindow();

    void render_table();
    void highlight_row(int row);


private slots:

    void closeMessageBox();
    void showTime();
    void slave_scan_progress(int pc);
    void on_pushButton_menu_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_11_clicked();

    void test_pa();

    void on_pushButton_14_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

private:
    gpio_utils gpio;
    bool test_all=false;
    QProgressDialog *pd;
    Ui::SystemDevicesWindow *ui;
    QStandardItemModel *dModel;
    int selectedIndex=0;
    int scrlIdx=0;

    QMessageBox *msgBox;
    QTimer *popupTimer;
    int dCnt;
};

#endif // SYSTEMDEVICESWINDOW_H
