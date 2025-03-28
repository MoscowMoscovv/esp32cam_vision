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

#ifndef CAMERA_WEB_SERVER
#define CAMERA_WEB_SERVER

#define PART_BOUNDARY "123456789000000000000987654321"

const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
const char* _STREAM_BOUNDARY     = "\r\n--" PART_BOUNDARY "\r\n";
const char* _STREAM_PART         = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";

httpd_handle_t stream_httpd = NULL;



// инициализирует пины (прописано отдельно чтобы при смене модели камеры
// можно было достать макросы из exaple'а)

#define PWDN_GPIO_NUM  32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  0
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27

#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    21
#define Y4_GPIO_NUM    19
#define Y3_GPIO_NUM    18
#define Y2_GPIO_NUM    5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22


static esp_err_t stream_handler(httpd_req_t *req);

void startCameraServer();

void camera_init(framesize_t framesize,  unsigned char jpeg_quality);

void start_stream(char* ssid, const char* password,IPAddress local_IP, bool debug_mode);


#endif