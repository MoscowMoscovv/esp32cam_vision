#include "esp_camera.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "your_SSID";         // Заменить на имя сети
const char* password = "your_PASSWORD"; // Заменить на пароль

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

AsyncWebServer server(80);

void setupCamera() {
    camera_config_t config;

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;


    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
}

// создание видеопотока
String generateMJPEG() {
    camera_fb_t* fb = NULL;

    // Получаем кадр с камеры
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return String();
    }

    String response = "--frame\r\n";
    response += "Content-Type: image/jpeg\r\n";
    response += "Content-Length: " + String(fb->len) + "\r\n\r\n";

    // Преобразуем кадр в строку
    String data = "";
    for (size_t i = 0; i < fb->len; i++) {
        data += String((char)fb->buf[i]);
    }

    response += data + "\r\n";

    esp_camera_fb_return(fb);

    return response;
}

// Обработчик для MJPEG потока
void handleJPGStream(AsyncWebServerRequest* request) {
    request->send_P(200, "multipart/x-mixed-replace; boundary=frame", generateMJPEG().c_str());
}

// Обработчики для кнопок
void handleUp(AsyncWebServerRequest* request) {
    Serial.println("Up button pressed");
    request->send(200, "text/plain", "Up");
}

void handleDown(AsyncWebServerRequest* request) {
    Serial.println("Down button pressed");
    request->send(200, "text/plain", "Down");
}

void handleLeft(AsyncWebServerRequest* request) {
    Serial.println("Left button pressed");
    request->send(200, "text/plain", "Left");
}

void handleRight(AsyncWebServerRequest* request) {
    Serial.println("Right button pressed");
    request->send(200, "text/plain", "Right");
}

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");


    setupCamera();

    // Обработчики для кнопок
    server.on("/up", HTTP_GET, handleUp);
    server.on("/down", HTTP_GET, handleDown);
    server.on("/left", HTTP_GET, handleLeft);
    server.on("/right", HTTP_GET, handleRight);

    // Обработчик для стрима
    server.on("/stream", HTTP_GET, handleJPGStream);

    // HTML страница с кнопками
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        String html = "<html>\
                    <body>\
                      <h1>ESP32 CAM Control</h1>\
                      <img src='/stream' width='640' height='480' />\
                      <br>\
                      <button onclick=\"location.href='/up'\">Up</button>\
                      <button onclick=\"location.href='/down'\">Down</button>\
                      <button onclick=\"location.href='/left'\">Left</button>\
                      <button onclick=\"location.href='/right'\">Right</button>\
                    </body>\
                   </html>";
        request->send(200, "text/html", html);
        });

    server.begin();
}

void loop() {
    // Сервер уже работает, поэтому цикл пустой
}
