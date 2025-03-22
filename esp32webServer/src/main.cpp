#include <Arduino.h>
//#include "page.h"
#include <mainESPwebServer.h>

/* 
 * Макрос для подключение сервера в роли клента сети WiFi к маршрутизатору
 * и также изменением URL с цифрового IP на mDNS для подключения
 */
#define ESP_STATION_MODE

void enterface_handler_example(String speed0, String angle0, String speed1, String angle1){
    Serial.println(" 0speed " + speed0+\
        " 0angle " + angle0+\
        " 1speed " + speed1+\
        " 1angle " + angle1);
}





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

    // принимает объект std::function<void(String,String,String,String)>
    // (server.arg("0speed"),server.arg("0angle"),server.arg("1speed"),server.arg("1angle"));

    /*  
        функция будет вызываться при взаимодействии с джойстиками:
        степень отдаления джойстика от центра 0 - 100
        угол отклонения джойстика - для 0angle от 0 до 360, для 1angle - 180 или 360 
        
        Функция вызывается при изменения положения джойстика: если джойстик находится не в центре 
        и не двигается - функция не вызывается.
        Если джойстик отпустили и он вернулся в центр - функция вызовется с аргументом speed = 0
    */


    WebServer& server = start_server(enterface_handler_example);

}

void loop()
{
    server.handleClient();
}