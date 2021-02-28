#ifndef LineMessage_ESP32
#define LineMessage_ESP32

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

class LineMessage
{
private:
    WiFiClientSecure client;
    String host;
    String LineTonken;
    String message;
public:
    LineMessage(String Host, String LineTonken);
    void sendMessage();
    void setMessage(String Message);
    ~LineMessage();
};



#endif