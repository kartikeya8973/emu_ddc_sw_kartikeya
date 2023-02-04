#include "execdialog.h"
#include "ui_execdialog.h"

extern routes routesInfo;
extern QList <routeData*> route_data;
extern systemconfig systemconfig;
extern QString base_dir;
ExecDialog::ExecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExecDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
    render();
    dCnt=180;//30;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),this, SLOT(timeout()));
    timer->start(1000);
}

ExecDialog::~ExecDialog()
{
    delete ui;
}

void ExecDialog::render()
{
    ui->label_7->setText("NOTE: GPS Source is Set to "+systemconfig.gps_source);
    for(int idx=0;idx<routesInfo.routeSel.count();idx++)
    {
        ui->comboBox->addItem(routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en+ " / "+routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_nm_en);
    }

    ui->comboBox_4->setCurrentText(systemconfig.gps_source);

    QDir *dir = new QDir(base_dir+"routes");
    QStringList sl = dir->entryList(QDir::Files,QDir::Name);

    for(int idx=0;idx<sl.count();idx++)
    {
        if(sl.at(idx).contains(".nmea"))
        {
            ui->comboBox_3->addItem(sl.at(idx));
        }
    }
}

void ExecDialog::timeout()
{
    ui->label->setText("System will start Auto Detect Route and Start Execution in "+QString::number(dCnt)+"s" );
    update();
    dCnt--;
    if(dCnt==0)
    {
        on_pushButton_clicked();
    }
}

void ExecDialog::on_pushButton_clicked()
{
    timer->stop();
    exec.exec_type = EXEC_AUTO;
    exec.st_idx = 0;
    exec.autoFile = ui->comboBox_3->currentText();
    emit decision(exec);
    this->close();
}

void ExecDialog::on_pushButton_4_clicked()
{
    timer->stop();
    exec.exec_type = (exec_type_t)ui->comboBox->currentIndex();

    if(ui->comboBox_2->currentText().contains("UNKNOWN"))
    {
        exec.st_idx = -1;
    }
    else
    {
        exec.st_idx = (exec_type_t)ui->comboBox_2->currentIndex();
    }

    emit decision(exec);
    this->close();
}

void ExecDialog::on_pushButton_2_clicked()
{
    timer->stop();
    exec.exec_type = EXEC_CANCEL;
    exec.st_idx = (exec_type_t)ui->comboBox_2->currentIndex();
    emit decision(exec);
    this->close();
}

void ExecDialog::on_comboBox_currentIndexChanged(int index)
{    
    ui->comboBox_2->clear();
    //parse through all stations of route

    for(int jdx=0;jdx<route_data.at(index)->st_list.count()-1;jdx++)
    {
        ui->comboBox_2->addItem(route_data.at(index)->st_list.at(jdx).st_code+" / "
                +route_data.at(index)->st_list.at(jdx).st_name_en);
    }

    ui->comboBox_2->addItem("UNKNOWN / NOT SURE");
}

void ExecDialog::on_ExecDialog_accepted()
{

}

void ExecDialog::on_comboBox_4_currentTextChanged(const QString &arg1)
{
    systemconfig.gps_source = ui->comboBox_4->currentText();
    w_log(",In AutoStart, Changed GPS Source to "+ systemconfig.gps_source);
    write_config();
}
