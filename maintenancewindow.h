#ifndef MAINTENANCEWINDOW_H
#define MAINTENANCEWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QScrollBar>
#include "common.h"
#include "run.h"
#include "logindialog.h"
#include <QDebug>

namespace Ui {
class MaintenanceWindow;
}

class MaintenanceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MaintenanceWindow(QWidget *parent = nullptr);
    ~MaintenanceWindow();

    void render_logs();
    void render_faults();
    void render_km_run();
    bool load_runs();

private slots:
    void showTime();
    void on_pushButton_menu_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

private:
    Ui::MaintenanceWindow *ui;

    int index=0;
    int scrlIdx_r=0;
    int scrlIdx_l=0;
    int scrlIdx_f=0;
    int fLines=0;
    int lLines=0;
    maintenance_t maintenance;

    QStandardItemModel *logModel;
    QStandardItemModel *faultModel;
    QStandardItemModel *rModel;

    QList <run*> kmRun;
    QStringList faultList;
};

#endif // MAINTENANCEWINDOW_H
