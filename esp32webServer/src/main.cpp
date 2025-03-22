#include <Arduino.h>
//#include "page.h"
#include <mainESPwebServer.h>

/* 
 * Макрос для подключение сервера в роли клента сети WiFi к маршрутизатору
 * и также изменением URL с цифрового IP на mDNS для подключения
 */
#define ESP_STATION_MODE


#ifdef ESP_CLIENT_MODE

const char* ssid = "HUAWEI P40 lite";     

const char* password = "Yan2006yany"; 

const char* nameDNS = "esp32-dashboard"; 

#endif


#ifdef ESP_STATION_MODE

const char *ssid = "ESP32_S3";     
const char *password = "esp32_s3"; 


IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

#endif


void setup()
{

    Serial.begin(115200);
    
    #ifdef ESP_CLIENT_MODE
        
        start_WIFI_in_client_mode(ssid,password,nameDNS);
        
    #endif

    #ifdef ESP_STATION_MODE

        start_WIFI_in_station_mode(ssid,password,local_ip,gateway,subnet);
        
    #endif

WebServer& server = start_server();

}

void loop()
{
    server.handleClient();
}