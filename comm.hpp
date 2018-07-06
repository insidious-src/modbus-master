#ifndef COMM_HPP
#define COMM_HPP

#include <QVariant>
#include <QSerialPort>
#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <iostream>



#include "modbus_settings.h"

using namespace std;

//******************************************************************//
namespace Comm {

//******************************************************************//
class cComm : public QObject {

public:

    //**************************************************************//
    cComm() {
        this->_modbusRTUMaster = nullptr;
        this->_modbusRTUMaster = new QModbusRtuSerialMaster(this);

        Modbus::Settings_t modbusSettings = Modbus::Settings_t();
        if(this->_modbusRTUMaster) {
            this->ModbusRTULoadSettings(modbusSettings);
            connect(this->_modbusRTUMaster, &QModbusClient::errorOccurred, this, &cComm::ModbusRTUError);
            connect(this->_modbusRTUMaster, &QModbusClient::stateChanged, this, &cComm::ModbusRTUStateChanged);
        }

        //this->du = new QModbusDataUnit(QModbusDataUnit::InputRegisters, 0x0000, 6);


 //       mosquitto_lib_init();

    }
    //**************************************************************//

    //**************************************************************//
    virtual ~cComm() {
        if(this->_modbusRTUMaster) {
            this->_modbusRTUMaster->disconnectDevice();
        }
        delete this->_modbusRTUMaster;
        this->_modbusRTUMaster = nullptr;


  //      mosquitto_disconnect (mosq);
  //      mosquitto_destroy (mosq);
  //      mosquitto_lib_cleanup();
    }
    //**************************************************************//

    //**************************************************************//
    void ModbusRTULoadSettings(Modbus::Settings_t &settings) {
        this->_modbusRTUMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, settings.portName.c_str());
        this->_modbusRTUMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, settings.baudRate);
        this->_modbusRTUMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, settings.dataBits);
        this->_modbusRTUMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, settings.stopBits);
        this->_modbusRTUMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, settings.parity);
        this->_modbusRTUMaster->setTimeout(settings.timeout);
        this->_modbusRTUMaster->setNumberOfRetries(settings.retries);
    }
    //**************************************************************//

    //**************************************************************//
    bool ModbusRTUMasterConnect(void) {
        if(this->_modbusRTUMaster) {
            return (this->_modbusRTUMaster->connectDevice());
        }
        return (false);
    }
    //**************************************************************//

    //**************************************************************//
    void ModbusRTUMasterDisconnect(void) {
        this->_modbusRTUMaster->disconnectDevice();
    }
    //**************************************************************//

    //**************************************************************//
    void Send(void) {
        if(!this->_modbusRTUMaster) {
            return;
        }
        QModbusDataUnit du = QModbusDataUnit(QModbusDataUnit::InputRegisters, 0x0000, 6);
        QModbusReply *reply = nullptr;
        reply = this->_modbusRTUMaster->sendReadRequest(du, 1);
        //_modbusRTUMaster->sendRawRequest()
        if(reply) {
            if(!reply->isFinished()) {
                //cout << "123"<<endl;
                connect(reply, &QModbusReply::finished, this, &cComm::readReady);

            }
            else {
                delete reply;
                cout << "456"<<endl;
            }
        }
        else {
            cout << "567"<<endl;
        }
        //delete reply;
        //QModbusReply *reply =
        //this->_modbusRTUMaster->sendReadRequest(this->du, 1);
    }
    //**************************************************************//

protected:

protected slots:

private:

    //**************************************************************//
    QModbusClient *_modbusRTUMaster;
    //struct mosquitto *mosq = NULL;
    //**************************************************************//

private slots:

    //**************************************************************//
    void ModbusRTUError(QModbusDevice::Error error) {
        //cout << this->_modbusRTUMaster->errorString().toStdString() << endl;

        cout << "hello" << endl;
    }
    //**************************************************************//

    //**************************************************************//
    void ModbusRTUStateChanged(QModbusDevice::State state) {
        cout << "hello1" << endl;
        //cout << this->_modbusRTUMaster->errorString().toStdString() << endl;
    }
    //**************************************************************//

    //**************************************************************//
    void readReady() {
        auto reply = qobject_cast<QModbusReply *>(sender());

        cout << reply->errorString().toStdString() << endl;
        cout << reply->serverAddress() << endl;
const QModbusDataUnit unit = reply->result();

        for (uint i = 0; i < unit.valueCount(); i++) {
            cout << unit.value(i) << endl;
        }

        cout << endl << endl;



        //cout << Int16Reg2Float(unit.value(2), unit.value(3)) << endl;
        //cout << Int16Reg2Float(unit.value(4), unit.value(5)) << endl;

        reply->deleteLater();
        //cout << du->HoldingRegisters.
        //cout << this->_modbusRTUMaster->errorString().toStdString() << endl;
    }
    //**************************************************************//



};
//******************************************************************//

}
//******************************************************************//

#endif // COMM_HPP
