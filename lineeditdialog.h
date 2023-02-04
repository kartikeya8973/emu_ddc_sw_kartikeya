#ifndef LINEEDITDIALOG_H
#define LINEEDITDIALOG_H

#include <QDialog>
#include "customle.h"
namespace Ui {
class LineEditDialog;
}

class LineEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LineEditDialog(QWidget *parent = nullptr);
    ~LineEditDialog();
    void setLabel(QString);
    void setLabel2(QString);
    void setEchoMode(QLineEdit::EchoMode echomode);

private:
    Ui::LineEditDialog *ui;
    customle *le;
    QString str;

signals:
    void text_entered(QString);
private slots:
    void on_buttonBox_accepted();
    void entered(QString);
};

#endif // LINEEDITDIALOG_H
