#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include <QMainWindow>
#include "logindialog.h"

namespace Ui {
class SplashWindow;
}

class SplashWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SplashWindow(QWidget *parent = nullptr);
    ~SplashWindow();

    void setError();

private slots:
    void on_pushButton_menu_clicked();
    void update_status(QString message);
    void close_window();

private:
    Ui::SplashWindow *ui;
};

#endif // SPLASHWINDOW_H
