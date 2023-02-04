#include "toiletmanagerthread.h"
extern QList <slave*> papis_slaves;

toiletmanagerthread::toiletmanagerthread()
{

}

//void toiletmanagerthread::serverReadyRead()
//{
//    QByteArray arr=sock->readAll();

//    if(arr.size()==4)
//    {
//        if(arr.at(0)=='*' && arr.at(3)=='#')
//        {
//            emit message_pass("Status Acquired");
//            toilet_packet_t packet = *(toilet_packet_t*)arr.data();

//             toilet_stat toilet;
//             toilet.addr = base + 1;
//             toilet.clogged=packet.t_packet1.clogged_sensor;
//             toilet.curtain=packet.t_packet1.carten_open_close;
//             toilet.occupancy=packet.t_packet1.occupancy;
//             toilet.stationery=packet.t_packet1.stationary_water;
//             toilet.sw=packet.t_packet1.emergency_button;
//             toilet.water_level=packet.t_packet1.water_leval_medium;

//             printf("\n Toilet1: Switch: %d", toilet.sw);
//             printf("\n Toilet1: stationery: %d", toilet.sw);

//             emit toilet_status(toilet);

//             toilet.addr = base + 2;
//             toilet.clogged=packet.t_packet2.clogged_sensor;
//             toilet.curtain=packet.t_packet2.carten_open_close;
//             toilet.occupancy=packet.t_packet2.occupancy;
//             toilet.stationery=packet.t_packet2.stationary_water;
//             toilet.sw=packet.t_packet2.emergency_button;
//             toilet.water_level=packet.t_packet2.water_leval_medium;
//             emit toilet_status(toilet);

//             printf("\n Toilet2: Switch: %d", toilet.sw);
//             printf("\n Toilet2: occupancy: %d", toilet.occupancy);
//        }
//    }

//    sock->disconnect();
//    sock->close();
//}

void toiletmanagerthread::run()
{
    int idx;
    while(1)
    {
        for(idx=0; idx< papis_slaves.count();idx++)
        {
            if((papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE
                &&papis_slaves.at(idx)->conn_mode=="ETH"&&papis_slaves.at(idx)->device_type==DEV_TMS))
            {
                sock = new QTcpSocket();
                QString ip=papis_slaves.at(idx)->ip_addr;
                emit message_pass("Toilet-"+QString::number(papis_slaves.at(idx)->rs485_addr-60)+" Get Status");
                if(ip.contains("161"))
                {
                    base = 1;
                }
                else
                {
                    base = 2;
                }
                sock->connectToHost(ip,24);

                if(sock->waitForConnected(3000))
                {
                    QString msg = "*dt#";
                    sock->write(msg.toLocal8Bit());

                   // connect(sock,SIGNAL(readyRead()),this,SLOT(serverReadyRead()));
                    sock->waitForReadyRead();

                    QByteArray arr=sock->readAll();

//                    QByteArray arr=sock->readAll();

                    if(arr.size()==6)
                    {
                        if(arr.at(0)=='*' && arr.at(5)=='#')
                        {
                            emit message_pass("Status Acquired");
                            toilet_packet_t packet = *(toilet_packet_t*)arr.data();
                            emit toilet_status(base,packet);


//                             toilet_stat toilet;
//                             toilet.addr = base + 1;
//                             toilet.occupancy=packet.occupancy;
//                             toilet.sw=packet.emergency_button;

//                             printf("\n Toilet1: Switch: %d", toilet.sw);
//                             printf("\n Toilet1: stationery: %d", toilet.sw);


//                             toilet.addr = base + 2;
//                             toilet.clogged=packet.t_packet2.clogged_sensor;
//                             toilet.curtain=packet.t_packet2.carten_open_close;
//                             toilet.occupancy=packet.t_packet2.occupancy;
//                             toilet.stationery=packet.t_packet2.stationary_water;
//                             toilet.sw=packet.t_packet2.emergency_button;
//                             toilet.water_level=packet.t_packet2.water_leval_medium;
//                             emit toilet_status(toilet);

//                             printf("\n Toilet2: Switch: %d", toilet.sw);
//                             printf("\n Toilet2: occupancy: %d", toilet.occupancy);
                        }
                    }

                    sock->disconnect();
                    sock->close();



                }
                else
                {

                }
            }
        }

        this->msleep(10000);
    }
}
