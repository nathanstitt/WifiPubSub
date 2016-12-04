#ifndef __WIFI_PUB_SUB_WIFI_CONFIG_H__
#define __WIFI_PUB_SUB_WIFI_CONFIG_H__

#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <WString.h>

class WiFiManagerParameter;

namespace WifiPubSub {

    class Configuration;

    class WifiConfig {
    public:
        bool start_in_configuration_mode;
        WifiConfig(Configuration *config);
        void connect();
        void loop();

        WiFiManagerParameter *server   = NULL;
        WiFiManagerParameter *port     = NULL;
        WiFiManagerParameter *user     = NULL;
        WiFiManagerParameter *password = NULL;
        WiFiManagerParameter *ota_pass = NULL;

        Configuration *_config;

    private:
        void configureOTA();
    };

}


#endif // __WIFI_PUB_SUB_WIFI_CONFIG_H__
