#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QJsonDocument>
#include "data.h"

//#define LAPTOP
#define EMBEDDED

#define SKIP_RS485
//#define LTE_EC25
//#define LTE_C100QM

typedef enum access_level_e{

    ACCESS_LEVEL_1,
    ACCESS_LEVEL_2,
    NO_ACCESS

}access_level_t;

typedef enum settings_e{

    SETTING_P1,
    SETTING_P2,
    SETTING_P3,

}settings_t;

typedef enum maintenance_e{

    MAINTENANCE_RUN,
    MAINTENANCE_FAULTS,
    MAINTENANCE_LOGS

}maintenance_t;

typedef enum datasync_e{

    SYNC_LIST,
    SYNC_STATUS

}datasync_t;

typedef enum device_type_e{

    DEV_LCD_PIS_18,
    DEV_LCD_PIS_10,
    DEV_DDB_SL,
    DEV_DDB_DL,
    DEV_LED_PIS,
    DEV_TMS,
    DEV_PA

}device_type_t;

typedef enum activeStatus_e{

    INACTIVE,
    ACTIVE

}activeStatus_t;

typedef enum installStatus_e{

    NOT_INSTALLED,
    INSTALLED

}installStatus_t;

typedef enum slave_tasks_e{

    SLAVE_NO_TASK,
    SLAVE_INIT,
    SLAVE_SCAN_IP,
    SLAVE_SCAN_ALL,

}slave_tasks_t;

typedef enum s_driver_tasks_e{

    SD_IDLE,
    SD_SILENT_SCAN,
    SD_NEG,
    SD_NEG_ALL,
    SD_DRIVE_DDB_SL,
    SD_DRIVE_DDB_DL,
    SD_DRIVE_LED_PIS,
    SD_EMERGENCY


}s_driver_tasks_t;

typedef enum ss_tasks_e{

    SS_BOOT,
    SS_LTE,
    SS_IDLE,

}ss_tasks_t;

typedef enum announcement_idx_e{

    ANN_HINDI,
    ANN_ENG,
    ANN_RL,
    ANN_DONE

}announcement_idx_t;

typedef enum ddb_cycle_e{

    DDB_HI_ROUTE_1  =0,
    DDB_HI_TR_NM    =1,
    DDB_HI_ROUTE_2  =2,
    DDB_HI_VIA      =3,
    DDB_EN_ROUTE_1  =4,
    DDB_EN_TR_NM    =5,
    DDB_EN_ROUTE_2  =6,
    DDB_EN_VIA      =7,
    DDB_RL_ROUTE_1  =8,
    DDB_RL_TR_NM    =9,
    DDB_RL_ROUTE_2  =10,
    DDB_RL_VIA      =11,
    DDB_ERROR       =12

}ddb_cycle_t;

typedef enum pis_cycle_e{

    PIS_WELCOME,
    PIS_ROUTE,
    PIS_SPEED,
    PIS_NEXT,
    PIS_NEXT_DIST,
    PIS_ETA,
    PIS_LATE,
    PIS_SLOGAN,
    PIS_DONE

}pis_cycle_t;

typedef enum papis_sequence_e{

    PAPIS_GEN_WELCOME,
    PAPIS_DEPARTURE,
    PAPIS_RUNNING,
    PAPIS_APPROACH,
    PAPIS_ARRIVAL,
    PAPIS_WAIT_ARRIVAL,
    PAPIS_DESTINATION,
    PAPIS_NEXT,
    PAPIS_NEXT_WAIT,

}papis_sequence_t;

typedef enum setting_ops_e{

    SETTING_USER_MODIFY,
    SETTING_USER_DEL,
    SETTING_USER_CREATE,
    SETTING_GPS_SRC,
    SETTING_COACH_ID,
    SETTING_COACH_NO,
    SETTING_LED_DISPLAY,
    SETTING_DISPLAY,
    SETTING_VOLUME,
    SETTING_WIFI,

}setting_ops_t;

typedef enum route_ops_e{

    ROUTE_DEL,
    ROUTE_SET_1,
    ROUTE_SET_2,
    ROUTE_SET_3,
    ROUTE_SET_4,
    ROUTE_SET_5,
    ROUTE_SET_6,

}route_ops_t;

typedef enum maintenance_ops_e{

    UPDATE_MPU_USB,
    UPDATE_MPU_BMU,
    IMPORT_ROUTES_USB,
    IMPORT_ROUTES_BMU,
    IMPORT_SETTINGS_USB,
    IMPORT_SETTINGS_BMU,
    EXPORT_LOGS_USB,
    EXPORT_FAULTS_USB,
    EXPORT_RUNNING_USB,
    EXPORT_LOGS_BMU,
    EXPORT_FAULTS_BMU,
    EXPORT_RUNNING_BMU

}maintenance_ops_t;

typedef enum sensor_op_e{

    OFF,
    ON

}sensor_op_t;

typedef enum water_level_e{

    WATER_LOW,
    WATER_MED,
    WATER_HI

}water_level_t;

typedef enum system_menu_ops_e{

    SYSTEM_POWEROFF,
    SYSTEM_RESTART,

    START_ROUTE,
    PAUSE_ROUTE,
    STOP_ROUTE,


}system_menu_ops_t;

typedef enum day_night_mode_e{

    MODE_DAY,
    MODE_NIGHT

}day_night_mode_t;

typedef enum route_type_e{

    ROUTE_1,
    ROUTE_2,
    ROUTE_3,
    ROUTE_4,
    ROUTE_5,
    ROUTE_6,
    WAIT,
    UNKNOWN,
    NOT_SURE

}route_type_t;

typedef enum exec_type_e{

    EXEC_R1,
    EXEC_R2,
    EXEC_R3,
    EXEC_R4,
    EXEC_R5,
    EXEC_R6,
    EXEC_AUTO,
    EXEC_CANCEL,
    EXEC_NOT_SURE

}exec_type_t;

typedef enum exec_pos{

    POS_RUNNING_PREV,
    POS_APPROACH,
    POS_ARRIVAL,
    POS_DEPARTURE,
    POS_RUNNING_PAST

}exec_pos_t;

typedef struct exec_s{

    exec_type_t exec_type;
    int st_idx=0;
    QString autoFile;
    exec_pos_t exec_pos;

}exec_t;

typedef enum languages_e{

        L_ENGLISH,
        L_BANGLA,
        L_GUJARATI,
        L_HINDI,
        L_KANNADA,
        L_MALAYALAM,
        L_MARATHI,
        L_NEPALI,
        L_ODIA,
        L_PUNJABI,
        L_SINDHI,
        L_TAMIL,
        L_TELUGU,
        L_URDU

}languages_t;

typedef enum dispType_e{

    DISP_OT,
    DISP_REP,

}dispType_t;

typedef enum scrlDir_e{

    SCRL_LEFT,  //align left
    SCRL_RIGHT, //align right

}scrlDir_t;

//emu-memu specific
typedef enum announcementType_e{

    ANN_LOCAL,
    ANN_NETWORK

}announcementType_t;

//keypad key type
typedef enum keypad_press_e{

    KEY_MS,
    KEY_AS,
    KEY_AD,
    KEY_CALL,
    KEY_CALL_DG,
    KEY_CALL_OC,
    KEY_CALL_INT,
    KEY_CALL_VU,
    KEY_CALL_VD,
    KEY_CALL_MUTE,
    KEY_CALL_REDIAL,
    KEY_NUM_1,
    KEY_NUM_2,
    KEY_NUM_3,
    KEY_NUM_4,
    KEY_NUM_5,
    KEY_NUM_6,
    KEY_NUM_7,
    KEY_NUM_8,
    KEY_NUM_9,
    KEY_NUM_0,
    KEY_NUM_AST,
    KEY_NUM_HASH,
    KEY_PLAY,
    KEY_UP,
    KEY_PAUSE,
    KEY_LEFT,
    KEY_OK,
    KEY_RIGHT,
    KEY_STOP,
    KEY_DOWN,
    KEY_BACK,
    KEY_VU,
    KEY_VD,
    KEY_MENU,
    KEY_SPEAK,
    KEY_ALARM,
    KEY_SOS,
    KEY_DELAY,
    KEY_PR,
    KEY_PL

}keypad_press_t;



typedef enum ddc_mode_s{

    DDC_MODE_MASTER,
    DDC_MODE_SLAVE

}ddc_mode_t;

typedef struct dataPacket1_s{

    unsigned char head;
    unsigned char addr;
    unsigned char pIdx;
    unsigned char sspeed;
    int dispType;
    int scrlDir;
    int     statCols;
    unsigned char fb[512];
    int crc;
    int bContrl;
    unsigned char foot;

}dataPacket1_t;

typedef struct dataPacket2_s{

    unsigned char head;
    unsigned char addr;
    unsigned char pIdx;
    unsigned char fb[512];
    int crc;
    int gap;
    unsigned char foot;

}dataPacket2_t;

typedef struct toilet_status_s{

    unsigned char head;
    unsigned char addr;
    unsigned char clogged;
    unsigned char stationery;
    unsigned char curtain;
    unsigned char water_level;
    unsigned char occupancy;
    unsigned char sw;
    int crc;
    unsigned char foot;

}toilet_status_t;

//new toilet packet
typedef struct t_packet_s
{
    uint8_t clogged_sensor:1;
    uint8_t stationary_water:1;
    uint8_t carten_open_close:1;
    uint8_t occupancy:1;
    uint8_t water_leval_low:1;
    uint8_t water_leval_medium:1;
    uint8_t water_leval_high:1;
    uint8_t emergency_button:1;
}t_packet_t;

typedef struct toilet_packet_s
{
    uint8_t head;
    uint8_t occupancy1;
    uint8_t occupancy2;
    uint8_t emergency1;
    uint8_t emergency2;
    uint8_t foot;
}toilet_packet_t;


typedef struct sdTask_s{

        s_driver_tasks_t s_driver_task;
        const char *data;
        int size;
        dispType_t dtype;
        scrlDir_t sdir;
        day_night_mode_t mode;

}sdtask_t;

extern data papis_data;
bool load_config();
void apply_config();
bool load_routes();
bool load_routeData(int idx);

bool reload_routes();
bool reload_routeData(int idx);

bool write_config();
bool write_routes();

bool read_services();
bool write_services();

void populate_default_config();
void apply_config();

bool w_log(QString msg);
bool w_fault(QString msg);

bool load_runs();
bool dump_runs();

int sendMPUStatus(QString status);

#endif // COMMON_H
