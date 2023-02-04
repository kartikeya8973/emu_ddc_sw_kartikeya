#include "downloadmanager.h"

DownloadManager::DownloadManager()
{

}

void DownloadManager::doDownload(const QUrl &url, QString save_path)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    connect(&manager, &QNetworkAccessManager::finished,
            this, &DownloadManager::downloadFinished);
#if QT_CONFIG(ssl)
    connect(reply, &QNetworkReply::sslErrors,
            this, &DownloadManager::sslErrors);
#endif

    currentDownloads.append(reply);
    savePath = save_path;
}

void DownloadManager::doUpload(const QUrl &url, QString file_path)
{
    QFile loadFile(file_path);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        return;
    }
    else
    {
        up_data = loadFile.readAll();
        QNetworkRequest request(url);
        QNetworkReply *reply = manager.put(request,up_data);

        connect(&manager, &QNetworkAccessManager::finished,
                this, &DownloadManager::uploadFinished);

        loadFile.close();

#if QT_CONFIG(ssl)
    connect(reply, &QNetworkReply::sslErrors,
            this, &DownloadManager::sslErrors);
#endif

    currentUploads.append(reply);
    }
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

bool DownloadManager::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::sslErrors(const QList<QSslError> &sslErrors)
{
#if QT_CONFIG(ssl)
    for (const QSslError &error : sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        emit message("File Downloaded Failed: "+reply->errorString());
    } else {
        if (isHttpRedirect(reply)) {
            emit message("Request was Redirected!");
        } else {
            if (saveToDisk(savePath, reply)) {

                emit message("File Downloaded Successfully");
            }
        }
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();

    if (currentDownloads.isEmpty()) {
        // all downloads finished
        disconnect(&manager, &QNetworkAccessManager::finished,
                this, &DownloadManager::downloadFinished);
    }
}

void DownloadManager::uploadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        QString str = reply->errorString();
        emit message("File Upload Failed: ");
    } else {
        if (isHttpRedirect(reply)) {
            emit message("Request was Redirected!");
        } else {
                emit message("File Uploaded Successfully");
        }
    }

    currentUploads.removeAll(reply);
    reply->deleteLater();

    if (currentUploads.isEmpty()) {
        // all downloads finished
        disconnect(&manager, &QNetworkAccessManager::finished,
                this, &DownloadManager::uploadFinished);
    }
}

