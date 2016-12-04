# WifiPubSub

A library that combines [PubSubClient](http://pubsubclient.knolleary.net), [WifiManager](https://github.com/tzapu/WiFiManager), and [OTA updates](http://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html).

Written for and primarily used with ESP8266, but should work with any Arduino that WifiManager and MQTT supports.

It adds configuration parameters to WifiManager for setting the MQTT parameters and then automatically connects to the MQTT server when the wifi connects.

A callback function can be associated with a MQTT topic and will be called whenever a matching message arrives.

## Example


```cpp

#include "wifi_pub_sub.h"

WifiPubSub::Application *app;

void
on_test_message(const byte* payload, unsigned int length){
    Serial.println("Received test payload, doing something special");
}

void setup() {
    Serial.begin(115200);
    Serial.println("");
    Serial.println("System Starting Up....");

    app = new WifiPubSub::Application();

    app->mqtt.subscribe("test", on_test_message);
    app->start();

    Serial.println("Done Starting Up....");

}

void loop() {
    app->loop();
}


```
