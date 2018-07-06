#include "modbus_settings.h"

//******************************************************************//
namespace Modbus {

//******************************************************************//
static inline bool LoadRTUSettingsFromFile(Settings_t *settings, const char *fileName, const char *fieldName) {
    bool ret = true;
    QFile file;                                                                         //JSON file
    file.setFileName(fileName);                                                         //Set name
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {                             //Open for reading in text mode
        qWarning("%s: Could not open file '%s'!\r\n", __func__, fileName);
        return (false);                                                                 //Error
    }
    QByteArray fileData = file.readAll();                                               //Read all data
    file.close();                                                                       //Close the file
    QJsonDocument doc = QJsonDocument::fromJson(fileData);                              //Dobument
    QJsonObject group = doc.object();                                                   //Group
    if(!group.contains(fieldName) || !group[fieldName].isObject()) {                    //If not existing
        qWarning("%s: Field '%s' not found!\r\n", __func__, fieldName);
        return (false);                                                                 //Error
    }
    QJsonObject item = group.value(QString(fieldName)).toObject();                      //Item
    QJsonValue itemVal;                                                                 //Item value
    if(item.contains("port name")) {                                                    //If existing
        itemVal = item["port name"];                                                    //Item value
        if(itemVal.isString()) {
            settings->portNameURL = itemVal.toString().toStdString();                   //Assign
        }
        else {
            qWarning("%s: Invalid value in field 'port name'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'port name' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("baud rate")) {                                                    //If existing
        itemVal = item["baud rate"];                                                    //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if(intVal > 0) {                                                            //Range check
                settings->baudRate = intVal;                                            //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'baud rate'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'baud rate'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'baud rate' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("data bits")) {                                                    //If existing
        itemVal = item["data bits"];                                                    //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if((intVal >= (int)QSerialPort::Data5) &&
               (intVal <= (int)QSerialPort::Data8)) {                                   //Range check
                settings->dataBits = intVal;                                            //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'data bits'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'data bits'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'data bits' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("stop bits")) {                                                    //If existing
        itemVal = item["stop bits"];                                                    //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            double valDbl = itemVal.toDouble();                                         //To double
            if(valDbl == 1) {
                settings->stopBits = (int)QSerialPort::OneStop;                         //Set
            }
            else if(valDbl == 1.5) {
                settings->stopBits = (int)QSerialPort::OneAndHalfStop;                  //Set
            }
            else if(valDbl == 2) {
                settings->stopBits = (int)QSerialPort::TwoStop;                         //Set
            }
            else {
                qWarning("%s: Invalid value in field 'stop bits'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'stop bits'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'stop bits' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("parity")) {                                                       //If existing
        itemVal = item["parity"];                                                       //Item value
        if(itemVal.isString()) {
            string valStr = itemVal.toString().toStdString();                           //To string
            if(valStr == "none") {
                settings->parity = (int)QSerialPort::NoParity;                          //Set
            }
            else if(valStr == "even") {
                settings->parity = (int)QSerialPort::EvenParity;                        //Set
            }
            else if(valStr == "odd") {
                settings->parity = (int)QSerialPort::OddParity;                         //Set
            }
            else {
                qWarning("%s: Invalid value in field 'parity'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'parity'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'parity' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("timeout")) {                                                      //If existing
        itemVal = item["timeout"];                                                      //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if(intVal >= 0) {                                                           //Range check
                settings->timeout = intVal;                                             //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'timeout'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'timeout'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    if(item.contains("retries")) {                                                      //If existing
        itemVal = item["retries"];                                                      //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if(intVal >= 1) {                                                           //Range check
                settings->retries = intVal;                                             //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'retries'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'retries'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
static inline bool LoadTCPSettingsFromFile(Settings_t *settings, const char *fileName, const char *fieldName) {
    bool ret = true;
    QFile file;                                                                         //JSON file
    file.setFileName(fileName);                                                         //Set name
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {                             //Open for reading in text mode
        qWarning("%s: Could not open file '%s'!\r\n", __func__, fileName);
        return (false);                                                                 //Error
    }
    QByteArray fileData = file.readAll();                                               //Read all data
    file.close();                                                                       //Close the file
    QJsonDocument doc = QJsonDocument::fromJson(fileData);                              //Dobument
    QJsonObject group = doc.object();                                                   //Group
    if(!group.contains(fieldName) || !group[fieldName].isObject()) {                    //If not existing
        qWarning("%s: Field '%s' not found!\r\n", __func__, fieldName);
        return (false);                                                                 //Error
    }
    QJsonObject item = group.value(QString(fieldName)).toObject();                      //Item
    QJsonValue itemVal;                                                                 //Item value
    if(item.contains("url")) {                                                          //If existing
        itemVal = item["url"];                                                          //Item value
        if(itemVal.isString()) {
            QUrl url = QUrl::fromUserInput(itemVal.toString());                         //URL
            if(url.isValid()) {
                settings->portNameURL = itemVal.toString().toStdString();               //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'url'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'url'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'url' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("timeout")) {                                                      //If existing
        itemVal = item["timeout"];                                                      //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if(intVal >= 0) {                                                           //Range check
                settings->timeout = intVal;                                             //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'timeout'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'timeout'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    if(item.contains("retries")) {                                                      //If existing
        itemVal = item["retries"];                                                      //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if(intVal >= 1) {                                                           //Range check
                settings->retries = intVal;                                             //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'retries'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'retries'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
bool LoadSettingsFromFile(Settings_t *settings, const char *fileName, const char *fieldName, bool tcp) {
    if(!tcp) {
        return (LoadRTUSettingsFromFile(settings, fileName, fieldName));                //RTU
    }
    else {
        return (LoadTCPSettingsFromFile(settings, fileName, fieldName));                //TCP
    }
}
//******************************************************************//

//******************************************************************//
void PrintSettings(const Settings_t &settings, bool tcp) {
    cout << endl;
    if(tcp) {
        cout << "Modbus TCP settings:" << endl;
        cout << "\tURL: " << settings.portNameURL << endl;
        cout << "\tTimeout [ms]: " << settings.timeout << endl;
        cout << "\tRetries: " << settings.retries << endl;
        return;
    }
    cout << "Modbus RTU settings:" << endl;
    cout << "\tPort name: " << settings.portNameURL << endl;
    cout << "\tBaud rate: " << settings.baudRate << endl;
    cout << "\tData bits: " << settings.dataBits << endl;
    cout << "\tStop bits: ";
    if(settings.stopBits == (int)QSerialPort::OneStop) {
        cout << "1" << endl;
    }
    else if(settings.stopBits == (int)QSerialPort::OneAndHalfStop) {
        cout << "1.5" << endl;
    }
    else if(settings.stopBits == (int)QSerialPort::TwoStop) {
        cout << "2" << endl;
    }
    else {
        cout << "unknown" << endl;
    }
    cout << "\tParity: ";
    if(settings.parity == (int)QSerialPort::NoParity) {
        cout << "none" << endl;
    }
    else if(settings.parity == (int)QSerialPort::EvenParity) {
        cout << "even" << endl;
    }
    else if(settings.parity == (int)QSerialPort::OddParity) {
        cout << "odd" << endl;
    }
    else {
        cout << "unknown" << endl;
    }
    cout << "\tTimeout [ms]: " << settings.timeout << endl;
    cout << "\tRetries: " << settings.retries << endl;
}
//******************************************************************//

}
//******************************************************************//
