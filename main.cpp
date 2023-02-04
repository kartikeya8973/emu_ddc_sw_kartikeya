#include "mainwindow.h"
#include "splashwindow.h"
#include "menuwindow.h"

#include "securitywindow.h"
#include "servicedatawindow.h"
#include "maintenancewindow.h"
#include "systemdeviceswindow.h"
#include "systeminfowindow.h"
#include "systemsettingswindow.h"
#include "idleeventfilter.h"
#include "common.h"
#include "user.h"
#include "route.h"
#include "routes.h"
#include "station.h"
#include "data.h"
#include "slave.h"
#include "run.h"
#include "slavesyncthread.h"
#include "slavedriverthread.h"
#include "subsystemthread.h"
#include "systemconfig.h"
#include <QApplication>
#include <QtNetwork>
#include <QQueue>
#include <QNetworkInterface>
#include "servicerecord.h"
#include "toiletmanagerthread.h"

SplashWindow *spw;
LoginDialog *ld;
MainWindow *mw;
MenuWindow *mnw;
SecurityWindow *sw;
ServiceDataWindow *sdw;
MaintenanceWindow *mtw;
SystemDevicesWindow *sdvw;
SystemInfoWindow *siw;
SystemSettingsWindow *ssw;
AlarmWindow *aw;

systemconfig systemconfig;
routes routesInfo;
QList <routeData*> route_data;
QList <slave*> papis_slaves;
QList <user> users;
user c_user;

toiletmanagerthread  *toiletThread;
slave_tasks_t   slave_task;
SlaveSyncThread *slaveSyncThread;
slaveDriverThread *slaveDriver;
subsystemthread *subsysThread;
const char* ptr_ddb_sl;
const char* ptr_ddb_dl;
const char* ptr_led_pis;
QString lcd_pis_text;

int inactive=0;
bool route_ongoing=false;

QList <QString> l_log;
QList <QString> l_fault;

#ifdef LAPTOP
QString base_dir = "/home/sahil/Projects/RTV_emu_memu/ddc_sw";
#else
QString base_dir = "/home/root/ddc_sw/";
#endif

QList <servicerecord*> sRecords;

//QList <run*> kmRun;

//QNetworkInterface ni_eth;
//QString mac_eth;
//bool has_eth=false;
//QNetworkInterface ni_wifi;
//QString mac_wifi;
//bool has_wifi=false;

//emu specific

QList <QString> keypad_arr{

    "#MS*",
    "#AS*",
    "#AD*",
    "#AH*",
    "#DG*",
    "#OC*",
    "#X1*",
    "#CVU*",
    "#CVD*",
    "#CVM*",
    "#RDP*",
    "#1*",
    "#2*",
    "#3*",
    "#4*",
    "#5*",
    "#6*",
    "#7*",
    "#8*",
    "#9*",
    "#0*",
    "#*",
    "##*",
    "#PY*",
    "#UP*",
    "#PU*",
    "#LF*",
    "#OK*",
    "#RT*",
    "#ST*",
    "#DW*",
    "#BK*",
    "#PVU*",
    "#PVD*",
    "#MU*",
    "#SP*",
    "#MA*",
    "#RT*",
    "#DEL*",
    "#PR*",
    "#PL*"
};

ddc_mode_t ddc_mode;

int main(int argc, char *argv[])
{
//    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);

    //check for config, if not found then
    if(!load_config())
    {
         populate_default_config();
    }


    //SK: 07Aug2022: default volume maximum
    //systemconfig.pa_vol = 127;
    //system(QString("amixer set 'Headphone' "+systemconfig.pa_vol).toLocal8Bit());
    system("amixer set 'Headphone' 127");

//#ifndef LAPTOP
//    apply_configs();
//#endif

    w_log(",System Booted with config: MPU_ID:"+systemconfig.mpu_id+", HW_ID:"+systemconfig.hw_id+", Coach_ID:"+systemconfig.coach_id);

    read_services();

//    //start windows
    mw = new MainWindow();
    mw->setWindowFlags(Qt::FramelessWindowHint);
//    mw->showFullScreen();
    mw->show();

//    idleEventFilter *filter=new idleEventFilter();
//    a.installEventFilter(filter);

    return a.exec();
}

int sendMPUStatus(QString status)
{
    int err=-1;

    for(int i=0;i<l_log.count();i++)
    {
        status+="; LOG: "+l_log.at(i);
    }

    for(int i=0;i<l_fault.count();i++)
    {
        status+="; FAULT: "+l_fault.at(i);
    }

//    printf("\n Sending Status to BMU\n");
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(systemconfig.bmu_ftp_ip,13000);
    err=socket->write(QString(status).toLocal8Bit());
    socket->disconnect();
    socket->close();

    l_log.clear();
    l_fault.clear();

    return err;
}

bool load_config()
{
    QFile loadFile(QString(base_dir+"config/config.json"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        w_fault("System Config file not found on disk!");
        return false;
    }
    else
    {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonObject json = loadDoc.object();

        //load config
        if (json.contains("systemconfig") && json["systemconfig"].isObject())
        {
            systemconfig.read(json["systemconfig"].toObject());
        }

        //load users
        if (json.contains("systemusers") && json["systemusers"].isArray())
        {
            QJsonArray userArray = json["systemusers"].toArray();
            users.clear();
            users.reserve(userArray.size());

            for(int uIdx=0;uIdx<userArray.size();uIdx++)
            {
                QJsonObject userObject = userArray[uIdx].toObject();
                user user;
                user.read(userObject);
                users.append(user);
            }
        }

        //load slaves
        if (json.contains("slaves") && json["slaves"].isArray())
        {
            QJsonArray slavesArray = json["slaves"].toArray();
            papis_slaves.clear();
            papis_slaves.reserve(slavesArray.size());

            for(int sIdx=0;sIdx<slavesArray.size();sIdx++)
            {
                QJsonObject slaveObject = slavesArray[sIdx].toObject();
                slave *s = new slave();
                s->read(slaveObject);
                papis_slaves.append(s);
            }
        }
        loadFile.close();
    }
    return true;
}

bool write_routes()
{
    QFile saveFile(QString(base_dir+"routes/routes.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        w_fault("Unable to save Routes file on disk!");
        return false;
    }

    QJsonObject jsonObject;
    QJsonObject routesObject;
    routesInfo.write(routesObject);
    jsonObject["routes"]=routesObject;

    saveFile.write(QJsonDocument(jsonObject).toJson());
    saveFile.close();
    return true;
}

bool write_config()
{
    QFile saveFile(QString(base_dir+"config/config.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        w_fault("Unable to Save System Config to disk!");
        return false;
    }

    QJsonObject jsonObject;
    QJsonObject configObject;
    systemconfig.write(configObject);

    QJsonArray userArray;
    for(user usr : users)
    {
        QJsonObject userObject;
        usr.write(userObject);
        userArray.append(userObject);
    }
    jsonObject["systemusers"] = userArray;

    QJsonArray slaveArray;
    for(slave *slv : papis_slaves)
    {
        QJsonObject slaveObject;
        slv->write(slaveObject);
        slaveArray.append(slaveObject);
    }
    jsonObject["slaves"] = slaveArray;

    jsonObject["systemconfig"]=configObject;
    saveFile.write(QJsonDocument(jsonObject).toJson());

    saveFile.close();
    w_log(c_user.name+",System Config Updated to disk!");
    return true;
}

bool reload_routes()
{
    routesInfo.routeList.clear();
    routesInfo.routeSel.clear();
    load_routes();
}

bool reload_routeData(int idx)
{
    route_data.clear();
    load_routeData(idx);
}

bool load_routes()
{
    //load routes
    QFile loadFile(QString(base_dir+"routes/routes.json"));
    if (!loadFile.open(QIODevice::ReadOnly)) {
        w_fault("Routes file not found on Disk!");
        return false;
    }
    else
    {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonObject json = loadDoc.object();

        if (json.contains("routes") && json["routes"].isObject())
        {
            routesInfo.read(json["routes"].toObject());
        }
        loadFile.close();
    }
    //read active route
    return true;
}

bool load_routeData(int idx)
{
    QString path = base_dir+"routes/"+
            routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en+"/route.json";

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        w_fault("Unable to Read Route data file for Route: "+routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en);
        return false;
    }
    else
    {
        QByteArray saveData = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonObject json = loadDoc.object();

        if (json.contains("routeData") && json["routeData"].isObject())
        {
            routeData *rd = new routeData();
            rd->read(json["routeData"].toObject());
            route_data.append(rd);
        }
        file.close();
    }
    return true;
}

bool read_services()
{
    QString path = base_dir+"services/srecords.json";

    if(QFile::exists(path))
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            w_fault("Unable to Read Service Records");
            return false;
        }
        else
        {
            QByteArray saveData = file.readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
            QJsonObject json = loadDoc.object();

            if (json.contains("sRecords") && json["sRecords"].isArray())
            {
                QJsonArray rArray = json["sRecords"].toArray();
                sRecords.clear();

                for(int rIdx=0;rIdx<rArray.size();rIdx++)
                {
                    QJsonObject rObject = rArray[rIdx].toObject();
                    servicerecord *sr=new servicerecord();
                    sr->read(rObject);
                    sRecords.append(sr);
                }
            }
            file.close();
        }
    }
    else
    {
        w_fault("No service records found!");
        return false;
    }
    return true;
}

bool write_services()
{
    QString path = base_dir+"services/srecords.json";
    QFile saveFile(path);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        w_fault("Unable to save service records to Disk!");
        return false;
    }

    QJsonObject jsonObject;
    QJsonArray rArray;
    for(servicerecord *sr : sRecords)
    {
        QJsonObject rObject;
        sr->write(rObject);
        rArray.append(rObject);
    }
    jsonObject["sRecords"] = rArray;
    saveFile.write(QJsonDocument(jsonObject).toJson());
    saveFile.close();
    return true;
}

void populate_default_config()
{
     systemconfig.mpu_id="";
     systemconfig.hw_id="";
     systemconfig.coach_id="";
     systemconfig.coach_no= "";

     systemconfig.gsm_status="ACTIVE";
     systemconfig.gsm_imei="";
     systemconfig.gsm_mdisdn="";
     systemconfig.gsm_apn="";

     systemconfig.eth_ip_mode="STATIC";
     systemconfig.eth_ip_addr="192.168.0.100";
     systemconfig.eth_net_mask="255.255.255.0";
     systemconfig.eth_gateway="192.168.0.1";
     systemconfig.eth_dns="192.168.0.1";

     systemconfig.wifi_mode="STATION";
     systemconfig.wifi_ap_ssid="";
     systemconfig.wifi_ap_pswd="";
     systemconfig.wifi_st_ssid="BMU_WIFI";
     systemconfig.wifi_st_pswd="123456789";
     systemconfig.wifi_ip_mode="DHCP";
     systemconfig.wifi_ip_addr="";
     systemconfig.wifi_net_mask="";
     systemconfig.wifi_gateway="";
     systemconfig.wifi_dns="";

     systemconfig.gps_source="GPS";
     systemconfig.gps_port="";

     systemconfig.dvi_active="INACTIVE";

     systemconfig.mem_int_ssd_mnt="ACTIVE";
     systemconfig.mem_ext_ssd1_mnt="ACTIVE";
     systemconfig.mem_ext_ssd2_mnt="ACTIVE";
     systemconfig.mem_sd_mnt="ACTIVE";
     systemconfig.mem_usb1_mnt="ACTIVE";
     systemconfig.mem_usb2_mnt="ACTIVE";

     systemconfig.ws_status="ACTIVE";
     systemconfig.ws_ser_url="";
     systemconfig.ws_ftp_url="";

     systemconfig.bmu_status="ACTIVE";
     systemconfig.bmu_ser_ip="";
     systemconfig.bmu_ftp_ip="";
     systemconfig.bmu_ftp_user="";
     systemconfig.bmu_ftp_pswd="";
     systemconfig.bmu_ftp_path="";

     systemconfig.pa_vol="100";
     w_log("Loaded Default Config for System");
}

void apply_config()
{
//    QString gsm_status="";
//    QString gsm_imei="";
//    QString gsm_mdisdn="";
//    QString gsm_apn="";

//    QString eth_gateway="";
//    QString eth_dns="";

//    QString wifi_mode="";
//    QString wifi_ap_ssid="";
//    QString wifi_ap_pswd="";
//    QString wifi_st_ssid="";
//    QString wifi_st_pswd="";
//    QString wifi_ip_mode="";
//    QString wifi_ip_addr="";
//    QString wifi_net_mask="";
//    QString wifi_gateway="";
//    QString wifi_dns="";

//    QString gps_source="";
//    QString gps_port="";

//    QString hdmi_active="";
//    QString dvi_active="";
//    QString vga_active="";

//    QString mem_int_ssd_mnt="";
//    QString mem_ext_ssd1_mnt="";
//    QString mem_ext_ssd2_mnt="";
//    QString mem_sd_mnt="";
//    QString mem_usb1_mnt="";
//    QString mem_usb2_mnt="";

//    QString ws_status="";
//    QString ws_ser_url="";
//    QString ws_ftp_url="";

//    QString bmu_status="";
//    QString bmu_ser_ip="";
//    QString bmu_ftp_ip="";
//    QString bmu_ftp_user="";
//    QString bmu_ftp_pswd="";
//    QString bmu_ftp_path="";

//    QString pa_vol="";
    system(QString("amixer set 'Headphone' "+systemconfig.pa_vol).toLocal8Bit());

//    int led_pis_speed=50;
//    QString led_pis_font="";
}

bool w_log(QString msg)
{
    l_log.append(msg);
    QFile file(base_dir+"logs/system.log");
    if (!file.open(QIODevice::Append)) {
        return false;
    }

    QTextStream ts(&file);
    ts<<QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss.zzz")<<","<<systemconfig.mpu_id<<","<<systemconfig.coach_id<<"/"
                        <<systemconfig.coach_no<<","<<msg<<"\n";
    file.close();

    return true;
}

bool w_fault(QString msg)
{
    l_fault.append(msg);
    QFile file(base_dir+"faults/fault.log");
    if (!file.open(QIODevice::Append)) {
        return false;
    }

    QTextStream ts(&file);
    ts<<QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss.zzz")<<","<<systemconfig.mpu_id<<","<<systemconfig.coach_id<<"/"
                        <<systemconfig.coach_no<<","<<msg<<"\n";
    file.close();

    return true;
}

#include "crc.h"
/*
 * Derive parameters from the standard-specific parameters in crc.h.
 */
#define WIDTH    (8 * sizeof(crc))
#define TOPBIT   (1 << (WIDTH - 1))

#if (REFLECT_DATA == TRUE)
#undef  REFLECT_DATA
#define REFLECT_DATA(X)			((unsigned char) reflect((X), 8))
#else
#undef  REFLECT_DATA
#define REFLECT_DATA(X)			(X)
#endif

#if (REFLECT_REMAINDER == TRUE)
#undef  REFLECT_REMAINDER
#define REFLECT_REMAINDER(X)	((crc) reflect((X), WIDTH))
#else
#undef  REFLECT_REMAINDER
#define REFLECT_REMAINDER(X)	(X)
#endif


/*********************************************************************
 *
 * Function:    reflect()
 *
 * Description: Reorder the bits of a binary sequence, by reflecting
 *				them about the middle position.
 *
 * Notes:		No checking is done that nBits <= 32.
 *
 * Returns:		The reflection of the original data.
 *
 *********************************************************************/
static unsigned long
reflect(unsigned long data, unsigned char nBits)
{
    unsigned long  reflection = 0x00000000;
    unsigned char  bit;

    /*
     * Reflect the data about the center bit.
     */
    for (bit = 0; bit < nBits; ++bit)
    {
        /*
         * If the LSB bit is set, set the reflection of it.
         */
        if (data & 0x01)
        {
            reflection |= (1 << ((nBits - 1) - bit));
        }

        data = (data >> 1);
    }

    return (reflection);

}	/* reflect() */


/*********************************************************************
 *
 * Function:    crcSlow()
 *
 * Description: Compute the CRC of a given message.
 *
 * Notes:
 *
 * Returns:		The CRC of the message.
 *
 *********************************************************************/
crc
crcSlow(unsigned char const message[], int nBytes)
{
    crc            remainder = INITIAL_REMAINDER;
    int            byte;
    unsigned char  bit;


    /*
     * Perform modulo-2 division, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
        remainder ^= (REFLECT_DATA(message[byte]) << (WIDTH - 8));

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /*
     * The final remainder is the CRC result.
     */
    return (REFLECT_REMAINDER(remainder) ^ FINAL_XOR_VALUE);

}   /* crcSlow() */


crc  crcTable[256];


/*********************************************************************
 *
 * Function:    crcInit()
 *
 * Description: Populate the partial CRC lookup table.
 *
 * Notes:		This function must be rerun any time the CRC standard
 *				is changed.  If desired, it can be run "offline" and
 *				the table results stored in an embedded system's ROM.
 *
 * Returns:		None defined.
 *
 *********************************************************************/
void
crcInit(void)
{
    crc			   remainder;
    int			   dividend;
    unsigned char  bit;


    /*
     * Compute the remainder of each possible dividend.
     */
    for (dividend = 0; dividend < 256; ++dividend)
    {
        /*
         * Start with the dividend followed by zeros.
         */
        remainder = dividend << (WIDTH - 8);

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        /*
         * Store the result into the table.
         */
        crcTable[dividend] = remainder;
    }

}   /* crcInit() */


/*********************************************************************
 *
 * Function:    crcFast()
 *
 * Description: Compute the CRC of a given message.
 *
 * Notes:		crcInit() must be called first.
 *
 * Returns:		The CRC of the message.
 *
 *********************************************************************/
crc
crcFast(unsigned char const message[], int nBytes)
{
    crc	           remainder = INITIAL_REMAINDER;
    unsigned char  data;
    int            byte;


    /*
     * Divide the message by the polynomial, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        data = REFLECT_DATA(message[byte]) ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);
    }

    /*
     * The final remainder is the CRC.
     */
    return (REFLECT_REMAINDER(remainder) ^ FINAL_XOR_VALUE);

}   /* crcFast() */

crc
crcFast(QByteArray message, int nBytes)
{
    crc	           remainder = INITIAL_REMAINDER;
    unsigned char  data;
    int            byte;


    /*
     * Divide the message by the polynomial, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        data = REFLECT_DATA(message[byte]) ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);
    }

    /*
     * The final remainder is the CRC.
     */
    return (REFLECT_REMAINDER(remainder) ^ FINAL_XOR_VALUE);

}   /* crcFast() */
