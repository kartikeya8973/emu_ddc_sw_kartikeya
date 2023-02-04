#ifndef SYSTEMINFOWINDOW_H
#define SYSTEMINFOWINDOW_H

#include <QMainWindow>
#include "logindialog.h"

namespace Ui {
class SystemInfoWindow;
}

class SystemInfoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SystemInfoWindow(QWidget *parent = nullptr);
    ~SystemInfoWindow();

private slots:
    void showTime();
    void on_pushButton_menu_clicked();

private:
    Ui::SystemInfoWindow *ui;
};

#endif // SYSTEMINFOWINDOW_H
