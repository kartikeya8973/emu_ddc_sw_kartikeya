#ifndef RUN_H
#define RUN_H

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

class run
{
public:
    run();
    run(int, QDate,QString,QString,QString,double);

    int sno;
    QDate date;
    QString coachId="";
    QString mpuId="";
    QString route="";
    double km;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void update_km(double k);
};

#endif // RUN_H
