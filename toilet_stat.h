#ifndef TOILET_STAT_H
#define TOILET_STAT_H

//#include <QObject>

class toilet_stat
{
//    Q_OBJECT
public:
    toilet_stat();
//    ~toilet_stat();

    unsigned char addr;
    unsigned char occupancy1;
    unsigned char emergency1;


};

#endif // TOILET_STAT_H
