#ifndef EXECDIALOG_H
#define EXECDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QDir>
#include <QDirIterator>
#include "route.h"
#include "routes.h"
#include "common.h"
#include "systemconfig.h"


namespace Ui {
class ExecDialog;
}

class ExecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExecDialog(QWidget *parent = nullptr);
    ~ExecDialog();
    void render();

private slots:
    void timeout();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_ExecDialog_accepted();

    void on_comboBox_4_currentTextChanged(const QString &arg1);

signals:
    void decision(exec_t);

private:
    Ui::ExecDialog *ui;
    int dCnt=0;
    QTimer *timer;
    exec_t exec;
};

#endif // EXECDIALOG_H
