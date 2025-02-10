#include <WiFi.h>
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h" 
#include "soc/rtc_cntl_reg.h"  
#include "esp_http_server.h"



char* ssid = "HUAWEI-1045ST";
const char* password = "Tyan2006yany";

const char* html_content = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Camera</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; }
    .button {
      padding: 20px;
      font-size: 20px;
      margin: 10px;
      cursor: pointer;
    }
    #stream { margin-top: 20px; }
  </style>
</head>
<body>
  <h1>ESP32 Camera Control</h1>
  <button class="button" id="up" onmousedown="sendCommand('up')" onmouseup="sendCommand('stop')">Up</button><br>
  <button class="button" id="left" onmousedown="sendCommand('left')" onmouseup="sendCommand('stop')">Left</button>
  <button class="button" id="right" onmousedown="sendCommand('right')" onmouseup="sendCommand('stop')">Right</button><br>
  <button class="button" id="down" onmousedown="sendCommand('down')" onmouseup="sendCommand('stop')">Down</button><br>
  <div id="stream">
    <img src"[INSERT ESP CAMERA IP HERE]/stream" alt="Camera Stream">
  </div>
  <script src="joy.js"></script>
  <div id="joyDiv" style="width:200px;height:200px;margin-bottom:20px;"></div>
  <script type="text/javascript">
    // Create JoyStick object into the DIV 'joyDiv'
    var joy = new JoyStick('joyDiv');
    setInterval(function(){ x.value=joy.GetX(); }, 50);
    </script>
  <script>
    function sendCommand(cmd) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/control?cmd=" + cmd, true);
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";

static esp_err_t index_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, html_content, strlen(html_content));
}





static esp_err_t cmd_handler(httpd_req_t *req) {
    char* buf;
    size_t buf_len;
    char variable[32] = {0};
  
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
      buf = (char*)malloc(buf_len);
      if (!buf) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
      }
      if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
        if (httpd_query_key_value(buf, "cmd", variable, sizeof(variable)) == ESP_OK) {
          Serial.println(variable); // Выводим команду в Serial
        }
      }
      free(buf);
    }
  
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
  }



  void startCameraServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
  
    httpd_uri_t index_uri = {
      .uri = "/",
      .method = HTTP_GET,
      .handler = index_handler,
      .user_ctx = NULL
    };
  
    httpd_uri_t cmd_uri = {
      .uri = "/control",
      .method = HTTP_GET,
      .handler = cmd_handler,
      .user_ctx = NULL
    };
  
    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
      httpd_register_uri_handler(stream_httpd, &index_uri);
      httpd_register_uri_handler(stream_httpd, &cmd_uri);
    }
  }
  

  void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
   
    Serial.begin(115200);
    Serial.setDebugOutput(false);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");


    Serial.print("Control part stream Ready! Go to: http://");
    Serial.print(WiFi.localIP());
    
    // Start streaming web server
    startCameraServer();
  }
  

void loop(){
}