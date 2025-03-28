#include "CameraWebServer.h"

bool serial_fps_output;


static esp_err_t stream_handler(httpd_req_t *req) {
    float fps = 0.0;
    camera_fb_t *fb = NULL;
    struct timeval _timestamp;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t *_jpg_buf = NULL;
    char *part_buf[128];

    
    static int64_t fr_start = 0;
    int64_t frame_time;
    int64_t fr_end;
    

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if (res != ESP_OK) {
    return res;
    }

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");// весь контент страницы доступен для fetch'а
    httpd_resp_set_hdr(req, "X-Framerate", "60"); // фпс страницы - 60

    while (true) {
    fb = esp_camera_fb_get(); // получение фото
    if (!fb) {
        log_e("Camera capture failed"); 
        res = ESP_FAIL;
    } 
    else {
        _timestamp.tv_sec = fb->timestamp.tv_sec;
        _timestamp.tv_usec = fb->timestamp.tv_usec;
        if (fb->format != PIXFORMAT_JPEG) {
            Serial.println("WRONG CAMERA PIXFORMAT");
            bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
            esp_camera_fb_return(fb);
            fb = NULL;
            if (!jpeg_converted) {
            log_e("JPEG compression failed");
            res = ESP_FAIL;
            }
        } else {
            _jpg_buf_len = fb->len;
            _jpg_buf = fb->buf;
        }

    }
    //отправка картинки по чанкам
    if (res == ESP_OK) {
        res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if (res == ESP_OK) {
        size_t hlen = snprintf((char *)part_buf, 128, _STREAM_PART, _jpg_buf_len, _timestamp.tv_sec, _timestamp.tv_usec);
        res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) {
        res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (fb) {
        esp_camera_fb_return(fb);
        fb = NULL;
        _jpg_buf = NULL;
    } else if (_jpg_buf) {
        free(_jpg_buf);
        _jpg_buf = NULL;
    }
    if (res != ESP_OK) {
        log_e("Send frame failed");
        break;
    }
    // если задан вывод фпс - вычисляем и проверяем фпс
    if(serial_fps_output){
        fr_end = esp_timer_get_time();

        frame_time = fr_end - fr_start;
        fr_start = fr_end;
        fps = 1000000.0/frame_time;

        if(fps<25){
            
            Serial.print("low fps: ");
            Serial.print(fps);
            Serial.print(" temp: ");
            Serial.println(temperatureRead());    
        }
    }
    }

    return res;
    }



void startCameraServer() 
    {
        // задаем дефолтную конфигурацию сервера
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
        //config.stack_size = 8192;
        config.server_port = 80;

        // привязываем хендлеры к uri
        httpd_uri_t stream_uri = {
            .uri = "/stream",
            .method = HTTP_GET,
            .handler = stream_handler,
            .user_ctx = NULL
        };

        // запускаем сервер с заданной конфигурацией, если запустился - инициализируем связку uri - handler 
        if (httpd_start(&stream_httpd, &config) == ESP_OK) {
            httpd_register_uri_handler(stream_httpd, &stream_uri);
        }
    }

void camera_init(framesize_t framesize,  unsigned char jpeg_quality){

    if(jpeg_quality > 63){
        Serial.println("jpeg quality given to  camera_init has to be between 0 and 63");
    }

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector



    // передаем пины в конфигурацию
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
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;

    // выставляем разрещение
    config.frame_size = framesize;

    // формат, место и способ хранения картинки
    config.pixel_format = PIXFORMAT_JPEG;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_count = 2;

    // выставляем разрешение изображения
    config.jpeg_quality = jpeg_quality;


    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
    }
}

void start_stream(char* ssid, const char* password,IPAddress local_IP, bool debug_mode){
    
    
    serial_fps_output = debug_mode;
    

    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 0, 0);


    // Wi-Fi connection
    
    WiFi.config(local_IP,gateway,subnet);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    startCameraServer();

    Serial.print("Camera Stream Ready! Go to: http://");
    Serial.print(WiFi.localIP());
    Serial.print("/stream");

}