#ifndef MQTT_SETTINGS_H
#define MQTT_SETTINGS_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <limits>
#include <iostream>

using namespace std;

//******************************************************************//
namespace MQTT {

//******************************************************************//
typedef struct Settings_t {
    Settings_t()
    :   host("localhost"),
        port(1883),
        username(""),
        password(""),
        keepAlive(60),
        qosPub(1),
        qosSub(1),
        retain(false),
        reconnectDelay(1) {
    }
    Settings_t(
        const char *host_,
        int port_,
        const char *username_,
        const char *password_,
        int keepAlive_,
        int qosPub_,
        int qosSub_,
        bool retain_,
        int reconnectDelay_
    )
    :   host(host_),
        port(port_),
        username(username_),
        password(password_),
        keepAlive(keepAlive_),
        qosPub(qosPub_),
        qosSub(qosSub_),
        retain(retain_),
        reconnectDelay(reconnectDelay_) {
    }
    string host;                                                                        //Host
    int port;                                                                           //Port
    string username;                                                                    //Username
    string password;                                                                    //Password
    int keepAlive;                                                                      //Keep-alive
    int qosPub;                                                                         //QoS for publish
    int qosSub;                                                                         //QoS for subscribe
    bool retain;                                                                        //Retain message
    int reconnectDelay;                                                                 //Re-connect delay
} Settings_t;
//******************************************************************//

//******************************************************************//
bool LoadSettingsFromFile(Settings_t *settings, const char *fileName, const char *fieldName);
void PrintSettings(const Settings_t &settings);
//******************************************************************//

}
//******************************************************************//

#endif // MQTT_SETTINGS_H
