#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QVariant>
#include <QtDebug>
#include "mqtt_client.h"
#include "modbus_master.h"
#include "utils.h"

using namespace std;

//******************************************************************//
class cClient : public QObject {

    Q_OBJECT

public:

    //**************************************************************//
    cClient(int id, Modbus::cMaster *modbusMaster, MQTT::cClient *mqttClient);
    cClient(string id, Modbus::cMaster *modbusMaster, MQTT::cClient *mqttClient);
    cClient(const char *id, Modbus::cMaster *modbusMaster, MQTT::cClient *mqttClient);
    virtual ~cClient();
    void PrintID(void);
    //**************************************************************//

    //**************************************************************//
    //cClient(int id_, Modbus::cMaster *modbusMaster);
    //**************************************************************//

    //**************************************************************//

    //**************************************************************//

    //**************************************************************//
    void GetUmeas(void);
    //**************************************************************//

public slots:

    //**************************************************************//
    void MQTTReceive(MQTT::Message message);
    void ModbusReceive(Modbus::Response response);
    //**************************************************************//

protected:

private:

    //**************************************************************//

    //**************************************************************//

    //**************************************************************//
    int _modbusID;
    string _mqttID;
    Modbus::cMaster *_modbusMaster;
    MQTT::cClient *_mqttClent;
    float uMeas;
    //**************************************************************//

};
//******************************************************************//

#endif // CLIENT_H
