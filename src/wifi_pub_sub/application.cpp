#include "wifi_pub_sub/application.h"



WifiPubSub::Application::Application() :
    wifi(&config), mqtt(&config)
{



}

void
WifiPubSub::Application::start() {
    wifi.connect();
    mqtt.connect();
}

void
WifiPubSub::Application::loop() {
    wifi.loop();
    mqtt.loop();
}
