#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h" 
#include "soc/rtc_cntl_reg.h"
#include "esp_http_server.h"
#include <string>

#include "config.h"

#ifndef CAMERA_WEB_SERVER
#define CAMERA_WEB_SERVER


static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY     = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART         = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";

static httpd_handle_t stream_httpd = NULL;


/**
 * \brief Обработка uri /stream
 * \param [in] req входящий запрос с клиентской части
 * \return код ошибки ESP
 **/
static esp_err_t stream_handler(httpd_req_t *req);

/**
 * \brief запуск сервера со стандартными параметрами, линковка функций-обработчиков к адресам
 * \return None
 **/
void startCameraServer();

/**
 * \brief Функция настройки камеры
 * \param [in] framesize разрешение 
 * \param [in] jpeg_quality степень сжатия jpeg: 0 <= jpeg_quality <= 63
 * меньше число - больше качество, рекомендуемое значение - ~20+
 * \return None
 */
void camera_init(framesize_t framesize,  unsigned char jpeg_quality);

/**
 * \brief Функция включения Esp в режим WiFi хаба
 * \param [in] ssid имя сети
 * \param [in] password пароль от сети
 * \param [in] local_ip ip платы в сети
 * \param [in] debug_mode при true выводит fps и температуру в Serial Monitor при просадках (инициализация не происходит в файле библиотеки, необходимо инициализировать Serial в вызывающем файле)
 * \return None
 **/
void start_stream(char* ssid, const char* password,IPAddress local_IP, bool debug_mode);


#endif