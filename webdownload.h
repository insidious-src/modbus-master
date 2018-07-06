#ifndef WEBDOWNLOAD_H
#define WEBDOWNLOAD_H

#include <vantagedata.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QUrl>
#include <QtCore/QByteArray>
#include <QtCore/QBasicTimer>
#include <QtCore/QTimerEvent>
#include <iostream>

class ReplyTimeout : public QObject
{
    Q_OBJECT
public:
    ReplyTimeout(QNetworkReply* reply, const int timeout) : QObject(reply)
    {
        Q_ASSERT(reply);

        if (reply && reply->isRunning())
        {
            m_timer.start(timeout, this);
            connect(reply, &QNetworkReply::finished, this, &QObject::deleteLater);
        }
    }
    
    static void set(QNetworkReply* reply, const int timeout)
    {
        new ReplyTimeout(reply, timeout);
    }

private:
    void timerEvent(QTimerEvent* ev)
    {
        if (!m_timer.isActive() || ev->timerId() != m_timer.timerId()) return;
        auto reply = static_cast<QNetworkReply*>(parent());

        if (reply->isRunning())
        {
            reply->abort();
            m_timer.stop();

            std::cout << "request timed out!\n";
        }
    }

    QBasicTimer m_timer;
};

// ================================================================

class WebDownload : public QObject
{
    Q_OBJECT
public:
    typedef QString string_type;

    WebDownload(QDateTime, const string_type& user, const string_type& pass);
    bool download();

    VantageReader data() const
    { return VantageReader(m_data); }

public slots:
    void onReplyFinished(QNetworkReply* reply);
    void onUpdateProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onReadyRead();

private:
    QNetworkAccessManager m_mgr { this };
    QUrl                  m_url         ;
    QByteArray            m_data        ;
};

#endif // WEBDOWNLOAD_H
