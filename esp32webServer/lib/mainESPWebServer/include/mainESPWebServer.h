#include <Arduino.h>

#include <WebServer.h>

#include <ESPmDNS.h>
#include <Wire.h>
#include <WiFi.h>

#include "page.h"


WebServer& start_server(std::function<void(String, String, String, String)> fn);

void start_WIFI_in_client_mode(const char *ssid, const char *password, const char* nameDNS);

void handleClient();


void start_WIFI_in_station_mode(const char *ssid,
    const char *password,
    IPAddress local_ip, //= IPAddress(192, 168, 1, 1),
    IPAddress gateway,  //= IPAddress(192, 168, 1, 1),
    IPAddress subnet //= IPAddress(255, 255, 255, 0)
);

