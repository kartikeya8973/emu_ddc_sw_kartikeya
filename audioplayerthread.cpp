#include "audioplayerthread.h"

audioplayerthread::audioplayerthread()
{

}

void audioplayerthread::add(QString filePath)
{
    pa_queue.enqueue(filePath);
    announcementType_queue.enqueue(ANN_LOCAL);
}

void audioplayerthread::add(QString filePath, announcementType_t announcementType)
{
    pa_queue.enqueue(filePath);
    announcementType_queue.enqueue(announcementType);
}

void audioplayerthread::play()
{
    wc_a.wakeAll();
}

void audioplayerthread::run()
{
    while(1)
    {
        m_a.lock();
        wc_a.wait(&m_a);
        m_a.unlock();

        while(pa_queue.count()>0)
        {
            QString path = pa_queue.dequeue();

            switch (announcementType_queue.dequeue()) {
                case ANN_LOCAL:
                       system((QString("gst-launch-1.0 playbin uri=file://")+
                       path).toLocal8Bit());
                    break;
                case ANN_NETWORK:
                        system((QString("gst-launch-1.0 filesrc location="+path+" ! decodebin ! tee name=t ! queue ! audioconvert ! autoaudiosink t. ! queue ! audioconvert ! audioresample ! rtpL24pay ! udpsink host=239.1.1.1 auto-multicast=true port=5000")).toLocal8Bit());

                        /* gst-launch-1.0 filesrc location=/home/root/ddc_sw/prerec_audio/1_eng_attention.mp3
                         * ! decodebin ! tee name=t ! queue ! audioconvert ! autoaudiosink t. ! queue
                         * ! audioconvert ! audioresample ! rtpL24pay ! udpsink host=239.1.1.1 auto-multicast=true port=5000 */
                    break;
            }
            //local announcement


            //streaming announcement
        }
    }
}
