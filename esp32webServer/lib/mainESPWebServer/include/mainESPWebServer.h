#include <Arduino.h>

#include <WebServer.h>

#include <ESPmDNS.h>
#include <Wire.h>
#include <WiFi.h>

#ifndef MAIN_ESP_WEB_SERVER
#define MAIN_ESP_WEB_SERVER

#include "page.h"



void handle_root();

void interface_feedback_handler();

void temp_sens();



// функции-интерфейс
WebServer& start_server(std::function<void(String, String, String, String)> fn);

void start_WIFI_in_client_mode(const char *ssid, const char *password, const char* nameDNS);

void start_WIFI_in_station_mode(const char *ssid,
    const char *password,
    IPAddress local_ip,
    IPAddress gateway,
    IPAddress subnet
);

void handleClient();

#endif