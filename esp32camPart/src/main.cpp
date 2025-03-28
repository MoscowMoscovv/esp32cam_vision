//#include "Arduino.h" // уже импортируется в CameraWebServer.h

#include "CameraWebServer.h"


/***
[22.03.2025] КОД ВЫГРУЖАЕТСЯ ЧЕРЕЗ ПЛАТФОРМИО

непосредственно у меня все еще не работает serial monitor для esp32 cam в VS ни через
отдельный модуль serial monitor, ни через монитор от platformio

через arduino IDE все работает коректно
***/


char* ssid = "ESP32_S3";
const char* password = "esp32_s3";



void setup() {
  //esp_log_level_set("*",ESP_LOG_ERROR);
  Serial.begin(115200);

  // 2й параметр - качество изображения jpeg, 0 <= КАЧЕСТВО <= 63
  // меньше число - больше качество
  // рекомендуемое значение - ~20+ при ~10 будет stack overflow на httpd

  Serial.println("starting camera init");
  camera_init(FRAMESIZE_QVGA,27);

  IPAddress local_IP(192,168,1,184);// стрим будет по uri  http://<local_IP>/stream
  

  start_stream(ssid, password, local_IP,true);
}

 

void loop() {

}