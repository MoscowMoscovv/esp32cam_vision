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

void start_stream(char* ssid, const char* password,IPAddress local_IP, bool debug_mode);

void camera_init(framesize_t framesize,  unsigned char jpeg_quality);