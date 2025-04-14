#include <Arduino.h>
//#include "page.h"
#include <mainESPWebServer.h>

/* 
 * Макрос для подключение сервера в роли клента сети WiFi к маршрутизатору
 * и также изменением URL с цифрового IP на mDNS для подключения
 */
#define ESP_STATION_MODE

void enterface_handler_example(int speed0, int angle0, int speed1, int angle1){
    Serial.print(" 0speed ");
    Serial.println(speed0);
    Serial.print(" 0angle ");
    Serial.println(angle0);
    Serial.print(" 1speed ");
    Serial.println(speed1);
    Serial.print(" 1angle ");
    Serial.println(angle1);
    Serial.println("---------------");
}
class MovementData{
    public:
    bool omni_mode;
    int8_t sector;
    int8_t speed;

    void commands_passer(int speed0, int angle0, int speed1, int angle1){
        // Serial.println("RECEIVED JOYSTIC POS");
        omni_mode = speed0 > 0;
        
        // Serial.println("omni mode checked");
        if (omni_mode) {
            // Serial.println("omni mode case");
            sector = int((angle0 + 22.5) / 45) % 8;
            speed = speed0;
            }
        else {
            // Serial.println("not ommi mode case");
            sector = angle1/180 - 1;
            speed = speed1; 
        }
        
        #ifdef SERIAL_DEBUG
        Serial.print("omni mode:");
        Serial.println(omni_mode);

        Serial.print("speed: ");
        Serial.println(speed);
        Serial.print("sector: ");
        Serial.println(sector);
        Serial.println("-----------------------");
        #endif
    }

    void full_stop(){
        speed = 0;
        /* тут нужно вызывать команды для двигателя */
    }
};


#ifdef ESP_CLIENT_MODE

const char* ssid = "HUAWEI P40 lite";     

const char* password = "Yan2006yany"; 

const char* nameDNS = "esp32-dashboard"; 

#endif


#ifdef ESP_STATION_MODE

const char *ssid = "ESP32_S3";     
const char *password = "esp32_s3"; 
MovementData movement;


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

    // принимает объект std::function<void(int,int,int,int)>
    // (server.arg("0speed"),server.arg("0angle"),server.arg("1speed"),server.arg("1angle"));

    /*  
        функция будет вызываться при взаимодействии с джойстиками:
        степень отдаления джойстика от центра 0 - 100
        угол отклонения джойстика - для 0angle от 0 до 360, для 1angle - 180 или 360 
        
        Функция вызывается при изменения положения джойстика: если джойстик находится не в центре 
        и не двигается - функция не вызывается.
        Если джойстик отпустили и он вернулся в центр - функция вызовется с аргументом speed = 0
    */
    #ifdef SERIAL_DEBUG
    Serial.println("Starting Server");
    #endif
    WebServer& server = start_server([movementPtr = &movement](int s0,int a0, int s1,int a1){movementPtr -> commands_passer(s0, a0, s1, a1);},
        [](){Serial.println("wifi disconected");});

}
 
void loop()
{
    handleClient();
}