#include <mqtt_client.h>

namespace MQTT {

//******************************************************************//
cClient::cClient(const char *id)
    : mosquittopp(id),
      _settings(NULL) {                                                                 //Default constructor
    mosqpp::lib_init();                                                                 //Initialize the library
    this->_settings = new Settings_t();                                                 //Generate default settings
}
//******************************************************************//

//******************************************************************//
cClient::cClient(const char *id, Settings_t &settings)
    : mosquittopp(id),
      _settings(NULL) {                                                                 //Constructor
    mosqpp::lib_init();                                                                 //Initialize the library
    this->_settings = new Settings_t();                                                 //Generate default settings
    this->LoadSettings(settings);                                                       //Load settings
}
//******************************************************************//

//******************************************************************//
cClient::cClient(const char *id, const char *settingsFileName, const char *fieldName)
    : mosquittopp(id),
      _settings(NULL) {                                                                 //Constructor
    mosqpp::lib_init();                                                                 //Initialize the library
    this->_settings = new Settings_t();                                                 //Generate default settings
    LoadSettingsFromFile(this->_settings, settingsFileName, fieldName);                 //Load the settings from file
}
//******************************************************************//

//******************************************************************//
cClient::~cClient() {
    loop_stop();                                                                        //Stop the network loop
    mosqpp::lib_cleanup();                                                              //Cleanup the library
    delete this->_settings;                                                             //Free
}
//******************************************************************//

//******************************************************************//
void cClient::LoadSettings(const Settings_t &settings) {
    memcpy((void *)this->_settings, (void *)&settings, sizeof(settings));               //Copy the new settings
    this->_settings->host = settings.host;                                              //Deep copy
    this->_settings->username = settings.username;                                      //Deep copy
    this->_settings->password = settings.password;                                      //Deep copy
}
//******************************************************************//

//******************************************************************//
int cClient::Connect(void) {
    mosqpp::mosquittopp::reconnect_delay_set(this->_settings->reconnectDelay,
                                             INT_MAX,
                                             false);                                    //Set reconnect delay
                                                                                        //Set username and password
    int ret = mosqpp::mosquittopp::username_pw_set(this->_settings->username.c_str(),
                                                   this->_settings->password.c_str());
    if(ret != MOSQ_ERR_SUCCESS) {                                                       //If error
        return (ret);                                                                   //Error
    }
    ret = mosqpp::mosquittopp::connect_async(this->_settings->host.c_str(),
                                             this->_settings->port,
                                             this->_settings->keepAlive);               //Connect
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::Disconnect(void) {
    int ret = mosqpp::mosquittopp::disconnect();                                        //Disconnect
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::ReConnect(void) {
    int ret = mosqpp::mosquittopp::reconnect_async();                                   //Reconnect
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::Start(void) {
    int ret = loop_start();                                                             //Start the network loop
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::Stop(void) {
    int ret = loop_stop();                                                              //Stop the network loop
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::UnSubscribe(const char *topic) {
    int ret = mosqpp::mosquittopp::unsubscribe(NULL, topic);                            //Unsubscribe
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishText(const char *topic, const char *payload) {
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           topic,
                                           strlen(payload),
                                           payload,
                                           this->_settings->qosPub,
                                           this->_settings->retain);                    //Publish with preset QoS and retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishText(const char *topic, const char *payload, int qos) {
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           topic,
                                           strlen(payload),
                                           payload,
                                           qos,
                                           this->_settings->retain);                    //Publish with requested QoS and preset retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishText(const char *topic, const char *payload, int qos, bool retain) {
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           topic,
                                           strlen(payload),
                                           payload,
                                           qos,
                                           retain);                                     //Publish with requested QoS and retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishText(const char *id, const char *topic, const char *payload) {
    string idTopic = id;
    idTopic += "/";
    idTopic += topic;                                                                   //Format: ID/topic
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           idTopic.c_str(),
                                           strlen(payload),
                                           payload,
                                           this->_settings->qosPub,
                                           this->_settings->retain);                    //Publish with preset QoS and retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishText(const char *id, const char *topic, const char *payload, int qos) {
    string idTopic = id;
    idTopic += "/";
    idTopic += topic;                                                                   //Format: ID/topic
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           idTopic.c_str(),
                                           strlen(payload),
                                           payload,
                                           qos,
                                           this->_settings->retain);                    //Publish with requested QoS and preset retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishText(const char *id, const char *topic, const char *payload, int qos, bool retain) {
    string idTopic = id;
    idTopic += "/";
    idTopic += topic;                                                                   //Format: ID/topic
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           idTopic.c_str(),
                                           strlen(payload),
                                           payload,
                                           qos,
                                           retain);                                     //Publish with requested QoS and retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishBin(const char *topic, void *payload, int payloadLen) {
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           topic,
                                           payloadLen,
                                           payload,
                                           this->_settings->qosPub,
                                           this->_settings->retain);                    //Publish with preset QoS and retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishBin(const char *topic, void *payload, int payloadLen, int qos) {
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           topic,
                                           payloadLen,
                                           payload,
                                           qos,
                                           this->_settings->retain);                    //Publish with requested QoS and preset retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishBin(const char *topic, const char *payload, int payloadLen, int qos, bool retain) {
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           topic,
                                           payloadLen,
                                           payload,
                                           qos,
                                           retain);                                     //Publish with requested QoS and retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishBin(const char *id, const char *topic, const char *payload, int payloadLen) {
    string idTopic = id;
    idTopic += "/";
    idTopic += topic;                                                                   //Format: ID/topic
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           idTopic.c_str(),
                                           payloadLen,
                                           payload,
                                           this->_settings->qosPub,
                                           this->_settings->retain);                    //Publish with preset QoS and retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishBin(const char *id, const char *topic, const char *payload, int payloadLen, int qos) {
    string idTopic = id;
    idTopic += "/";
    idTopic += topic;                                                                   //Format: ID/topic
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           idTopic.c_str(),
                                           payloadLen,
                                           payload,
                                           qos,
                                           this->_settings->retain);                    //Publish with requested QoS and preset retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::PublishBin(const char *id, const char *topic, const char *payload, int payloadLen, int qos, bool retain) {
    string idTopic = id;
    idTopic += "/";
    idTopic += topic;                                                                   //Format: ID/topic
    int ret = mosqpp::mosquittopp::publish(NULL,
                                           idTopic.c_str(),
                                           payloadLen,
                                           payload,
                                           qos,
                                           retain);                                     //Publish with requested QoS and retain
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::Subscribe(const char *topic) {
    int ret = mosqpp::mosquittopp::subscribe(NULL, topic, this->_settings->qosSub);     //Subscribe with preset QoS
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
int cClient::Subscribe(const char *topic, int qos) {
    int ret = mosqpp::mosquittopp::subscribe(NULL, topic, qos);                         //Subscribe with requested QoS
    return (ret);                                                                       //Result
}
//******************************************************************//

//******************************************************************//
void cClient::PrintSettings(void) {
    MQTT::PrintSettings(*this->_settings);
}
//******************************************************************//

//******************************************************************//
void cClient::on_connect(int rc) {
    QByteArray s = this->_settings->host.c_str();
    if(rc == 0) {
        emit OnConnected();                                                             //Emit signal
        qInfo() << "MQTT Client: connected to host" << \
        s << endl;
    }
    else if(rc == 1) {
        emit OnDisconnected();                                                          //Emit signal
        qInfo() << "MQTT Client: connection to host" << \
        s << "refused. Reason: unacceptable protocol version" << endl;
    }
    else if(rc == 2) {
        emit OnDisconnected();                                                          //Emit signal
        qInfo() << "MQTT Client: connection to host" << \
        s << "refused. Reason: identifier rejected" << endl;
    }
    else if(rc == 3) {
        emit OnDisconnected();                                                          //Emit signal
        qInfo() << "MQTT Client: connection to host" << \
        s << "refused. Reason: broker unavailable" << endl;
    }
    else {
        emit OnDisconnected();                                                          //Emit signal
        qInfo() << "MQTT Client: connection to host" << \
        s << "refused. Reason: unknown" << endl;
    }
}
//******************************************************************//

//******************************************************************//
void cClient::on_disconnect(int rc) {
    emit OnDisconnected();                                                              //Emit signal
    if(rc == 0) {
        qInfo() << "MQTT Client: disconnected. Reason: client" << endl;
    }
    else {
        qInfo() << "MQTT Client: disconnected. Reason: unknown" << endl;
    }
}
//******************************************************************//

//******************************************************************//
void cClient::on_message(const struct mosquitto_message *msg) {
    qInfo() << "MQTT Client: message received" << endl;
    Message message = Message(msg->topic, msg->payload, msg->payloadlen);               //Create message
    emit MessageReceived(message);                                                      //Emit signal
}
//******************************************************************//

//******************************************************************//
void PrintMessage(const Message &message) {
    cout << endl;
    cout << "MQTT Message:" << endl;
    cout << "\tTopic: " << message.topic << endl;
    cout << "\tPayload: " << message.payload << endl;
}
//******************************************************************//

}
