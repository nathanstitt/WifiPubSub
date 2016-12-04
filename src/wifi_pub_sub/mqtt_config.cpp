#include "wifi_pub_sub/mqtt_config.h"
#include "wifi_pub_sub/configuration.h"
#include <functional>

WifiPubSub::MqttConfig::Callback::Callback(const String &t, callback_t c) :
    topic(t), callback(c), next(NULL)
{

}

WifiPubSub::MqttConfig::MqttConfig(Configuration *config) :
    _config(config), client(_wclient), _callback_list(NULL)
{


}

void
WifiPubSub::MqttConfig::subscribe(const String &topic, callback_t cb, int qos){
    Callback *callback = new Callback(topic, cb);

    if (!_callback_list){
        _callback_list = callback;
    } else {
        auto pos = _callback_list;
        while (pos->next){ pos = pos->next; }
        pos->next = callback;
    }
    if (client.connected()){
        client.subscribe( callback->topic.c_str(), 1);
    }
}

void
WifiPubSub::MqttConfig::reconnect() {
    Serial.println("Reconnecting to MQTT server");
    auto attempts = 0;

    while (!client.connected() && attempts < 10) {
        connect();
        attempts++;
        delay(attempts * 1000);
        Serial.print("Attempt: ");
        Serial.print(attempts);
        Serial.print(" State: ");
        Serial.println(client.state());
    }
}

void
WifiPubSub::MqttConfig::loop() {
    if (!client.connected()) {
        reconnect();
    } else {
        client.loop();
    }
}

void
WifiPubSub::MqttConfig::connect() {
    Serial.println("Connecting to MQTT server");
    client.setServer( _config->server.c_str(), _config->port );
    client.setCallback( std::bind(&MqttConfig::_on_message, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

    Serial.print( "Authenticating with client id: ");
    Serial.print( _config->client_id );
    Serial.print( " login: " );
    Serial.print( _config->user );
    Serial.print( " password: " );
    Serial.println( _config->password );


    if ( client.connect( _config->client_id.c_str(), _config->user.c_str(), _config->password.c_str() ) ){
        if ( _callback_list ){
            Serial.print("Subscribing to ");
            auto callback = _callback_list;
            while (callback){
                Serial.println(callback->topic);
                client.subscribe(callback->topic.c_str());
                callback = callback->next;
            }
        } else {
            Serial.println("No callbacks added");
        }
    } else {
        Serial.print("Could not connect to MQTT server: ");
        Serial.print(_config->server.c_str());
        Serial.print(":");
        Serial.println(_config->port);
    }
}

void
WifiPubSub::MqttConfig::_on_message(const char topic[], byte* payload, unsigned int length) {
    Serial.print("Recv'd message for topic: ");
    Serial.println(topic);
    auto callback = _callback_list;
    while (callback){
        if (callback->topic.equals(topic)){
            callback->callback(payload, length);
            return;
        }
    }
}
