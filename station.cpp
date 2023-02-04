#include "station.h"

station::station()
{

}

void station::write_s(QJsonObject &json) const
{
    json["st_no"]=st_no;
    json["st_code"]=st_code;
    json["st_name_en"]=st_name_en;
    json["st_name_hi"]=st_name_hi;
    json["st_name_rl"]=st_name_rl;
    json["rl"]=rl;
    json["dist_km"]=dist_km;
}

void station::write(QJsonObject &json) const
{
    json["tr_zone"]=tr_zone;
    json["tr_no_en"]=tr_no_en;
    json["tr_nm_en"]=tr_nm_en;
    json["tr_no_hi"]=tr_no_hi;
    json["tr_nm_hi"]=tr_nm_hi;
    json["tr_route_en"]=tr_route_en;
    json["tr_route_hi"]=tr_route_hi;
    json["tr_via_en"]=tr_via_en;
    json["tr_via_hi"]=tr_via_hi;
    json["tr_no_rl"]=tr_no_rl;
    json["tr_nm_rl"]=tr_nm_rl;
    json["tr_route_rl"]=tr_route_rl;
    json["tr_via_rl"]=tr_via_rl;
    json["w_msg1_hi"]=w_msg1_hi;
    json["w_msg2_hi"]=w_msg2_hi;
    json["w_msg3_hi"]=w_msg3_hi;
    json["w_msg1_en"]=w_msg1_en;
    json["w_msg2_en"]=w_msg2_en;
    json["w_msg3_en"]=w_msg3_en;
    json["w_msg_rl"]=w_msg_rl;
    json["w_msg1_rl"]=w_msg1_rl;
    json["w_msg2_rl"]=w_msg2_rl;
    json["w_msg3_rl"]=w_msg3_rl;
    json["f_msg1_hi"]=f_msg1_hi;
    json["f_msg2_hi"]=f_msg2_hi;
    json["f_msg3_hi"]=f_msg3_hi;
    json["f_msg4_hi"]=f_msg4_hi;
    json["f_msg1_en"]=f_msg1_en;
    json["f_msg2_en"]=f_msg2_en;
    json["f_msg3_en"]=f_msg3_en;
    json["f_msg4_en"]=f_msg4_en;
    json["f_msg1_rl"]=f_msg1_rl;
    json["f_msg2_rl"]=f_msg2_rl;
    json["f_msg3_rl"]=f_msg3_rl;
    json["f_msg4_rl"]=f_msg4_rl;
    json["ss_speed_rl"]=ss_speed_rl;
    json["ss_curr_time_rl"]=ss_curr_time_rl;
    json["ss_next_stn_rl"]=ss_next_stn_rl;
    json["ss_next_dist_rl"]=ss_next_dist_rl;
    json["ss_eta_rl"]=ss_eta_rl;
    json["st_no"]=st_no;
    json["st_code"]=st_code;
    json["st_name_en"]=st_name_en;
    json["st_name_hi"]=st_name_hi;
    json["st_name_rl"]=st_name_rl;
    json["rl"]=rl;
    json["gps_lat"]=gps_lat;
    json["gps_lon"]=gps_lon;
    json["app_r"]=app_r;
    json["arr_r"]=arr_r;
    json["dep_r"]=dep_r;
    json["time_arr_hh"]=time_arr_hh;
    json["time_arr_mm"]=time_arr_mm;
    json["time_dep_hh"]=time_dep_hh;
    json["time_dep_mm"]=time_dep_mm;
    json["dist_km"]=dist_km;
}

void station::read(const QJsonObject &json)
{
    if (json.contains("tr_zone") && json["tr_zone"].isString())
        tr_zone  = json["tr_zone"].toString();

    if (json.contains("tr_no_en") && json["tr_no_en"].isString())
        tr_no_en  = json["tr_no_en"].toString();

    if (json.contains("tr_nm_en") && json["tr_nm_en"].isString())
        tr_nm_en  = json["tr_nm_en"].toString();

    if (json.contains("tr_no_hi") && json["tr_no_hi"].isString())
        tr_no_hi  = json["tr_no_hi"].toString();

    if (json.contains("tr_nm_hi") && json["tr_nm_hi"].isString())
        tr_nm_hi  = json["tr_nm_hi"].toString();

    if (json.contains("tr_route_en") && json["tr_route_en"].isString())
        tr_route_en  = json["tr_route_en"].toString();

    if (json.contains("tr_route_hi") && json["tr_route_hi"].isString())
        tr_route_hi  = json["tr_route_hi"].toString();

    if (json.contains("tr_via_en") && json["tr_via_en"].isString())
        tr_via_en  = json["tr_via_en"].toString();

    if (json.contains("tr_via_hi") && json["tr_via_hi"].isString())
        tr_via_hi  = json["tr_via_hi"].toString();

    if (json.contains("tr_no_rl") && json["tr_no_rl"].isString())
        tr_no_rl  = json["tr_no_rl"].toString();

    if (json.contains("tr_nm_rl") && json["tr_nm_rl"].isString())
        tr_nm_rl  = json["tr_nm_rl"].toString();

    if (json.contains("tr_route_rl") && json["tr_route_rl"].isString())
        tr_route_rl  = json["tr_route_rl"].toString();

    if (json.contains("tr_via_rl") && json["tr_via_rl"].isString())
        tr_via_rl  = json["tr_via_rl"].toString();

    if (json.contains("w_msg1_hi") && json["w_msg1_hi"].isString())
        w_msg1_hi  = json["w_msg1_hi"].toString();

    if (json.contains("w_msg2_hi") && json["w_msg2_hi"].isString())
        w_msg2_hi  = json["w_msg2_hi"].toString();

    if (json.contains("w_msg3_hi") && json["w_msg3_hi"].isString())
        w_msg3_hi  = json["w_msg3_hi"].toString();

    if (json.contains("w_msg1_en") && json["w_msg1_en"].isString())
        w_msg1_en  = json["w_msg1_en"].toString();

    if (json.contains("w_msg2_en") && json["w_msg2_en"].isString())
        w_msg2_en  = json["w_msg2_en"].toString();

    if (json.contains("w_msg3_en") && json["w_msg3_en"].isString())
        w_msg3_en  = json["w_msg3_en"].toString();

    if (json.contains("w_msg_rl") && json["w_msg_rl"].isString())
        w_msg_rl  = json["w_msg_rl"].toString();

    if (json.contains("w_msg1_rl") && json["w_msg1_rl"].isString())
        w_msg1_rl  = json["w_msg1_rl"].toString();

    if (json.contains("w_msg2_rl") && json["w_msg2_rl"].isString())
        w_msg2_rl  = json["w_msg2_rl"].toString();

    if (json.contains("w_msg3_rl") && json["w_msg3_rl"].isString())
        w_msg3_rl  = json["w_msg3_rl"].toString();

    if (json.contains("f_msg1_hi") && json["f_msg1_hi"].isString())
        f_msg1_hi  = json["f_msg1_hi"].toString();

    if (json.contains("f_msg2_hi") && json["f_msg2_hi"].isString())
        f_msg2_hi  = json["f_msg2_hi"].toString();

    if (json.contains("f_msg3_hi") && json["f_msg3_hi"].isString())
        f_msg3_hi  = json["f_msg3_hi"].toString();

    if (json.contains("f_msg4_hi") && json["f_msg4_hi"].isString())
        f_msg4_hi  = json["f_msg4_hi"].toString();

    if (json.contains("f_msg1_en") && json["f_msg1_en"].isString())
        f_msg1_en  = json["f_msg1_en"].toString();

    if (json.contains("f_msg2_en") && json["f_msg2_en"].isString())
        f_msg2_en  = json["f_msg2_en"].toString();

    if (json.contains("f_msg3_en") && json["f_msg3_en"].isString())
        f_msg3_en  = json["f_msg3_en"].toString();

    if (json.contains("f_msg4_en") && json["f_msg4_en"].isString())
        f_msg4_en  = json["f_msg4_en"].toString();

    if (json.contains("f_msg1_rl") && json["f_msg1_rl"].isString())
        f_msg1_rl  = json["f_msg1_rl"].toString();

    if (json.contains("f_msg2_rl") && json["f_msg2_rl"].isString())
        f_msg2_rl  = json["f_msg2_rl"].toString();

    if (json.contains("f_msg3_rl") && json["f_msg3_rl"].isString())
        f_msg3_rl  = json["f_msg3_rl"].toString();

    if (json.contains("f_msg4_rl") && json["f_msg4_rl"].isString())
        f_msg4_rl  = json["f_msg4_rl"].toString();

    if (json.contains("ss_speed_rl") && json["ss_speed_rl"].isString())
        ss_speed_rl  = json["ss_speed_rl"].toString();

    if (json.contains("ss_curr_time_rl") && json["ss_curr_time_rl"].isString())
        ss_curr_time_rl  = json["ss_curr_time_rl"].toString();

    if (json.contains("ss_next_stn_rl") && json["ss_next_stn_rl"].isString())
        ss_next_stn_rl  = json["ss_next_stn_rl"].toString();

    if (json.contains("ss_next_dist_rl") && json["ss_next_dist_rl"].isString())
        ss_next_dist_rl  = json["ss_next_dist_rl"].toString();

    if (json.contains("ss_eta_rl") && json["ss_eta_rl"].isString())
        ss_eta_rl  = json["ss_eta_rl"].toString();

    if (json.contains("st_no") && json["st_no"].isDouble())
        st_no  = json["st_no"].toInt();

    if (json.contains("st_code") && json["st_code"].isString())
        st_code  = json["st_code"].toString();

    if (json.contains("st_name_en") && json["st_name_en"].isString())
        st_name_en  = json["st_name_en"].toString();

    if (json.contains("st_name_hi") && json["st_name_hi"].isString())
        st_name_hi  = json["st_name_hi"].toString();

    if (json.contains("st_name_rl") && json["st_name_rl"].isString())
        st_name_rl  = json["st_name_rl"].toString();

    if (json.contains("rl") && json["rl"].isString())
        rl  = json["rl"].toString();

    if (json.contains("gps_lat") && json["gps_lat"].isDouble())
        gps_lat  = json["gps_lat"].toDouble();

    if (json.contains("gps_lon") && json["gps_lon"].isDouble())
        gps_lon  = json["gps_lon"].toDouble();

    if (json.contains("app_r") && json["app_r"].isDouble())
        app_r  = json["app_r"].toDouble();

    if (json.contains("arr_r") && json["arr_r"].isDouble())
        arr_r  = json["arr_r"].toDouble();

    if (json.contains("dep_r") && json["dep_r"].isDouble())
        dep_r  = json["dep_r"].toDouble();

    if (json.contains("time_arr_hh") && json["time_arr_hh"].isDouble())
        time_arr_hh  = json["time_arr_hh"].toInt();

    if (json.contains("time_arr_mm") && json["time_arr_mm"].isDouble())
        time_arr_mm  = json["time_arr_mm"].toDouble();

    if (json.contains("time_dep_hh") && json["time_dep_hh"].isDouble())
        time_dep_hh  = json["time_dep_hh"].toInt();

    if (json.contains("time_dep_mm") && json["time_dep_mm"].isDouble())
        time_dep_mm  = json["time_dep_mm"].toInt();

    if (json.contains("dist_km") && json["dist_km"].isDouble())
        dist_km  = json["dist_km"].toDouble();

    time_arr=  QTime(time_arr_hh,time_arr_mm,0,0);
    time_dep=  QTime(time_dep_hh,time_dep_mm,0,0);

}
