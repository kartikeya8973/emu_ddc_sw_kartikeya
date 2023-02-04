#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>

class DownloadManager : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentDownloads;
    QVector<QNetworkReply *> currentUploads;
    QString savePath;

public:
//    explicit DownloadManager(QObject *parent = nullptr);
    DownloadManager();
    void doDownload(const QUrl &url, QString save_path);
    void doUpload(const QUrl &url, QString file_path);
    bool saveToDisk(const QString &filename, QIODevice *data);
    static bool isHttpRedirect(QNetworkReply *reply);

public slots:
    void downloadFinished(QNetworkReply *reply);
    void uploadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);

signals:
    void message(QString);

private:
    QByteArray up_data;

};

#endif // DOWNLOADMANAGER_H
