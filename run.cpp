#include "run.h"

run::run()
{

}

void run::read(const QJsonObject &json)
{
    if (json.contains("sno") && json["sno"].isDouble())
        sno = json["sno"].toInt();

    if (json.contains("date") && json["date"].isString())
        date = QDate::fromString(json["date"].toString());

    if (json.contains("coachId") && json["coachId"].isString())
        coachId = json["coachId"].toString();

    if (json.contains("mpuId") && json["mpuId"].isString())
        coachId = json["mpuId"].toString();

    if (json.contains("route") && json["route"].isString())
        route = json["route"].toString();

    if (json.contains("km") && json["km"].isString())
        km = json["km"].toString().toDouble();
}

run::run(int s, QDate dt,QString cid,QString mid,QString rt,double k)
{
    sno = s;
    coachId=cid;
    mpuId = mid;
    date=dt;
    route=rt;
    km=k;
}

void run::write(QJsonObject &json) const
{
    json["sno"]=sno;
    json["date"]=date.toString();
    json["coachId"]=coachId;
    json["mpuId"]=mpuId;
    json["route"]=route;
    json["km"]= QString::number(km,'_f',2);
}

void run::update_km(double k)
{
    km=k;
}
