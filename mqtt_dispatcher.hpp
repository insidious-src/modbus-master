#ifndef MQTT_DISPATCHER_HPP
#define MQTT_DISPATCHER_HPP

#include <QObject>
#include <QVariant>
#include "mqtt_client.h"
#include <iostream>

using namespace std;

namespace MQTT {

//******************************************************************//
class cDispatcher : public QObject {

Q_OBJECT

public:

    //**************************************************************//
    cDispatcher();
    ~cDispatcher();
    //**************************************************************//

public slots:

    /***************************************************************/
    void Dispatch(MQTT::Message data);
    /***************************************************************/

protected:

signals:

private:

};
//******************************************************************//

}

#endif // MQTT_DISPATCHER_HPP
