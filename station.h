#ifndef STATION_H
#define STATION_H

#include <QString>
#include <QTime>
#include <QJsonArray>
#include <QJsonObject>

class station
{
public:
    station();

    QString tr_zone="";
    QString tr_no_en="";
    QString tr_nm_en="";

    QString tr_no_hi="";
    QString tr_nm_hi="";

    // train route name
    QString tr_route_en="";
    QString tr_route_hi="";

    // train route via list
    QString tr_via_en="";
    QString tr_via_hi="";

    // train number and name (text is separate, announcement is same)
    QString tr_no_rl="";
    QString tr_nm_rl="";

    QString tr_route_rl="";
    QString tr_via_rl="";

    QString w_msg1_hi                ="";
    QString w_msg2_hi                ="";
    QString w_msg3_hi                ="";

    QString w_msg1_en                ="";
    QString w_msg2_en                ="";
    QString w_msg3_en                ="";

    QString w_msg_rl                ="";
    QString w_msg1_rl                ="";
    QString w_msg2_rl                ="";
    QString w_msg3_rl                ="";

    //farewell messages
    QString f_msg1_hi                ="";   //train terminate here
    QString f_msg2_hi                ="";
    QString f_msg3_hi                ="";
    QString f_msg4_hi                ="";

    QString f_msg1_en                ="";
    QString f_msg2_en                ="";
    QString f_msg3_en                ="";
    QString f_msg4_en                ="";

    QString f_msg1_rl                ="";
    QString f_msg2_rl                ="";
    QString f_msg3_rl                ="";
    QString f_msg4_rl                ="";

    QString ss_speed_rl="";
    QString ss_curr_time_rl="";
    QString ss_next_stn_rl="";
    QString ss_next_dist_rl="";
    QString ss_eta_rl="";

    int st_no;
    QString st_code="";
    QString st_name_en="";

    QString st_name_hi="";

    QString st_name_rl="";

    QString rl="";

    double gps_lat;
    double gps_lon;

    double app_r;
    double arr_r;
    double dep_r;

    int time_arr_hh;
    int time_arr_mm;
    int time_dep_hh;
    int time_dep_mm;

    QTime time_arr;
    QTime time_dep;
    double dist_km;

   void read(const QJsonObject &json);
   void write(QJsonObject &json) const;
   void write_s(QJsonObject &json) const;
};

#endif // STATION_H
