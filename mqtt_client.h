#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <mosquittopp.h>
#include <QObject>
#include <QVariant>
#include <QDebug>
#include <iostream>
#include "mqtt_settings.h"

using namespace std;

namespace MQTT {

//******************************************************************//
struct Message {
    Message() : topic(""), payload(""), payloadLen(0) {}
    Message(const char *topic_, const void *payload_, int paylaodLen_) {
        this->topic = topic_;
        this->payload.assign((const char *)payload_, paylaodLen_);
        this->payloadLen = paylaodLen_;
    }
    string topic;
    string payload;
    int payloadLen;
};
//******************************************************************//

//******************************************************************//
class cClient : public QObject, public mosqpp::mosquittopp {

    Q_OBJECT

public:

    //**************************************************************//
    cClient(const char *id);
    cClient(const char *id, Settings_t &settings);
    cClient(const char *id, const char *settingsFileName, const char *fieldName);
    virtual ~cClient();
    void LoadSettings(const Settings_t &settings);
    int Connect(void);
    int Disconnect(void);
    int ReConnect(void);
    int Start(void);
    int Stop(void);
    int UnSubscribe(const char *topic);
    int PublishText(const char *topic, const char *payload);
    int PublishText(const char *topic, const char *payload, int qos);
    int PublishText(const char *topic, const char *payload, int qos, bool retain);
    int PublishText(const char *id, const char *topic, const char *payload);
    int PublishText(const char *id, const char *topic, const char *payload, int qos);
    int PublishText(const char *id, const char *topic, const char *payload, int qos, bool retain);
    int PublishBin(const char *topic, void *payload, int payloadLen);
    int PublishBin(const char *topic, void *payload, int payloadLen, int qos);
    int PublishBin(const char *topic, const char *payload, int payloadLen, int qos, bool retain);
    int PublishBin(const char *id, const char *topic, const char *payload, int payloadLen);
    int PublishBin(const char *id, const char *topic, const char *payload, int payloadLen, int qos);
    int PublishBin(const char *id, const char *topic, const char *payload, int payloadLen, int qos, bool retain);
    int Subscribe(const char *topic);
    int Subscribe(const char *topic, int qos);
    void PrintSettings(void);
    //**************************************************************//

protected:

signals:

    //**************************************************************//
    void MessageReceived(MQTT::Message message);
    void OnConnected(void);
    void OnDisconnected(void);
    //**************************************************************//

private:

    //**************************************************************//
    Settings_t *_settings;
    //**************************************************************//

    //**************************************************************//
    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_message(const struct mosquitto_message *msg);
    //**************************************************************//

private slots:

};
//******************************************************************//

//******************************************************************//
void PrintMessage(const Message &message);
//******************************************************************//

}

//******************************************************************//
Q_DECLARE_METATYPE(MQTT::Message)
//******************************************************************//

#endif // MQTT_CLIENT_H
