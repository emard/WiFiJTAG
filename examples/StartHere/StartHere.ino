#include <ESP8266WiFi.h>
#include <FS.h>
#include <WiFiClient.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <Hash.h>
#include <LibXSVF.h>
#include "WiFiJTAG.h"

LibXSVF jtag = LibXSVF();

void setup() {
    // WiFi is started inside library
    SPIFFS.begin(); // Not really needed, checked inside library and started if needed
    ESPHTTPServer.begin(&SPIFFS);
    /* add setup code here */
}

void loop() {
    /* add main program code here */
    static int16_t last;
    int16_t difftime, current = millis();
    difftime = current-last;
    if(difftime > 10000)
    {
      last = current;
      jtag.scan();
      jtag.program("/bitstream.svf", 0);
    } 

    // DO NOT REMOVE. Attend OTA update from Arduino IDE
    ESPHTTPServer.handle();	
}
