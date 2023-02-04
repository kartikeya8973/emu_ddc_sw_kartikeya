QT       += core gui network sql positioning multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
requires(qtConfig(tableview))

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alarmwindow.cpp \
    audioplayerthread.cpp \
    customle.cpp \
    data.cpp \
    devicetestdialog.cpp \
    downloadmanager.cpp \
    execdialog.cpp \
    gpio_utils.cpp \
    idleeventfilter.cpp \
    keyboarddialog.cpp \
    lineeditdialog.cpp \
    logindialog.cpp \
    main.cpp \
    maintenancewindow.cpp \
    mainwindow.cpp \
    menuwindow.cpp \
    route.cpp \
    routedata.cpp \
    routes.cpp \
    run.cpp \
    securitywindow.cpp \
    servicedatawindow.cpp \
    servicerecord.cpp \
    slave.cpp \
    slavedriverthread.cpp \
    slavesyncthread.cpp \
    splashwindow.cpp \
    station.cpp \
    subsystemthread.cpp \
    systemconfig.cpp \
    systemdeviceswindow.cpp \
    systeminfowindow.cpp \
    systemsettingswindow.cpp \
    toilet_stat.cpp \
    toiletmanagerthread.cpp \
    user.cpp \
    userdialog.cpp

HEADERS += \
    alarmwindow.h \
    audioplayerthread.h \
    common.h \
    connection.h \
    crc.h \
    customle.h \
    data.h \
    devicetestdialog.h \
    downloadmanager.h \
    execdialog.h \
    gpio_utils.h \
    idleeventfilter.h \
    keyboarddialog.h \
    lineeditdialog.h \
    logindialog.h \
    maintenancewindow.h \
    mainwindow.h \
    menuwindow.h \
    route.h \
    routedata.h \
    routes.h \
    run.h \
    securitywindow.h \
    servicedatawindow.h \
    servicerecord.h \
    slave.h \
    slavedriverthread.h \
    slavesyncthread.h \
    splashwindow.h \
    station.h \
    subsystemthread.h \
    systemconfig.h \
    systemdeviceswindow.h \
    systeminfowindow.h \
    systemsettingswindow.h \
    toilet_stat.h \
    toiletmanagerthread.h \
    user.h \
    userdialog.h

FORMS += \
    alarmwindow.ui \
    devicetestdialog.ui \
    execdialog.ui \
    keyboarddialog.ui \
    lineeditdialog.ui \
    logindialog.ui \
    maintenancewindow.ui \
    mainwindow.ui \
    menuwindow.ui \
    securitywindow.ui \
    servicedatawindow.ui \
    splashwindow.ui \
    systemdeviceswindow.ui \
    systeminfowindow.ui \
    systemsettingswindow.ui \
    userdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/anim/anim_bars.gif \
    images/anim/anim_radio.gif \
    images/icons/call_pick.png \
    images/icons/call_reject.png \
    images/icons/check.png \
    images/icons/emergency.png \
    images/icons/error.png \
    images/icons/icon_audioL.png \
    images/icons/icon_bmu.png \
    images/icons/icon_bmuL.png \
    images/icons/icon_checkL.png \
    images/icons/icon_close.png \
    images/icons/icon_day.png \
    images/icons/icon_dayL.png \
    images/icons/icon_display.png \
    images/icons/icon_displayL.png \
    images/icons/icon_driver.png \
    images/icons/icon_driverL.png \
    images/icons/icon_errorL.png \
    images/icons/icon_eth.png \
    images/icons/icon_ethL.png \
    images/icons/icon_gps.png \
    images/icons/icon_gpsL.png \
    images/icons/icon_home.png \
    images/icons/icon_info.png \
    images/icons/icon_infoL.png \
    images/icons/icon_lock.png \
    images/icons/icon_lockL.png \
    images/icons/icon_lte.png \
    images/icons/icon_lteL.png \
    images/icons/icon_maintenance.png \
    images/icons/icon_maintenanceL.png \
    images/icons/icon_menu.png \
    images/icons/icon_night.png \
    images/icons/icon_nightL.png \
    images/icons/icon_restart.png \
    images/icons/icon_sdcard.png \
    images/icons/icon_sdcardL.png \
    images/icons/icon_server.png \
    images/icons/icon_serverL.png \
    images/icons/icon_settings.png \
    images/icons/icon_settings_L.png \
    images/icons/icon_shutdown.png \
    images/icons/icon_siren.png \
    images/icons/icon_ss.png \
    images/icons/icon_toilet.png \
    images/icons/icon_train.png \
    images/icons/icon_trainL.png \
    images/icons/icon_usb.png \
    images/icons/icon_usbL.png \
    images/icons/icon_wifi.png \
    images/icons/icon_wifiL.png \
    imagre  es/ir_logo.png \
    images/rtv_logo.png

RESOURCES += \
    resources.qrc


