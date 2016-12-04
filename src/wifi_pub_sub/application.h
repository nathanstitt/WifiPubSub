#ifndef __WIFI_PUB_SUB_APPLICATION_H__
#define __WIFI_PUB_SUB_APPLICATION_H__

#include "wifi_pub_sub/configuration.h"
#include "wifi_pub_sub/wifi_config.h"
#include "wifi_pub_sub/mqtt_config.h"

namespace WifiPubSub {

    class Application {

    public:
        Application();
        void start();
        void loop();

        Configuration config;
        MqttConfig mqtt;
        WifiConfig wifi;
    };

}

#endif // __WIFI_PUB_SUB_APPLICATION_H__
