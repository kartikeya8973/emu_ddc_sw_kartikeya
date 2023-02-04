#ifndef ROUTEDATA_H
#define ROUTEDATA_H


#include <QString>
#include <QTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include "station.h"

class routeData
{
public:
    routeData();

    QVector<QString> slogans={
        "बिना टिकट यात्रा करना दंडनीय अपराध है।",
        "Travelling without a ticket is a punishable offence.",
        "कृपया चलती गाड़ी में न चढ़ें।",
        "Please do not board a running train",
        "कृपया अपने-पास के लावारिस सामान की सूचना रेल कर्मचारी को दें",
        "Please check and inform about unattended things to a on duty railway staff.",
        "कोई भी लावारिस वस्तु जैसे ब्रीफ़केस, थर्मस, और ट्रांजिस्टर बम हो सकता है",
        "Any unattended or suspicious article like bag, thermos, radio or mobile could be a bomb.",
        "गाडी में और स्टेशन पर जब कतरों से सावधान रहें",
        "Beware of pick-pockets in train and station area.",
        "अपने सामान को लावारिस न छोड़ें",
        "Do not keep your luggage unattended.",
        "कृपया अपना सामान साथ ले जाना न भूलें",
        "Please remember to take your belongings.",
        "गाडी को स्वच्छ रखने में हमारी सहायता करें",
        "Please help us keep the train clean.",
        "सुरक्षा जांच में कृपया सहयोग करें",
        "Please help us in security checking.",
        "कृपया ज्वलनशील पदार्थ के साथ यात्रा न करें",
        "Please do not carry inflammable materials in the train.",
        "कृपया गाड़ी में धूम्रपान न करें",
        "Please do not smoke in the train.",
        "कृपया गाडी में गंदगी न फैलाएं",
        "Please do not throw garbage in the train.",
        "कृपया अनजान व्यक्ति से खाने-पीने की वस्तु न लें",
        "Please do not accept any eatables from any unknown person.",
        "अपने सामान की स्वयं हिफाज़त करें",
        "Please ensure the safety of your own luggage.",
        "कृपया कोच में मदिरा सेवन न करें",
        "Please do not consume liquor in train.",
        "किसी भी प्रकार की सहायता हेतु उपस्थित रेल कर्मचारी से संपर्क करें",
        "Please contact on duty railway staff for any help.",
        "सुरक्षित यात्रा हेतु रेलवे से सहयोग करें",
        "Please cooperate with railways for safe journey.",
        "किसी भी अनजान व्यक्ति, जो यात्रा के दौरान आपसे मित्रता करने का प्रयत्न कर रहा हो, से कोई भी खाद्य पदार्थ तथा पेय पदार्थ तथा भोजन, बिस्कुट, नमकीन, प्रसाद, फल, चाय, कॉफी, कोल्डडिंक (षीतलपेय),  यहां तक कि पानी भी न लेे  क्यूंकि इन खाद्य पदाथो र्म  जहरीले या सुला देने वाले रासायनिक तत्व मिले या लगे हुए हो सकते है। कृपया सावधान रहें।",
        "Please do not accept any eatable/beverages like food, biscuits, snacks, “Prasad”, fruits, tea, coffee, cold drinks, water etc. from the unknown passengers especially those who try to befriend you during journey. These eatables may be laced with sedatives/poison. Please be careful."
    };

    QString e_msg_en                ="WELCOME TO INDIAN RAILWAYS";
    QString w_msg_hi                ="भारतीय रेल में सभी यात्रियों का स्वागत है";
    QString w_msg_en                ="Indian Railways Welcomes You";
    QString ss_speed_en             ="Train Speed";
    QString ss_curr_time_en         ="Current Time";
    QString ss_next_stn_en          ="Next Station";
    QString ss_next_dist_en         ="Distance";
    QString ss_eta_en               ="Estimated Arrival Time";
    QString ss_km_en                ="km";
    QString ss_hr_en                ="hr";
    QString ss_min_en               ="min";    

    QString ss_speed_hi             ="ट्रेन की गति";
    QString ss_curr_time_hi         ="वर्तमान समय";
    QString ss_next_stn_hi          ="अगला स्टेशन";
    QString ss_next_dist_hi         ="दूरी";
    QString ss_eta_hi               ="पहुँचने का अनुमानित समय";
    QString ss_km_hi                ="किमी";
    QString ss_hr_hi                ="घंटा";
    QString ss_min_hi               ="मिनट";
    QList <station> st_list;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void write_s(QJsonObject &json) const;
};


#endif // ROUTEDATA_H
