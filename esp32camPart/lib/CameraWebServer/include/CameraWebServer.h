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

static esp_err_t stream_handler(httpd_req_t *req);

void startCameraServer();

void camera_init(framesize_t framesize,  unsigned char jpeg_quality);

void start_stream(char* ssid, const char* password,IPAddress local_IP, bool debug_mode);


#endif