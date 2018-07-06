#include "client.h"

//******************************************************************//
cClient::cClient(int id, Modbus::cMaster *modbusMaster, MQTT::cClient *mqttClient)
    : _modbusID(id),
      _mqttID(""),
      _modbusMaster(modbusMaster),
      _mqttClent(mqttClient) {                                                          //Default constructor
    QVariant aux(id);                                                                   //Auxiliary
    this->_mqttID = aux.toString().toStdString();                                       //Convert to string
}
//******************************************************************//

//******************************************************************//
cClient::cClient(string id, Modbus::cMaster *modbusMaster, MQTT::cClient *mqttClient)
    : _modbusID(-1),
      _mqttID(id),
      _modbusMaster(modbusMaster),
      _mqttClent(mqttClient) {                                                          //Constructor
    QVariant aux(QString::fromStdString(id));                                           //Auxiliary
    bool ok = false;                                                                    //Conversion result
    int tmp = aux.toInt(&ok);                                                           //Convert to int
    if(ok) {                                                                            //If valid integer representation
        this->_modbusID = tmp;                                                          //Assign
    }
}
//******************************************************************//

//******************************************************************//
cClient::cClient(const char *id, Modbus::cMaster *modbusMaster, MQTT::cClient *mqttClient)
    : _modbusID(-1),
      _mqttID(id),
      _modbusMaster(modbusMaster),
      _mqttClent(mqttClient) {
    QVariant aux(QString::fromStdString(id));                                           //Auxiliary
    bool ok = false;                                                                    //Conversion result
    int tmp = aux.toInt(&ok);                                                           //Convert to int
    if(ok) {                                                                            //If valid integer representation
        this->_modbusID = tmp;                                                          //Assign
    }
}
//******************************************************************//

//******************************************************************//
cClient::~cClient() {                                                                   //Destructor

}
//******************************************************************//

//******************************************************************//
void cClient::PrintID(void) {
    cout << endl;
    cout << "Client ID:" << endl;
    cout << "\tModbus: " << this->_modbusID << endl;
    cout << "\tMQTT: " << this->_mqttID << endl;
}
//******************************************************************//

//******************************************************************//

void cClient::GetUmeas(void) {
    connect(this->_modbusMaster, &Modbus::cMaster::DataReady, this, &cClient::ModbusReceive);
    this->_modbusMaster->ReadInputRegisters(this->_modbusID, 2, 2);

    //modbusRTUMaster->ReadInputRegisters(1, 0, 6);
}

//******************************************************************//
void cClient::MQTTReceive(MQTT::Message message) {

    string topicCpy = message.topic;                                                    //Copy the topic
    string id = strtok((char *)topicCpy.c_str(), "/");                                  //Get the ID
    string topic = strtok(NULL, "\0");                                                  //Get the topic
    if(id != this->_mqttID) {                                                           //If the ID does not match
        return;                                                                         //Exit
    }

    QVariant payload;
    float aux;
    bool ok = false;
    if(topic == "u_meas") {
        //cout << message.payload << endl;

        //message.payload[message.payloadLen] = '\0';
        payload.setValue(QString::fromStdString(message.payload));
        aux = payload.toFloat(&ok);
        //cout << aux << endl;
        if(ok) {
            this->uMeas = aux;
            cout << this->uMeas << endl;
        }
    }
    //string ids = id;
    //cout << id << endl;
    //string topics = topic;
    //cout << topic << endl;
    //string id = msg.topic.substr(0, msg.topic.find("/")); // token is "scott"
    //string to = msg.topic.substr(1, msg.topic.find("/")); // token is "scott"
    //cout << id << endl;
    //cout << to << endl;
    //strtok(msg.topic.c_str(), "/");
    //cout << id;
    //string line = msg.topic;
    //string word;

    //istringstream iss(line, istringstream::in);

    //while( iss >> word )
    //{
    //    cout << word;
        // Do something on `word` here...
    //}
    //MQTT::PrintMessage(msg);
}
//******************************************************************//

void cClient::ModbusReceive(Modbus::Response response) {
    if(response.errorCode) {
        disconnect(this->_modbusMaster, &Modbus::cMaster::DataReady, this, &cClient::ModbusReceive);
        return;
    }
    if(response.id != this->_modbusID) {
        disconnect(this->_modbusMaster, &Modbus::cMaster::DataReady, this, &cClient::ModbusReceive);
        return;
    }
    //Modbus::PrintResponse(response);
    if(response.errorCode == 0) {

        QVector<uint16_t> reg = response.dataUnit.values();
        if(response.dataUnit.startAddress() == 0x0002) {
            this->uMeas = Utils::Int16RegTo32bitData<float>(&reg[0], &reg[1], Utils::_32bitDataByteOrder_t::noSwap);
            cout << this->uMeas << endl;
        }
        //uint32_t test = Utils::Int16RegTo32bitData<uint32_t>(&reg[0], &reg[1], Utils::_32bitDataByteOrder_t::noSwap);
        //cout << hex << test << endl;
    }
    disconnect(this->_modbusMaster, &Modbus::cMaster::DataReady, this, &cClient::ModbusReceive);
}

