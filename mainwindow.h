#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFont>
#include <QFontDatabase>
#include <QtNetwork>
#include <QProgressDialog>
#include <QDebug>
#include <QFile>
#include <QNmeaPositionInfoSource>
#include <QtSerialPort>
#include <QMovie>
#include <QStandardItemModel>
#include "menuwindow.h"
#include "splashwindow.h"
#include "slave.h"
#include "logindialog.h"
#include "common.h"
#include "slavesyncthread.h"
#include "slavedriverthread.h"
#include "subsystemthread.h"
#include "systemconfig.h"
#include "keyboarddialog.h"
#include <math.h>
#include "alarmwindow.h"
#include "run.h"
#include "execdialog.h"
#include <QVBoxLayout>
#include "gpio_utils.h"
#include <iostream>
#include "crc.h"
#include "toilet_stat.h"
#include "audioplayerthread.h"
#include "toiletmanagerthread.h"

namespace Ui {
class MainWindow;
}

class probable{

public:
    probable(){}
    probable(int r, int s){route=r; stn = s;}

    int route;
    int stn;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void settings_updated();

private slots:

    void pow_lo();
    void pow_hi();
    void pow_ok();
    void mic_in();
    void mic_out();
    void eth_present();
    void eth_absent();
    void eth_up();
    void eth_down();
    void eth_con();
    void eth_dis();
    void slave_init_done();

    void ss_present();
    void ss_absent();
    void lte_present();
    void lte_absent();

    void slaveTimeout();
    void routeTimeout();
    void ddbCycleTimeout();
    void drive_ddb();
    void update_occ(QString);
    void pis_drive_slogan();

    void closeMessageBox();
    void switch_route();
    void showTime();
    void clearMessage();

    void slave_scan_progress_eth(int pc,QList <slave*> slaves);
    void slave_scan_progress_rs485(int pc,QList <slave*> slaves);
    void slave_scan_silent(int pc,QList <slave*> slaves);
    void slave_silent_scan(QList <slave*> slaves);

    void load_info();
    void draw_route();
    bool load_runs();
    bool dump_runs();

    void positionUpdated(QGeoPositionInfo positionInfo);
    route_type_t wakeupStart();
    route_type_t wakeupMidRoute();
    void assign_route();
    void updateTimeout(void);
    void error(QGeoPositionInfoSource::Error positioningError);

    void on_pushButton_menu_clicked();    

    void socketReadyRead();
    void socketConnected();
    void socketConnectionClosed();
    void socketClosed();
    void socketWritten();
    void socketError( QAbstractSocket::SocketError e );

    void on_pushButton_5_clicked();

    void slave_send_route();
    void slave_send_distance(double km);
    void slave_send_alert(int i);
    void slave_send_clear(int i);
    void slave_send_return();
    void slave_send_error();
    void slave_send_mode(QString mode);
    void slave_send_message();
    void slave_get_logs();

    void decision(exec_t);    

    void on_pushButton_6_clicked();

    void atTimeout();
    void atReadyRead();
    void tmsTimeout();
    void alarm_ack(int);

    void start_route();
    void stop_route();

    void on_pushButton_8_toggled(bool checked);

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();
    void message_pass(QString);
    void toilet_status(int,toilet_packet_t);

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_toggled(bool checked);

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void client_connected();
    void server_ready_read();
    void serverReadyRead();

    void on_pushButton_19_clicked();

    void pjsip_connected();
    void pjsip_ready_read();

    void on_pushButton_call_pick_clicked();

    void on_pushButton_call_reject_clicked();

    void callTimerTimeout();

    void on_pushButton_select_etb1_toggled(bool checked);

    void on_pushButton_select_etb_dg_toggled(bool checked);

    void on_pushButton_select_etb_occ_toggled(bool checked);

    void on_pushButton_pa_manual_toggled(bool checked);

    void on_pushButton_next_clicked();

    void on_pushButton_back_clicked();

    void on_verticalSlider_cabmic_valueChanged(int value);

    void on_verticalSlider_cabspk_valueChanged(int value);

    void on_verticalSlider_paspk_valueChanged(int value);

    void on_pushButton_cabmic_set_clicked();

    void on_pushButton_cabspk_set_clicked();

    void on_pushButton_paspk_set_clicked();

    void load_prerec_files();

    void on_pushButton_pa_auto_clicked();

    void on_pushButton_pa_manual_clicked();

    void on_pushButton_pa_auto_toggled(bool checked);

    void on_pushButton_speak_toggled(bool checked);

    void on_pushButton_mute_clicked();

    void on_pushButton_mute_toggled(bool checked);

    void on_pushButton_select_etb2_toggled(bool checked);

    void on_pushButton_spk_toggled(bool checked);

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_stoprec_clicked();

    void on_pushButton_cast_clicked();

    void on_pushButton_playrec_clicked();

    void highlight_row(int row);

    void on_listView_rec_clicked(const QModelIndex &index);

    void on_pushButton_cabmic_p_clicked();

    void on_pushButton_cabmic_m_clicked();

    void on_pushButton_cabspk_p_clicked();

    void on_pushButton_cabspk_m_clicked();

    void on_pushButton_paspk_p_clicked();

    void on_pushButton_paspk_m_clicked();

    bool eventFilter(QObject* object, QEvent* event);

    void keypressed_slot(keypad_press_t);

    void on_pushButton_select_etb4_toggled(bool checked);

public:


//    bool load_runs();
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTcpServer *tcp = nullptr;
    QTcpSocket *socket = nullptr;

    double Calc_distance_between_locations(double longA, double latA, double longB, double latB);
    void gain_consciousness();
    void route_decision();
    void error_mode();
    void welcome_mode_eng();
    void welcome_mode_hin();
    void welcome_mode_rl(int idx);
    void drive_slaves();
    void drive_pis();
    void drive_pis_rs485(dispType_t dtype, scrlDir_t sdir, int scols);
    void drive_ddb_sl_rs485(dispType_t dtype, scrlDir_t sdir, int scols);
    void drive_ddb_dl_rs485(dispType_t dtype, scrlDir_t sdir, int scols);

    void render_dashboard();
    void render_slave_status();
    QFont* getRLFont(QString rl);

    void drive_pis485();
    void drive_ddb485();

//    void img();

signals:
//    void splash_status_update(QString);
    void audio_add(QString);
    void audio_add(QString,announcementType_t);
    void audio_play();
    void splash_close();
    void announce(QString);
    void ddb_drive();
    void pis_slogan_drive();
    void rs485_drive_matrix(s_driver_tasks_t s_d_task,const char* d, int s, dispType_t dt, scrlDir_t sd,day_night_mode_t mode);
    void rs485_scan_all(QList <slave*>);
    void rs485_scan(int addr);
    void rs485_emergency_ack(int addr);
    void eth_scan_all(QList <slave*>);

    void slave_route_send();
    void slave_distance_send(double km);
    void slave_alert_send(int i);
    void slave_clear_send(int i);
    void slave_return_send();
    void slave_error_send();
    void slave_mode_send(QString mode);
    void slave_message_send();


    void keypressed(keypad_press_t);

private:
    Ui::MainWindow *ui;

    QString lang="";
    QString sw_ver;

    QProgressDialog *pd;
    bool is_pd=false;

    bool gpsFix=false;
    bool wake;
    bool route_ok=true;
    QFile *gpsFile;
    QNmeaPositionInfoSource *nmeaSource;

    QTimer *msgTimer;
    QTimer *ddbCycler;
    QTimer *popupTimer;
    QTimer *dcTimer;
    QMessageBox *msgBox;
    QMessageBox *eMsgBox;
    int dCnt;

    double orig_lat, orig_long;
    double curr_lat, curr_long;
    double prev_lat, prev_long;
    QTime prev_tim, cur_tim;
    int gps_int=20;
    QDateTime gpsDateTime;
    int gps_ctr=0;
    int gps_pidx=0;
    int gps_fp=true;
    double k_lat, k_lon;
    QList <run*> kmRun;
    double kmR=0;
    double kmT=0;
    double kmD=0;
    double kmRt=0;
    double nDist_p=0;
    int nDist_Ctr=0;

    int prev_stn_idx=0;
    int current_stn_idx=99999;
    int nex_stn_idx=1;
    bool audio_busy;

    double pCtr=0;

    int appCtr=0;
    announcement_idx_t  announcement_idx = ANN_HINDI;
    ddb_cycle_t ddb_cycle = DDB_HI_ROUTE_1;
    pis_cycle_t pis_cycle = PIS_WELCOME;
    routeData *aRouteData;
    int aStnIdx;
    bool slogan_play = false;
    int slogan_idx=0;

    double avg_speed=0;
    double c_speed=0;
    bool train_started=false;
    bool wel_played=false;
    volatile bool  announcement_ongoing=false;
    bool route_ongoing=false;
    bool error_displayed=false;

    int w_msg_idx=0;
    int f_msg_idx=0;
    bool skipSync=false;

    int ai;
    int pi;
    int kilometer=0;

    double du, dd;
    int iu, id;
    route_type_t route_type;
    int uctr=0;
    int aRIdx=0;
    bool lr=false,rt=true;
    QTime lateBy;

    papis_sequence_t papis_sequence=PAPIS_GEN_WELCOME;

    QTimer *slaveTimer;
    QTimer *routeTimer;
    volatile day_night_mode_t day_night_mode;
    bool mask_dn=false;
    bool mask_wc=false;

    QSerialPort* serial;
    QSerialPort* atSerialPort;
    QSerialPort* rs485Port;
    char tmsSendData[3];
    QTimer* tmsTimer;
    QTimer *rsTimer;
    QTimer *atTimer;

    gpio_utils gpio;
    QTimer *onTimer;
    QTimer *offTimer;

    bool emergency=false;
    bool tms_alarm=false;
    QList <int>  *alarmSrcList;

    int atCnt=0;

    QList<probable*> pList;

    bool pnext_popup=false;

    int unk=0;

    ushort ba_ddb_sl[512];
    ushort ba_ddb_dl[1024];
    ushort ba_led_pis[512];

    //font objects
    QFont *font_hi;
    QFont *font_en;
    QFont *font_ban;
    QFont *font_guj;
    QFont *font_kan;
    QFont *font_mal;
    QFont *font_nep;
    QFont *font_odia;
    QFont *font_pun;
    QFont *font_tam;
    QFont *font_tel;
    QFont *font_urdu;

    QTcpServer *tcpServer = nullptr;
    QTcpSocket *clientConnection = nullptr;

    bool toiletThreadStarted=false;

    QTcpSocket *sock;
    int alarmSource;

    QString tstat1a="0";
    QString tstat1b="0";
    QString tstat2a="0";
    QString tstat2b="0";

    //emu-memu specific
    QTcpServer *pjsipListener = nullptr;
    QTcpSocket *pjsipLocalConnection = nullptr;
    QTcpSocket *pjsipSocket;
    QTcpSocket *ccVolSocket;
    QTcpSocket *etbCallSocket;

    QTimer* callTimer;
    int callTimeCtr;
    int callAutoCutTime = 120;
    bool callAutoCut=true;

    QString buddyNum="";

    int cab_mic_vol=100;
    int cab_spk_vol=100;
    int pa_spk_vol= 100;
    bool pa_vol_auto=false;

    QStringList audio_files;
    QStandardItemModel *aModel;
    int selectedIndex=0;
    int scrlIdx=0;

    //keypad specific
    QString keystr="";

};

#endif // MAINWINDOW_H
