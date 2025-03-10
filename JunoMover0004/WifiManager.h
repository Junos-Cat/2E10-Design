#pragma once
#include <Arduino.h>
#include "variables.h"

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFiS3.h>

class WifiManager {
  public:
    // Constructor: SSID, şifre ve port numarasını alır.
    WifiManager(const char* ssid, const char* password, uint16_t port);
    
    // setup() içinde bir kez çağrılacak: WiFi bağlantısını kurar ve sunucuyu başlatır.
    void wifiSetup();
    
    // loop() içinde sürekli çağrılarak, gelen HTTP isteklerini kontrol eder ve komutları işler.
    void processCommands();
    
  private:
    const char* _ssid;
    const char* _password;
    uint16_t _port;
    WiFiServer _server;
};

#endif
