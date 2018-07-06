#include <QCoreApplication>
//#include "comm.hpp"
#include <QTimer>
//#include <QKeyEvent>
#include <iostream>
#include <QObject>
#include "mqtt_client.h"
#include <mqtt_dispatcher.hpp>
#include "mqtt_settings.h"
#include "client.h"
#include "webdownload.h"


#include "modbus_settings.h"
#include "modbus_master.h"

#include <QFile>
#include <QThread>
#include <iostream>
#include <functional>

#include "main.h"

#include <QThreadPool>

float floatizeMe (unsigned int myNumba)
{
    //// myNumba comes in as 32 bits or 8 byte

    unsigned int  sign = myNumba >>31;
    signed int  exponent = ((myNumba >> 23) & 0xff) - 0x7F;
    unsigned int  mantissa = myNumba << 9;
    float  value = 0;
    float mantissa2;

    cout << endl << "input is : " << hex << myNumba << endl;
    cout << endl << "mantissa is : " << hex << mantissa << endl;

    value = 0.5f;
    mantissa2 = 0.0f;
    while (mantissa)
    {
        if (mantissa & 0x80000000)
            mantissa2 += value;
        mantissa <<= 1;
        value *= 0.5f;
    }

    cout << "\nsign is: " << sign << endl;
    cout << "exponent is : " << hex << exponent << endl;
    cout << "mantissa 2 is : " << mantissa2 << endl;

    /* REMOVE:
       if above this number it is negative
    if ( sign  == 1)
        sign = -1;

    // if above this number it is positive
    else {
        sign = 1;
    } */

    /* value = sign * (1.0f + mantissa2) * (pow (2, exponent)); */
    value = (1.0f + mantissa2) * (pow (2, exponent));
    if (sign) value = -value;
    cout << dec << "Float value is: " << value << "\n\n\n";

    return value;
}

union Floatize
{
   unsigned short word[2];
   float          value  ;

   constexpr Floatize (unsigned short word1, unsigned short word2)
   : word { word1, word2 }
   { }
};

//******************************************************************//



//******************************************************************//

//******************************************************************//
int main(int argc, char *argv[])
{
    qRegisterMetaType<MQTT::Message>();
    qRegisterMetaType<Modbus::Response>();
    QCoreApplication a(argc, argv);


    MQTT::cClient mqttClient ("this", "/home/home/Projects/Qt/modbus_master/settings.json", "mqtt");
    mqttClient.PrintSettings();

    Modbus::cMaster modbusRTUMaster (false, "/home/home/Projects/Qt/modbus_master/settings.json", "modbus rtu");
    modbusRTUMaster.PrintSettings();


    mqttClient.Connect();
    mqttClient.Start();
    mqttClient.Subscribe("+/+");

    QObject::connect(&modbusRTUMaster, &Modbus::cMaster::DataReady, [](Modbus::Response resp)
    {
        if(resp.dataUnit.startAddress() == 12)
            std::cout << "Act. Power: " << Floatize(resp.dataUnit.value(1), resp.dataUnit.value(0)).value << " W\n";
    });

    modbusRTUMaster.Connect();

    std::function<void()> scheduler;

    scheduler = [&]
    {
        auto time = QTime::currentTime();

        for (auto h = time.hour(); h < 24; ++h)
        {
            for (auto m = time.minute(); m < 60; ++m)
            {
                for (auto s = time.second(); s < 60; s += 5)
                {
                    time.setHMS(h, m, s);

                    Modbus::runAt(time, [&]
                    {
                        modbusRTUMaster.ReadInputRegisters(1, 12, 2);
                        if (QTime::currentTime() > QTime(23, 59, 55)) scheduler();
                    });
                }
            }
        }
    };

    scheduler();

    cClient *cli = new cClient("1", &modbusRTUMaster, &mqttClient);
    cli->PrintID();
    cli->GetUmeas();

    //QObject::connect(mqttClient, &MQTT::cClient::MessageReceived, cli, &cClient::MQTTReceive);

    //QTimer t;
    //note that we need to use t.connect, as main is not a QObject
    //t.connect (&t, SIGNAL(timeout()), &a, SLOT(quit()));
    //t.start(7000);

    return a.exec();
}
