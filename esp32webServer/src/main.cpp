#include <Arduino.h>
#include <Wire.h>

// Для WiFi
#include <WiFi.h>
// Для веб-сервера
#include <WebServer.h>
// Смены имени в сети
#include <ESPmDNS.h>

// Including our html, css, js code from other <...>.h file
#include "page.h"

/* 
 * Макрос для подключение сервера в роли клента сети WiFi к маршрутизатору
 * и также изменением URL с цифрового IP на mDNS для подключения
 */
#define ESP_STATION_MODE

#ifdef ESP_CLIENT_MODE

/* SSID & Password */

// SSID существующей сети WiFI
const char* ssid = "HUAWEI P40 lite";     // Enter your exist WiFi ssid here
// Password существующей сети WiFI
const char* password = "Yan2006yany"; // Enter your exist WiFi password here
// DNS для переименования URL сервера в составе существующей сети WiFI в виде <DNS>.local
const char* nameDNS = "esp32-dashboard"; //

#endif

#ifdef ESP_STATION_MODE

// SSID & Password
const char *ssid = "ESP32_S3";     // Enter your SSID here
const char *password = "esp32_s3"; // Enter your Password here

// IP Address details
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

#endif

// Object of WebServer(HTTP port, 80 is defult)
WebServer server(80);

// Последние значение таймера
unsigned long time_p = 0;
// Значение температуры
int8_t temp = 0;

// Обновление данных о температуре


/**
 * \brief Функциия для фреймворка Ардуино, предназначенная преобразовавать промежуток из строки в целочисленное значение.
 *
 * \param [in] color - Название переменной типа строка, в которой нужно находить символы цифр
 * \param [in] start_ch - Символ, с которого начинатся поиск, предыдущий перед нужной цифрой
 * \param [in] end_ch - Символ, на который оканчивается интервал поиска, после интервала поиска цифр
 * \param [in] row_ch - Максимальное количество цифр, которое нужно преобразовать в тип Integer (макс. значение 255)
 * 
 * \return Значение типа uint8_t - это целое число в диапазоне от 0 до 255, записывается в заранее объявленную переменную такого же типа
 */



// Handle root url (/)
void handle_root()
{
    server.send(200, "text/html", HTML);
    // Время обновления веб-страницы
    delay(200);
}

// Отправка данных о температуре на сервер
void output_joystic_pos(){
    Serial.println(" 0speed " + server.arg("0speed")+\
     " 0angle " + server.arg("0angle")+\
     " 1speed " + server.arg("1speed")+\
     " 1angle " + server.arg("1angle"));
    server.send(200);
    //delay(150);
}


void temp_sens()
{
    const char* new_temp_data = String(temp).c_str();
    server.send(200, "text/plain", String(temp));
    server.sendContent(String(temp));
}

void setup()
{
    // Передача данных по Serial
    Serial.begin(115200);
    
#ifdef ESP_CLIENT_MODE
    // Подключение к существующей сети WiFI
    WiFi.begin(ssid, password);
    // Ручная конфигурация подключения по WiFi для изменения DNS
    MDNS.begin(nameDNS);
    
#endif

#ifdef ESP_STATION_MODE
    // Подключение wifi в режиме Station
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    // Вывод IP микроконтроллера в сети
    Serial.println("Create wifi station: " + String(ssid));
#endif
    // Полный код веб-страницы
    server.on("/", handle_root);
    server.on("/joystic_pos",output_joystic_pos);
    // Регистрация событий (event)

    // Запуск сервера
    server.begin();

#ifdef ESP_STATION_MODE
    Serial.println("Server was started");
    Serial.println(local_ip);
#endif


}

void loop()
{
    // Принимать действия пользователя
    server.handleClient();

    //update_temp_data();
}