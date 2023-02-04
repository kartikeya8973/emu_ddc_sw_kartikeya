#ifndef DEVICETESTDIALOG_H
#define DEVICETESTDIALOG_H

#include <QDialog>
#include <QTimer>
#include "common.h"
#include "slave.h"
#include "systemconfig.h"
#include <QMessageBox>
namespace Ui {
class DeviceTestDialog;
}

class DeviceTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceTestDialog(QWidget *parent = nullptr);
    ~DeviceTestDialog();

    void drive_slaves();
    void drive_pis();
    void drive_lcd_pis();
    void render();

private slots:
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void timeout();

private:
    Ui::DeviceTestDialog *ui;
    QTimer *timer;
    QList <QString> test_strings = {

      "This is a Test",
        "এটা একটা পরীক্ষা",
        "આ એક પરીક્ષા છે",
        "यह एक परीक्षण है",
        "ಇದು ಒಂದು ಪರೀಕ್ಷೆ",
        "ഇതൊരു പരീക്ഷണമാണ്",
        "ही परीक्षा आहे",
        "यो एउटा परिक्षा हो",
        "ଏହା ଏକ ପରୀକ୍ଷା |",
        "ਇਹ ਇੱਕ ਪ੍ਰੀਖਿਆ ਹੈ",
        "هيءَ هڪ امتحان آهي",
        "இது ஒரு சோதனை",
        "ఇది ఒక పరీక్ష",
        "ہ ایک امتحان ہے"
    };
    int strIdx=0;
};

#endif // DEVICETESTDIALOG_H
