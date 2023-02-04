#include "routes.h"

routes::routes()
{

}

void routes::write(QJsonObject &json) const
{
    QJsonArray rsArray;
    for(int ridx : routeSel)
    {
        QJsonObject rsObject;
        QJsonValue jValue =  ridx;
        rsArray.append(jValue);
    }
    json["routeSel"] = rsArray;

    QJsonArray routeArray;
    for(route rt : routeList)
    {
        QJsonObject routeObject;
        rt.write(routeObject);
        routeArray.append(routeObject);
    }
    json["routeList"] = routeArray;
}

void routes::read(const QJsonObject &json)
{
    if (json.contains("routeSel") && json["routeSel"].isArray())
    {
        QJsonArray rArray = json["routeSel"].toArray();
        routeSel.clear();
        routeSel.reserve(rArray.size());

        for(int rIdx=0;rIdx<rArray.size();rIdx++)
        {
            int ridx = rArray[rIdx].toInt();
            routeSel.append(ridx);
        }
    }

    if (json.contains("routeList") && json["routeList"].isArray())
    {
        QJsonArray rArray = json["routeList"].toArray();
        routeList.clear();
        routeList.reserve(rArray.size());

        for(int rIdx=0;rIdx<rArray.size();rIdx++)
        {
            QJsonObject rObject = rArray[rIdx].toObject();
            route route;
            route.read(rObject);
            routeList.append(route);
        }
    }

}
