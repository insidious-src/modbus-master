#ifndef MODBUS_SETTINGS_H
#define MODBUS_SETTINGS_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QSerialPort>
#include <QUrl>
#include <limits>
#include <iostream>

using namespace std;

//******************************************************************//
namespace Modbus {

//******************************************************************//
typedef struct Settings_t {
    Settings_t()
        :
        portNameURL(""),
        baudRate(19200),
        dataBits((int)QSerialPort::Data8),
        stopBits((int)QSerialPort::OneStop),
        parity((int)QSerialPort::EvenParity),
        timeout(3000),
        retries(3) {
    }
    Settings_t(
        const char *portNameURL_,
        int baudRate_,
        int dataBits_,
        int stopBits_,
        int parity_,
        int timeout_,
        int retries_
        )
    :   portNameURL(portNameURL_),
        baudRate(baudRate_),
        dataBits(dataBits_),
        stopBits(stopBits_),
        parity(parity_),
        timeout(timeout_),
        retries(retries_) {
    }
    string portNameURL;                                                                 //Port name or URL
    int baudRate;                                                                       //Baud rate
    int dataBits;                                                                       //Data bits
    int stopBits;                                                                       //Stop bits
    int parity;                                                                         //Parity
    int timeout;                                                                        //Timeout
    int retries;                                                                        //Retries
} Settings_t;
//******************************************************************//

//******************************************************************//
bool LoadSettingsFromFile(Settings_t *settings, const char *fileName, const char *fieldName, bool tcp);
void PrintSettings(const Settings_t &settings, bool tcp);
//******************************************************************//

}
//******************************************************************//

#endif // MODBUS_SETTINGS_H
