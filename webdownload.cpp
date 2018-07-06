#include <webdownload.h>
#include <QtCore/QDateTime>
#include <QtCore/QThread>
#include <iostream>
#include <QEventLoop>

namespace {

inline QString dateToDecimal(QDateTime dateTime)
{
    int vantageDateStamp = dateTime.date().day  ()         +
                           dateTime.date().month() *   32  +
                          (dateTime.date().year () - 2000) * 512;
    int vantageTimeStamp = 100 * dateTime.time().hour() + dateTime.time().minute();

    return QString::number((vantageDateStamp << 16) | vantageTimeStamp);
}

} // anonymous namespace

// ================================================================

WebDownload::WebDownload(QDateTime dateTime, const string_type& user, const string_type& pass)
: QObject(nullptr),
  m_url  ("http://weatherlink.com/webdl.php?timestamp=" + dateToDecimal(dateTime) +
          "&user=" + user +
          "&pass=" + pass +
          "&action=data")
{

    std::cout << m_url.toString().toStdString() << std::endl;

    if (!download())
    {
        std::cout << "failed to connect to weatherlink!\n";
        exit(1);
    }
}

bool WebDownload::download()
{
    connect(&m_mgr, &QNetworkAccessManager::finished, this, &WebDownload::onReplyFinished);

    QNetworkRequest request(m_url);
    
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Upgrade-Insecure-Requests", "1");
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (X11; Linux x86_64) "
                         "AppleWebKit/537.36 (KHTML, like Gecko) "
                         "Chrome/66.0.3359.181 Safari/537.36");
    request.setRawHeader("Accept",
                         "text/html,application/xhtml+xml,application/xml;"
                         "q=0.9,image/webp,image/apng,*/*;q=0.8");
    request.setRawHeader("Accept-Encoding", "gzip, deflate");
    request.setRawHeader("Accept-Language", "en-US,en;q=0.9");
    request.setRawHeader("DNT", "1");
    
    QNetworkReply* reply = m_mgr.get(request);

    std::cout << "requesting download...\n";

    connect(reply, &QNetworkReply::readyRead, this, &WebDownload::onReadyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &WebDownload::onUpdateProgress);

    QEventLoop eventLoop;
    eventLoop.exec();

    ReplyTimeout::set(reply, 3000);

    return reply->isRunning() or reply->isFinished();
}

void WebDownload::onUpdateProgress(qint64 bytesReceived, qint64)
{
    std::cout << bytesReceived << " bytes received\n";
}

void WebDownload::onReadyRead()
{
    std::cout << "request processing...\n";
}

void WebDownload::onReplyFinished(QNetworkReply* reply)
{
    if(reply->error())
    {
        std::cout << "download failed! " << reply->errorString().toStdString() << std::endl;
    }
    else
    {
        std::cout <<
            reply->header(QNetworkRequest::ContentTypeHeader).toString().toStdString() << std::endl;
        std::cout <<
            reply->header(QNetworkRequest::LastModifiedHeader).toDateTime ().toString().toStdString() << std::endl;
        std::cout <<
            reply->header(QNetworkRequest::ContentLengthHeader).toULongLong() << std::endl;
        std::cout <<
            reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << std::endl;
        std::cout <<
            reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString().toStdString() << std::endl;

        if (reply->size())
        {
            std::cout << reply->size() << " bytes downloaded\n";
            m_data = reply->readAll();

            std::cout << "Solar Radiation: "
                      << data().value<VantageReader::DataType::SolarRadiationWm2, int> ()
                      << " W/m2\n";
        }
        else
        {
            std::cout << "empty download!\n";
        }
    }

    reply->deleteLater();
}
