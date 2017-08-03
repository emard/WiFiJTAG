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
    static bool to_program = false;
    static int16_t last;
    int16_t difftime, current = millis();
    difftime = current-last;
    if(difftime > 5000 && to_program)
    {
      jtag.program("/bitstream.svf", 0);
      Serial.println("DONE");
      last = millis();
      to_program = false;
      ESPHTTPServer.jtag_program = false;
    }
    if(ESPHTTPServer.jtag_scan)
    {
      jtag.scan();
      Serial.println("DONE");
      ESPHTTPServer.jtag_scan = false;
    }
    if(to_program == false && ESPHTTPServer.jtag_program)
    {
      to_program = true; 
      // this is to schedule programing after few seconds
      // so web interface can finish accessing all its files.
      // don't click to jtag's web interface during programming!
      // web server and jtag programmer cannot access
      // filesystem at the same time
      last = millis();
    }

    // DO NOT REMOVE. Attend OTA update from Arduino IDE
    ESPHTTPServer.handle();	
}
