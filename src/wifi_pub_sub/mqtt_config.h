#ifndef __WIFI_PUB_SUB_MQTT_CONFIG_H__
#define __WIFI_PUB_SUB_MQTT_CONFIG_H__

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <functional>

namespace WifiPubSub {
    class Configuration;

    class MqttConfig {

    public:
        typedef std::function<void(const byte* payload, unsigned int length)> callback_t;
        callback_t callback;

        MqttConfig(Configuration *config);

        void loop();
        void connect();
        void reconnect();
        void subscribe(const String &topic, callback_t callback, int qos = 0);
        PubSubClient client;
    private:

        Configuration *_config;
        WiFiClient _wclient;

        struct Callback {
            Callback(const String &topic, callback_t callback);
            String topic;
            callback_t callback;
            struct Callback *next;
        };
        Callback *_callback_list;

        void _on_message(const char topic[], byte* payload, unsigned int length);
    };

}


#endif // __WIFI_PUB_SUB_MQTT_CONFIG_H__
