#include "slave.h"

slave::slave()
{

}

slave::slave(int _sno,int _rs485_addr,QString _ip_addr,device_type_t _device_type, activeStatus_t _activeStatus, installStatus_t _installStatus, QTcpSocket *_socket, QString _hw_id)
{
    sno=_sno;
    rs485_addr=_rs485_addr;
    ip_addr=_ip_addr;
    device_type=_device_type;
    activeStatus=_activeStatus;
    installStatus = _installStatus;
//    socket = _socket;
    hw_id = _hw_id;

    switch(device_type)
    {
    case DEV_LCD_PIS_18:
        device_type_desc ="LCD PIS 18.5'";
        break;
    case DEV_LCD_PIS_10:
        device_type_desc ="LCD PIS 10.1'";
        break;
    case DEV_DDB_SL:
        device_type_desc ="DDB SL";
        break;
    case DEV_DDB_DL:
        device_type_desc ="DDB DL";
        break;
    case DEV_LED_PIS:
        device_type_desc ="LED PIS";
        break;
    case DEV_TMS:
        device_type_desc ="EPB, TMU";
        break;
    }
}

void slave::write(QJsonObject &json) const
{
    json["sno"]=sno;
    json["rs485_addr"]=rs485_addr;
    json["ip_addr"]=ip_addr;

    switch(device_type)
    {
    case DEV_LCD_PIS_18:
    {
        json["device_type"]="DEV_LCD_PIS_18";
    }
    break;
    case DEV_LCD_PIS_10:
    {
        json["device_type"]="DEV_LCD_PIS_10";
    }
    break;
    case DEV_DDB_SL:
    {
        json["device_type"]="DEV_DDB_SL";
    }
    break;
    case DEV_DDB_DL:
    {
        json["device_type"]="DEV_DDB_DL";
    }
    break;
    case DEV_LED_PIS:
    {
        json["device_type"]="DEV_LED_PIS";
    }
    break;
    case DEV_TMS:
    {
        json["device_type"]="DEV_TMS";
    }
    break;
    case DEV_PA:
    {
        json["device_type"]="DEV_PA";
    }
    break;
    }

    switch(activeStatus)
    {
    case INACTIVE:
        json["activeStatus"]="INACTIVE";
        break;
    case ACTIVE:
        json["activeStatus"]="ACTIVE";
        break;
    }

    switch(installStatus)
    {
    case NOT_INSTALLED:
        json["installStatus"]="NOT_INSTALLED";
        break;
    case INSTALLED:
        json["installStatus"]="INSTALLED";
        break;
    }

    json["hw_id"]=hw_id;
    json["conn_mode"]=conn_mode;
}

void slave::read(const QJsonObject &json)
{
    if (json.contains("sno") && json["sno"].isDouble())
        sno = json["sno"].toInt();

    if (json.contains("rs485_addr") && json["rs485_addr"].isDouble())
        rs485_addr = json["rs485_addr"].toInt();

    if (json.contains("ip_addr") && json["ip_addr"].isString())
        ip_addr = json["ip_addr"].toString();

    if (json.contains("device_type") && json["device_type"].isString())
    {
        QString dt = json["device_type"].toString();

        if(dt=="DEV_LCD_PIS_18")
        {
            device_type=DEV_LCD_PIS_18;
        }
        else
        if(dt=="DEV_LCD_PIS_10")
        {
            device_type=DEV_LCD_PIS_10;
        }
        else
        if(dt=="DEV_DDB_SL")
        {
            device_type=DEV_DDB_SL;
        }
        else
        if(dt=="DEV_DDB_DL")
        {
            device_type=DEV_DDB_DL;
        }
        else
        if(dt=="DEV_LED_PIS")
        {
            device_type=DEV_LED_PIS;
        }
        else
        if(dt=="DEV_TMS")
        {
            device_type=DEV_TMS;
        }
        else
        if(dt=="DEV_PA")
        {
            device_type=DEV_PA;
        }
    }

    switch(device_type)
    {
    case DEV_LCD_PIS_18:
        device_type_desc ="LCD PIS 18.5'";
        break;
    case DEV_LCD_PIS_10:
        device_type_desc ="LCD PIS 10.1'";
        break;
    case DEV_DDB_SL:
        device_type_desc ="DDB SL";
        break;
    case DEV_DDB_DL:
        device_type_desc ="DDB DL";
        break;
    case DEV_LED_PIS:
        device_type_desc ="LED PIS";
        break;
    case DEV_TMS:
        device_type_desc ="EPB, TMU";
        break;
    case DEV_PA:
        device_type_desc ="PA SYSTEM";
        break;
    }

    if (json.contains("installStatus") && json["installStatus"].isString())
    {
        QString is = json["installStatus"].toString();

        if(is=="NOT_INSTALLED")
        {
            installStatus=NOT_INSTALLED;
        }
        else
        if(is=="INSTALLED")
        {
            installStatus=INSTALLED;
        }
    }

    if (json.contains("activeStatus") && json["activeStatus"].isString())
    {
        QString as = json["activeStatus"].toString();

        if(as=="INACTIVE")
        {
            activeStatus=INACTIVE;
        }
        else
        if(as=="ACTIVE")
        {
            activeStatus=ACTIVE;
        }
    }

    if (json.contains("hw_id") && json["hw_id"].isString())
    {
        hw_id = json["hw_id"].toString();
    }

    if (json.contains("conn_mode") && json["conn_mode"].isString())
    {
        conn_mode = json["conn_mode"].toString();
    }
}
