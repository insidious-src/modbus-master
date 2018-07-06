#include "modbus_master.h"

namespace Modbus {

//******************************************************************//
cMaster::cMaster()
    : _device(NULL),
      _tcp(false),
      _settings(NULL) {                                                                 //Default constructor
    this->_device = new QModbusRtuSerialMaster(this);                                   //New Modbus RTU master
    this->_settings = new Settings_t();                                                 //Generate default settings
    connect(this->_device, &QModbusClient::stateChanged,
            this, &cMaster::OnStateChanged);                                            //Connect
}
//******************************************************************//

//******************************************************************//
cMaster::cMaster(bool tcp)
    : _device(NULL),
      _tcp(tcp),
      _settings(NULL) {                                                                 //Constructor
    if(!tcp) {
        this->_device = new QModbusRtuSerialMaster(this);                               //New Modbus RTU master
    }
    else {
        this->_device = new QModbusTcpClient(this);                                     //New Modbus TCP client
    }
    this->_settings = new Settings_t();                                                 //Generate default settings
    connect(this->_device, &QModbusClient::stateChanged,
            this, &cMaster::OnStateChanged);                                            //Connect
}
//******************************************************************//

//******************************************************************//
cMaster::cMaster(bool tcp, Settings_t &settings)
    : _device(NULL),
      _tcp(tcp),
      _settings(NULL) {                                                                 //Constructor
    if(!tcp) {
        this->_device = new QModbusRtuSerialMaster(this);                               //New Modbus RTU master
    }
    else {
        this->_device = new QModbusTcpClient(this);                                     //New Modbus TCP client
    }
    this->_settings = new Settings_t();                                                 //Generate default settings
    this->LoadSettings(settings);                                                       //Load settings
    connect(this->_device, &QModbusClient::stateChanged,
            this, &cMaster::OnStateChanged);                                            //Connect
}
//******************************************************************//

//******************************************************************//
cMaster::cMaster(bool tcp, const char *settingsFileName, const char *fieldName)
    : _device(NULL),
      _tcp(tcp),
      _settings(NULL) {                                                                 //Constructor
    if(!tcp) {
        this->_device = new QModbusRtuSerialMaster(this);                               //New Modbus RTU master
    }
    else {
        this->_device = new QModbusTcpClient(this);                                     //New Modbus TCP client
    }
    this->_settings = new Settings_t();                                                 //Generate default settings
    LoadSettingsFromFile(this->_settings, settingsFileName, fieldName, tcp);            //Load the settings from file
    connect(this->_device, &QModbusClient::stateChanged,
            this, &cMaster::OnStateChanged);                                            //Connect
}
//******************************************************************//

//******************************************************************//
cMaster::~cMaster() {
    if(this->_device) {                                                                 //If exsisting
        this->_device->disconnectDevice();                                              //Disconnect
    }
    delete this->_device;                                                               //Free
    delete this->_settings;                                                             //Free
}
//******************************************************************//

//******************************************************************//
void cMaster::LoadSettings(const Settings_t &settings) {
    memcpy((void *)this->_settings, (void *)&settings, sizeof(settings));               //Copy the new settings
    this->_settings->portNameURL = settings.portNameURL;                                //Deep copy
}
//******************************************************************//

//******************************************************************//
bool cMaster::Connect(void) {
    if(this->_device) {
        if(!this->_tcp) {                                                               //If serial
            this->_device->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                                  this->_settings->portNameURL.c_str());//Port name/URL
            this->_device->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                                  this->_settings->baudRate);           //Baud rate
            this->_device->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                                  this->_settings->dataBits);           //Data bits
            this->_device->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                                  this->_settings->stopBits);           //Stop bits
            this->_device->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                                  this->_settings->parity);             //Parity
        }
        else {                                                                          //If TCP
            QUrl url = QUrl::fromUserInput(this->_settings->portNameURL.c_str());       //URL
            this->_device->setConnectionParameter(QModbusDevice::NetworkPortParameter,
                                                  url.port());                          //Port
            this->_device->setConnectionParameter(QModbusDevice::NetworkAddressParameter,
                                                  url.host());                          //Host
        }
        this->_device->setTimeout(this->_settings->timeout);                            //Timeout
        this->_device->setNumberOfRetries(this->_settings->retries);                    //Number of retries
        return (this->_device->connectDevice());                                        //Connect
    }
    return (false);
}
//******************************************************************//

//******************************************************************//
void cMaster::Disconnect(void) {
    this->_device->disconnectDevice();                                                  //Disconnect
}
//******************************************************************//

//******************************************************************//
bool cMaster::IsConnected(void) {
    if(this->_device->state() == QModbusDevice::ConnectedState) {
        return (true);                                                                  //Connected
    }
    return (false);                                                                     //Not connected
}
//******************************************************************//

//******************************************************************//
QModbusDevice::State cMaster::GetState(void) {
    return (this->_device->state());                                                    //State
}
//******************************************************************//

//******************************************************************//
bool cMaster::ReadInputRegisters(int id, int addr, int count) {
    if(!this->_device) {                                                                //If not existing
        return (false);                                                                 //Error
    }
    QModbusDataUnit dataUnit = QModbusDataUnit(QModbusDataUnit::InputRegisters,
                                               addr,
                                               count);                                  //Data unit
    if(auto *reply = this->_device->sendReadRequest(dataUnit, id))  {                   //Send read request
        if(!reply->isFinished()) {                                                      //If not finished (not broadcast)
            connect(reply,
                    &QModbusReply::finished,
                    this,
                    &Modbus::cMaster::ReadWriteReady);                                  //Connect
        }
        else {
            delete reply;                                                               //Free
        }
    }
    else {
        return (false);                                                                 //Error
    }
    return (true);                                                                      //Success
}
//******************************************************************//

//******************************************************************//
bool cMaster::WriteHoldingRegisters(int id, int addr, int *data, int count) {
    if(!this->_device) {                                                                //If not exsisting
        return (false);                                                                 //Error
    }
    QModbusDataUnit dataUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters,
                                               addr,
                                               count);                                  //Data unit
    for(int i = 0; i < count; ++ i) {                                                   //For each element
        dataUnit.setValue(i, *data ++);                                                 //Set value
    }
    if(auto *reply = this->_device->sendWriteRequest(dataUnit, id))  {                  //Send write request
        if(!reply->isFinished()) {                                                      //If not finished (not broadcast)
            connect(reply,
                    &QModbusReply::finished,
                    this,
                    &Modbus::cMaster::ReadWriteReady);                                  //Connect
        }
        else {
            delete reply;                                                               //Free
        }
    }
    else {
        return (false);                                                                 //Error
    }
    return (true);                                                                      //Success
}
//******************************************************************//

//******************************************************************//
void cMaster::ReadWriteReady(void) {
    auto reply = qobject_cast<QModbusReply *>(sender());                                //Get the reply
    QModbusDataUnit dataUnit = reply->result();                                         //Get the result
    Response response(reply->serverAddress(), dataUnit, reply->error());                //Make response structure
    //PrintResponse(response);
    emit DataReady(response);                                                           //Emit the response
    reply->deleteLater();                                                               //Free
}
//******************************************************************//

//******************************************************************//
void cMaster::PrintSettings(void) {
    Modbus::PrintSettings(*this->_settings, this->_tcp);                                //Print the settings
}
//******************************************************************//

//******************************************************************//
void cMaster::OnStateChanged(void) {
    QModbusDevice::State state = this->_device->state();                                //Get state
    QByteArray s = this->_settings->portNameURL.c_str();
    qInfo() << endl;
    switch(state) {
    case QModbusDevice::UnconnectedState:
        emit OnDisconnected();                                                          //Emit signal
        qInfo() << "Modbus Master: unconnected" << endl;
        break;
    case QModbusDevice::ConnectingState:
        emit OnDisconnected();                                                          //Emit signal
        qInfo() << "Modbus Master: connecting to" << s << endl;
        break;
    case QModbusDevice::ConnectedState:
        emit OnConnected();                                                             //Emit signal
        qInfo() << "Modbus Master: connected to" << s << endl;
        break;
    case QModbusDevice::ClosingState:
        emit OnDisconnected();                                                          //Emit signal
        qInfo() << "Modbus Master: closing" << endl;
        break;
    default:
        emit OnDisconnected();                                                          //Emit signal
        qInfo() << "Modbus Master: unknown" << endl;
    }
}
//******************************************************************//

//******************************************************************//
void PrintResponse(Response &response) {
    cout << endl;
    cout << "Modbus response: " << endl;
    cout << "\tID: " << response.id << endl;
    if(response.errorCode != QModbusDevice::NoError) {
        cout << "\tError code: " << response.errorCode << endl;
        return;
    }
    cout << "\tRegister type: " << response.dataUnit.registerType() <<  endl;
    cout << "\tData: " << endl;
    for(uint i = 0; i < response.dataUnit.valueCount(); ++ i) {
        if((response.dataUnit.registerType() == QModbusDataUnit::InputRegisters) || \
           (response.dataUnit.registerType() == QModbusDataUnit::HoldingRegisters)) {
            cout << "\t\t" << \
            (response.dataUnit.startAddress() + i) << ": " << \
            response.dataUnit.value(i) << endl;
        }
    }
}
//******************************************************************//

}
