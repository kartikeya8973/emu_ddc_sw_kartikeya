#include "servicerecord.h"

servicerecord::servicerecord(QObject *parent) : QObject(parent)
{

}

servicerecord::servicerecord(int s_no,maintenance_ops_t o_p,QString src,
                                 QString usr,QString stat)
{
    sno=s_no;
    op=o_p;
    issue_dt=QDateTime::currentDateTime();
    issue_src=src;
    user=usr;
    status=stat;
}

void servicerecord::read(const QJsonObject &json)
{
    if (json.contains("sno") && json["sno"].isDouble())
        sno  = json["sno"].toInt();

    if (json.contains("op") && json["op"].isString())
    {
        if(json["op"]=="UPDATE_MPU_USB")
        {
            op=UPDATE_MPU_USB;
        }
        else
        if(json["op"]=="UPDATE_MPU_BMU")
        {
            op=UPDATE_MPU_BMU;
        }
        else
        if(json["op"]=="IMPORT_ROUTES_USB")
        {
            op=IMPORT_ROUTES_USB;
        }
        else
        if(json["op"]=="IMPORT_ROUTES_BMU")
        {
            op=IMPORT_ROUTES_BMU;
        }
        else
        if(json["op"]=="IMPORT_SETTINGS_USB")
        {
            op=IMPORT_SETTINGS_USB;
        }
        else
        if(json["op"]=="IMPORT_SETTINGS_BMU")
        {
            op=IMPORT_SETTINGS_BMU;
        }
        else
        if(json["op"]=="EXPORT_LOGS_USB")
        {
            op=EXPORT_LOGS_USB;
        }
        else
        if(json["op"]=="EXPORT_FAULTS_USB")
        {
            op=EXPORT_FAULTS_USB;
        }
        else
        if(json["op"]=="EXPORT_RUNNING_USB")
        {
            op=EXPORT_RUNNING_USB;
        }
        else
        if(json["op"]=="EXPORT_LOGS_BMU")
        {
            op=EXPORT_LOGS_BMU;
        }
        else
        if(json["op"]=="EXPORT_FAULTS_BMU")
        {
            op=EXPORT_FAULTS_BMU;
        }
        else
        if(json["op"]=="EXPORT_RUNNING_BMU")
        {
            op=EXPORT_RUNNING_BMU;
        }
    }

    if (json.contains("issue_dt") && json["issue_dt"].isDouble())
        issue_dt  =  QDateTime::fromSecsSinceEpoch(json["issue_dt"].toInt());

    if (json.contains("issue_src") && json["issue_src"].isString())
        issue_src  = json["issue_src"].toString();

    if (json.contains("user") && json["user"].isString())
        user  = json["user"].toString();

    if (json.contains("status") && json["status"].isString())
        status  = json["status"].toString();

}

void servicerecord::write(QJsonObject &json) const
{
    json["sno"]=sno;
    switch(op)
    {
    case UPDATE_MPU_USB:
        json["op"]="UPDATE_MPU_USB";
        break;
    case UPDATE_MPU_BMU:
        json["op"]="UPDATE_MPU_USB";
        break;
    case IMPORT_ROUTES_USB:
        json["op"]="IMPORT_ROUTES_USB";
        break;
    case IMPORT_ROUTES_BMU:
        json["op"]="IMPORT_ROUTES_BMU";
        break;
    case IMPORT_SETTINGS_USB:
        json["op"]="IMPORT_SETTINGS_USB";
        break;
    case IMPORT_SETTINGS_BMU:
        json["op"]="IMPORT_SETTINGS_BMU";
        break;
    case EXPORT_LOGS_USB:
        json["op"]="EXPORT_LOGS_USB";
        break;
    case EXPORT_FAULTS_USB:
        json["op"]="EXPORT_FAULTS_USB";
        break;
    case EXPORT_RUNNING_USB:
        json["op"]="EXPORT_RUNNING_USB";
        break;
    case EXPORT_LOGS_BMU:
        json["op"]="EXPORT_LOGS_BMU";
        break;
    case EXPORT_FAULTS_BMU:
        json["op"]="EXPORT_FAULTS_BMU";
        break;
    case EXPORT_RUNNING_BMU:
        json["op"]="EXPORT_RUNNING_BMU";
        break;
    }
    json["issue_dt"]=issue_dt.toSecsSinceEpoch();
    json["user"]=user;
    json["status"]=status;
}


void servicerecord::update_status(QString stat)
{
    status=stat;
}
