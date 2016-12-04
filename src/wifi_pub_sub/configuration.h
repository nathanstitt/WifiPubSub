#ifndef __WIFI_PUB_SUB_CONFIGURATION_H__
#define __WIFI_PUB_SUB_CONFIGURATION_H__

#include <EEPROM.h>
#include <WString.h>

namespace WifiPubSub {

    struct Configuration {

        Configuration();
        void save();
        bool isConfigured();

        String version = "WFPS";
        String auto_configured_ssid;
        String auto_configured_password;
        String ota_password;

        int port = 0;
        String server;
        String user;
        String password;
        String client_id;
    };

}


#endif // __WIFI_PUB_SUB_CONFIGURATION_H__
