#include "user.h"

user::user()
{

}

void user::write(QJsonObject &json) const
{
    json["name"]=name;
    json["password"]=password;

    switch(access_level)
    {
    case ACCESS_LEVEL_1:
        json["access_level"]="ACCESS_LEVEL_1";
        break;
    case ACCESS_LEVEL_2:
        json["access_level"]="ACCESS_LEVEL_2";
        break;
    case NO_ACCESS:
        json["access_level"]="NO_ACCESS";
        break;
    }

    switch (activeStatus)
    {
    case INACTIVE:
        json["activeStatus"]="INACTIVE";
        break;
    case ACTIVE:
        json["activeStatus"]="ACTIVE";
        break;
    }
}

void user::read(const QJsonObject &json)
{
    if (json.contains("name") && json["name"].isString())
        name = json["name"].toString();

    if (json.contains("password") && json["password"].isString())
        password = json["password"].toString();

    if (json.contains("access_level") && json["access_level"].isString())
    {
        QString al = json["access_level"].toString();

        if(al=="ACCESS_LEVEL_1")
        {
            access_level=ACCESS_LEVEL_1;
        }
        else
        if(al=="ACCESS_LEVEL_2")
        {
            access_level=ACCESS_LEVEL_2;
        }
        else
        {
            access_level= NO_ACCESS;
        }
    }

    if (json.contains("activeStatus") && json["activeStatus"].isString())
    {
        QString as = json["activeStatus"].toString();

        if(as =="ACTIVE")
        {
            activeStatus  = ACTIVE;
        }
        else
        {
            activeStatus = INACTIVE;
        }
    }
}
