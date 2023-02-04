#ifndef SECURITYWINDOW_H
#define SECURITYWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "menuwindow.h"
#include <QStandardItemModel>
#include <QScrollBar>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QScreen>
#include <QScroller>
#include <QTreeView>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QScreen>
#include <QScroller>
#include <QTreeView>
#include <QDir>
#include <QFile>
#include <QProcess>

#include "common.h"
#include "lineeditdialog.h"
#include "systemconfig.h"
#include "servicerecord.h"
#include "run.h"
#include "downloadmanager.h"

namespace Ui {
class SecurityWindow;
}

class SecurityWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SecurityWindow(QWidget *parent = nullptr);
    ~SecurityWindow();
    void render_table();
    void render_fs();
    void render_records();
    void wait_prompt();

private slots:
    void showTime();
    void on_pushButton_menu_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_7_clicked();

    void verify_password();
    void closeMessageBox();
    void receive_password(QString);

    void on_pushButton_15_clicked();

    void on_pushButton_19_clicked();

    void on_tableView_2_clicked(const QModelIndex &index);

    void popupTimeout();
    void on_tableView_clicked(const QModelIndex &index);
    void job_finished();
    bool load_runs();
    bool dump_runs();
    void download_message(QString);
    void upload_message(QString);

    void on_pushButton_8_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_25_toggled(bool checked);

    void on_pushButton_24_toggled(bool checked);    

    void on_pushButton_25_clicked();

private:
    Ui::SecurityWindow *ui;
    QStandardItemModel *sModel;
    QStandardItemModel *rModel;
    int scrlIdx=0;
    LineEditDialog *passwordDialog;

    int dCnt=0;
    QMessageBox *waitPrompt;
    QMessageBox *msgBox;
    QTimer *popupTimer;

    datasync_t datasync=SYNC_LIST;
    int index=0;

    int selectedIndex=0;
    maintenance_ops_t op;

    QNetworkAccessManager *nManager;
    QString usb_path;

    QList <run*> kmRun;

    QProcess *proc;

    DownloadManager *dManager;

    bool mask_close=false;

    bool u1_mnt=false;
    bool u2_mnt=false;
};

#endif // SECURITYWINDOW_H
