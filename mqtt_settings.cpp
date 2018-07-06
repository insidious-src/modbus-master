#include "mqtt_settings.h"

namespace MQTT {

//******************************************************************//
bool LoadSettingsFromFile(Settings_t *settings, const char *fileName, const char *fieldName) {
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
    if(item.contains("host")) {                                                         //If existing
        itemVal = item["host"];                                                         //Item value
        if(itemVal.isString()) {
            settings->host = itemVal.toString().toStdString();                          //Assign
        }
        else {
            qWarning("%s: Invalid value in field 'host'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'host' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("port")) {                                                         //If existing
       itemVal = item["port"];                                                          //Item value
       if(itemVal.isDouble()) {                                                         //Type check
           int intVal = (int)itemVal.toDouble();                                        //To double and integer
           if(intVal >= 0 && intVal <= UINT16_MAX) {                                    //Range check
               settings->port = intVal;                                                 //Assign
           }
           else {
               qWarning("%s: Invalid value in field 'port'!\r\n", __func__);
               ret = false;                                                             //Error
           }
       }
       else {
           qWarning("%s: Invalid value in field 'port'!\r\n", __func__);
           ret = false;                                                                 //Error
       }
    }
    else {
        qWarning("%s: Field 'port' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("username")) {                                                     //If existing
        itemVal = item["username"];                                                     //Item value
        if(!itemVal.isNull()) {                                                         //If exsisting
            if(itemVal.isString()) {
                settings->username = itemVal.toString().toStdString();                  //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'username'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
    }
    else {
        qWarning("%s: Field 'username' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("password")) {                                                     //If existing
        itemVal = item["password"];                                                     //Item value
        if(!itemVal.isNull()) {                                                         //If exsisting
            if(itemVal.isString()) {
                settings->password = itemVal.toString().toStdString();                  //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'password'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
    }
    else {
        qWarning("%s: Field 'password' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("keep-alive")) {                                                   //If existing
        itemVal = item["keep-alive"];                                                   //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if((intVal >= 0) && (intVal <= UINT16_MAX)) {                               //Range check
                settings->keepAlive = intVal;                                           //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'keep-alive'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'keep-alive'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'keep-alive' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("qos pub")) {                                                      //If existing
        itemVal = item["qos pub"];                                                      //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if((intVal >= 0) && (intVal <= 2)) {                                        //Range check
                settings->qosPub = intVal;                                              //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'qos pub'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'qos pub'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'qos pub' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("qos sub")) {                                                      //If existing
        itemVal = item["qos sub"];                                                      //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if((intVal >= 0) && (intVal <= 2)) {                                        //Range check
                settings->qosSub = intVal;                                              //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'qos sub'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'qos sub'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'qos sub' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("retain")) {                                                       //If existing
        itemVal = item["retain"];                                                       //Item value
        if(itemVal.isBool()) {                                                          //Type check
            settings->retain = itemVal.toBool();                                        //Assign
        }
        else {
            qWarning("%s: Invalid value in field 'retain'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'retain' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    if(item.contains("reconnect delay")) {                                              //If existing
        itemVal = item["reconnect delay"];                                              //Item value
        if(itemVal.isDouble()) {                                                        //Type check
            int intVal = (int)itemVal.toDouble();                                       //To double and integer
            if(intVal >= 0) {                                                           //Range check
                settings->reconnectDelay = intVal;                                      //Assign
            }
            else {
                qWarning("%s: Invalid value in field 'reconnect delay'!\r\n", __func__);
                ret = false;                                                            //Error
            }
        }
        else {
            qWarning("%s: Invalid value in field 'reconnect delay'!\r\n", __func__);
            ret = false;                                                                //Error
        }
    }
    else {
        qWarning("%s: Field 'reconnect delay' not found!\r\n", __func__);
        ret = false;                                                                    //Error
    }
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
void PrintSettings(const Settings_t &settings) {
    cout << endl;
    cout << "MQTT settings:" << endl;
    cout << "\tHost: " << settings.host << endl;
    cout << "\tPort: " << settings.port << endl;
    cout << "\tUsername: " << settings.username << endl;
    cout << "\tPassword: " << settings.password << endl;
    cout << "\tKeep-alive [s]: " << settings.keepAlive << endl;
    cout << "\tQoS level for publish: " << settings.qosPub << endl;
    cout << "\tQoS level for subscribe: " << settings.qosSub << endl;
    cout << "\tRetain: " << boolalpha << settings.retain << endl;
    cout << "\tReconnect delay [s]: " << settings.reconnectDelay << endl;
}
//******************************************************************//

}
