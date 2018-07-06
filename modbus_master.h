#ifndef MODBUS_MASTER_H
#define MODBUS_MASTER_H

#include <QTimerEvent>
#include <QSerialPort>
#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QAbstractEventDispatcher>
#include <QTime>
#include <QTimer>
#include <QUrl>
#include <QVariant>
#include <QDebug>
#include <iostream>
#include <memory>
#include <functional>
#include <iomanip>
#include "modbus_settings.h"

//******************************************************************//

namespace Modbus {

class Timer : public QObject
{
    Q_OBJECT
public:
    typedef std::function<void()> fn_type;

    void timerEvent(QTimerEvent* ev)
    {
        if (ev->timerId() == m_timer.timerId())
        {
            start();
            m_fn ();
        }
    }

    void start()
    {
        m_timer.setTimerType(Qt::CoarseTimer);
        m_timer.start(5000);
    }

    template <typename Processor>
    Timer (Processor fn) : m_fn (fn)
    { }

private:
    QTimer  m_timer { this };
    fn_type m_fn;

};

//******************************************************************//


inline int msecsTo (const QTime& at)
{
    constexpr const int msecsPerDay = 24 * 60 * 60 * 1000;
    int msecs = QTime::currentTime().msecsTo(at);

    if (msecs < 0) msecs += msecsPerDay;
    return msecs;
}

template <typename Processor>
inline void runAt (QTime const& at, Processor job)
{
    // Timer ownership prevents timer leak when the thread terminates.
    auto timer = std::make_shared<QTimer>(QAbstractEventDispatcher::instance());

    timer->setTimerType(Qt::CoarseTimer);
    timer->start(msecsTo(at));

    auto conn = std::make_shared<QMetaObject::Connection>();

    *conn = QObject::connect(timer.get(), &QTimer::timeout, [job, conn, timer]
    {
        job();
        QObject::disconnect(*conn);
        //timer->deleteLater();
    });
}

//******************************************************************//

struct Response {
    Response() : id(0), errorCode(QModbusDevice::NoError) {}
    Response(int id_, QModbusDataUnit &dataUnit_, QModbusDevice::Error errorCode_) {
        this->id = id_;
        this->dataUnit = dataUnit_;
        this->errorCode = errorCode_;
    }
    int id;
    QModbusDataUnit dataUnit;
    QModbusDevice::Error errorCode;
};

//******************************************************************//

//******************************************************************//

class cMaster : public QObject {

    Q_OBJECT

public:

    //**************************************************************//
    cMaster();
    cMaster(bool tcp);
    cMaster(bool tcp, Settings_t &settings);
    cMaster(bool tcp, const char *settingsFileName, const char *fieldName);
    virtual ~cMaster();
    void LoadSettings(const Settings_t &settings);
    bool Connect(void);
    void Disconnect(void);
    bool IsConnected(void);
    QModbusDevice::State GetState(void);
    bool ReadInputRegisters(int unit_id, int start_addr, int reg_count);
    bool WriteHoldingRegisters(int unit_id, int start_addr, int *data, int reg_count);
    void PrintSettings();
    //**************************************************************//

protected:

signals:

    //**************************************************************//
    void DataReady(Response response);
    void OnConnected(void);
    void OnDisconnected(void);
    //**************************************************************//

private:

    //**************************************************************//
    QModbusClient *_device;                                                             //Modbus device (RTU or TCP)
    bool _tcp;                                                                          //TCP connection
    Settings_t *_settings;                                                              //Settings
    //**************************************************************//

private slots:

    //**************************************************************//
    void ReadWriteReady(void);
    void OnStateChanged(void);
    //**************************************************************//

};
//******************************************************************//

//******************************************************************//
void PrintResponse(Response &response);
//******************************************************************//

}
//******************************************************************//

//******************************************************************//
Q_DECLARE_METATYPE(Modbus::Response)
//******************************************************************//

#endif // MODBUS_MASTER_H
