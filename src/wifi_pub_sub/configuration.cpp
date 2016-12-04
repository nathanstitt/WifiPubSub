#include "wifi_pub_sub/configuration.h"

#include <EEPROM.h>

#include<Arduino.h>

#define EEPROM_SIZE 512

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}

String
read_eeprom_string(int addr, int maxLen = EEPROM_SIZE){
    String ret;
    int pos = 0;
    char ch = EEPROM.read(addr + pos);

    while ( (ch != 0x00) && ((addr + pos) <= EEPROM_SIZE) && (pos < maxLen) ) {
        ret.concat(ch);
        pos++;
        ch = EEPROM.read(addr + pos);
    }
    return ret;
}

bool
write_eeprom_string(const String &str, int addr){
    auto len = str.length();
    if (addr + len > EEPROM_SIZE){
        return false;
    }
    Serial.print("Write: '");
    Serial.print(addr);
    Serial.print("' : ");
    Serial.println(str);
    for (int i = 0; i < len; i++) {
        Serial.print("Write: ");
        Serial.print(addr + i);
        Serial.print(" : ");
        Serial.println(str.charAt(i));
        EEPROM.write(addr + i, str.charAt(i));
    }
    EEPROM.write(addr + len, '\0');
}

void
read_multiple_strings( String **strings, int count, int startAt ){
    for (int i = 0; i < count; i++ ){
        String *str = strings[i];
        str->remove(0);
        str->concat( read_eeprom_string(startAt) );
        startAt += ( str->length() + 1 );
    }
}

void
write_multiple_strings( String **strings, int count, int startAt ){
    for (int i = 0; i < count; i++ ){
        String *str = strings[i];
        Serial.print("Writing string: ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(*str);
        write_eeprom_string(*str, startAt);
        startAt += ( str->length() + 1 );
    }
}


WifiPubSub::Configuration::Configuration() {
    EEPROM.begin( EEPROM_SIZE );
    client_id = "ESP8266Client-";
    client_id += String(random(0xffff), HEX);

    auto v = read_eeprom_string(0);
    if (v != version){
        Serial.println("Aborting configuration read, version mismatch");
        Serial.print(v);
        Serial.print(" == ");
        Serial.println(version);
        write_eeprom_string(version, 0);
        save();
    } else {
        String *strs[4] = { &server, &user, &password, &client_id };
        EEPROM_readAnything(5, port);
        read_multiple_strings(strs, 4, 10);
    }

    if (!auto_configured_ssid.length()){
        auto_configured_ssid = "ESP ";
        auto_configured_ssid += ESP.getChipId();
    }

}

void
WifiPubSub::Configuration::save(){
    Serial.println("Saving config");
    EEPROM_writeAnything(5, port);
    String *strs[4] = { &server, &user, &password, &client_id };
    write_multiple_strings(strs, 4, 10);
    EEPROM.commit();
}

bool
WifiPubSub::Configuration::isConfigured(){
    return (port && server.length());
}
