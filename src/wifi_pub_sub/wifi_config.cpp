#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include "wifi_pub_sub/configuration.h"
#include "wifi_pub_sub/wifi_config.h"


static WifiPubSub::WifiConfig *WIFI;

void
onConfiguredCallback(){
    Serial.println("Wifi Configured");

    auto config = *(WIFI->_config);

    config.server   = WIFI->server->getValue();
    config.port     = String( WIFI->port->getValue() ).toInt();
    config.user     = WIFI->user->getValue();
    config.password = WIFI->password->getValue();
    config.ota_password = WIFI->ota_pass->getValue();
    config.save();
}


WifiPubSub::WifiConfig::WifiConfig(Configuration *config) :
    _config(config), start_in_configuration_mode(false)
{ }

void
WifiPubSub::WifiConfig::connect() {

    WiFiManager wifiManager;

    Serial.println("Starting wifi config");

    server   = new WiFiManagerParameter("server",   "mqtt server",       _config->server.c_str(), 40);
    wifiManager.addParameter(server);
    port     = new WiFiManagerParameter("port",     "mqtt port", String( _config->port).c_str(), 5);
    wifiManager.addParameter(port);
    user     = new WiFiManagerParameter("user",     "mqtt user",         _config->user.c_str(), 40);
    wifiManager.addParameter(user);
    password = new WiFiManagerParameter("password", "mqtt password",     _config->password.c_str(), 40);
    wifiManager.addParameter(password);

    ota_pass = new WiFiManagerParameter("password", "OTA update password",     _config->ota_password.c_str(), 40);
    wifiManager.addParameter(ota_pass);

    WIFI = this;

    wifiManager.setSaveConfigCallback(onConfiguredCallback);

    Serial.print("is configured: ");
    Serial.println( _config->isConfigured() );

    if ( !start_in_configuration_mode && _config->isConfigured() ){
        wifiManager.autoConnect();
    } else {
        wifiManager.startConfigPortal( _config->auto_configured_ssid.c_str(),
                                       _config->auto_configured_password.length() ? _config->auto_configured_password.c_str() : NULL
                                       );
    }

    configureOTA();
}

void
WifiPubSub::WifiConfig::configureOTA(){
    ArduinoOTA.setPort(8266);

    if (WIFI->_config->ota_password.length()){
        ArduinoOTA.setPassword(WIFI->_config->ota_password.c_str());
    }

    ArduinoOTA.onStart([]() {
            Serial.println("Starting OTA update");
        });

    ArduinoOTA.onEnd([]() {
            Serial.println("\nEnd");
        });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });

    ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });

    ArduinoOTA.begin();

}

void
WifiPubSub::WifiConfig::loop(){
    ArduinoOTA.handle();
}
