#include <Arduino.h>

#include <WebServer.h>

#include <ESPmDNS.h>
#include <Wire.h>
#include <WiFi.h>

#ifndef MAIN_ESP_WEB_SERVER
#define MAIN_ESP_WEB_SERVER

#define SERIAL_DEBUG

#include "page.h"


/**
 * \brief Обработка главной страницы: отправляет html-разметку главной страницы 
 * \return None
 **/
void handle_root();

/** 
 * \brief обработчик входящей на uri /joystic_pos информация, вызов передаваемой в StartServer функции
 *  \return None
*/
void interface_feedback_handler();

/**
 *  \brief  обработчик uri /temp_sens, отправляет температуру процессора каждый раз, когда приходит запрос 
 *  \return None 
 **/
void temp_sens();



// функции-интерфейс

/**
 * \brief Функция запуска веб-сервера
 * \param [in] interface_handler функция обработки входящих с веб-интерфейса значений
 *      \param 0speed расстояния левого джойстика от центра
 *      \param 0angle угол отклонения левого джойстика от центра
 *      \param 1speed расстояния правого джойстика от центра
 *      \param 1angle угол отклонения правого джойстика от центра
 *      \return None
 * \param disconect_handler обработчик отключения вай-фая
 * \return WebServer
 **/
WebServer& start_server(std::function<void(int, int, int, int)> interface_handler, std::function<void()> disconect_handler);

/**
 * \brief Функция подключения к доступной точке WiFi
 * \param [in] ssid имя сети
 * \param [in] bts7960_data пароль от сети
 * \param [in] nameDNS DNS
 * \return None
 */
void start_WIFI_in_client_mode(const char *ssid, const char *password, const char* nameDNS);

/**
 * \brief Функция включения Esp в режим WiFi хаба
 * \param [in] ssid имя сети
 * \param [in] password пароль от сети
 * \param [in] local_ip ip платы в сети
 * \param [in] gateway шлюз
 * \param [in] subnet подсеть
 * \return None
 **/
void start_WIFI_in_station_mode(const char *ssid,
    const char *password,
    IPAddress local_ip,
    IPAddress gateway,
    IPAddress subnet
);

/**
* \brief Функция вызывается в цикле loop()
* \return None
**/
void handleClient();

#endif