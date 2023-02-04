#ifndef SERVICEDATAWINDOW_H
#define SERVICEDATAWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QInputDialog>
#include <QScrollBar>
#include "route.h"
#include "routes.h"
#include "data.h"
#include "slavesyncthread.h"
#include "logindialog.h"
#include "lineeditdialog.h"
#include "systemconfig.h"
#include "menuwindow.h"
#include "mainwindow.h"

namespace Ui {
class ServiceDataWindow;
}

class ServiceDataWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServiceDataWindow(QWidget *parent = nullptr);
    ~ServiceDataWindow();

    void render_routes();
    void render_selection();

    void verify_password();
    void highlight_row(int row);

signals:
//    void update_info();

private slots:
    void showTime();
    void receive_text(QString);
    void on_pushButton_menu_clicked();

    void on_pushButton_6_clicked();

    void receive_password(QString);

    void on_tableView_routes_clicked(const QModelIndex &index);

    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_r1_clicked();

    void on_pushButton_r2_clicked();

    void on_pushButton_r3_clicked();

    void on_pushButton_r4_clicked();

    void on_pushButton_r5_clicked();

    void on_pushButton_r6_clicked();

    void on_pushButton_d1_clicked();

    void on_pushButton_d2_clicked();

    void on_pushButton_d3_clicked();

    void on_pushButton_d4_clicked();

    void on_pushButton_d5_clicked();

    void on_pushButton_d6_clicked();

    void closeMessageBox();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::ServiceDataWindow *ui;

    QStandardItemModel *rModel;
    int selectedRouteIndex=0;

    route_ops_t route_op;
    int scrlIdx=0;
    LineEditDialog *passwordDialog;

    QMessageBox *msgBox;
    QTimer *popupTimer;
    int dCnt=0;

    LineEditDialog *lineEditDialog;
};

#endif // SERVICEDATAWINDOW_H
