#include "mainESPWebServer.h"

WebServer server(80);
std::function<void(int, int, int, int)> interface_handler_insides;
std::function<void()> disconnect_handler_insides;

/* по какой-то причине не могу создать класс с приватными методами чтобы он нормально обрабатывал переменную типа WebServer */

float received_fps = 0.0;
unsigned long time_p = 0;
int8_t temp = 0;
float result = 0;
WiFiClient wifiClient;

/*  обработка главной страницы: обновление html-страницы каждые N мс (выставил 500)
 (не влияет на кол-во фпс) */
void handle_root()
{
    server.send(200, "text/html", HTML);
    
    delay(5000);
}

// int mfinint;

/*  обработчик входящей на uri /joystic_pos информации, вывод информации о положении джойстиков */
void interface_feedback_handler(){
    //mfinint = server.arg("0speed").toInt();

    // Serial.print("incoming info ");
    // Serial.println(server.arg("0speed").toInt());
    
    interface_handler_insides(server.arg("0speed").toInt(),server.arg("0angle").toInt(),server.arg("1speed").toInt(),server.arg("1angle").toInt());
    server.send(200);
}

/*  обработчик uri /temp_sens, отправляет температуру процессора каждый раз, когда приходит запрос */
void temp_sens(){   
        server.send(200, "text/plain", String(temperatureRead()));
        server.sendContent(String(temperatureRead()));
}


/* выставляет все хендлеры и запускает сервер, возвращает ссылку на него, тк в loop
 нужно положить server.handleClient()*/

WebServer& start_server(std::function<void(int, int, int, int)> interface_handler, std::function<void()> disconect_handler)
{   
    wifiClient = WiFiClient();
    if (!interface_handler){
        Serial.println("[ERROR] you have to give corrrect std::function<void(int, int, int, itn)> function");
        //throw(ESP_ERR_INVALID_ARG);
    }
    interface_handler_insides = interface_handler;
    disconnect_handler_insides = disconect_handler;
    server.on("/", handle_root);
    server.on("/joystic_pos",interface_feedback_handler);
    server.on("/temperature", temp_sens);

    
    server.begin();
    return server;

}

/* подключается к заданной сети вайфай */
void start_WIFI_in_client_mode(const char *ssid, const char *password, const char* nameDNS)
{
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

/* создает точку вай-фай */
void start_WIFI_in_station_mode(const char *ssid,
                                const char *password,
                                IPAddress local_ip = IPAddress(192, 168, 1, 1),
                                IPAddress gateway= IPAddress(192, 168, 1, 1),
                                IPAddress subnet= IPAddress(255, 255, 255, 0)){
    WiFi.setSleep(false);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    Serial.println("Create wifi station: " + String(ssid));
    
}



void  handleClient(){
    server.handleClient();
    // if ( (wifiClient.connected()) && (WiFi.status() == WL_CONNECTED) ){
    //     disconnect_handler_insides();
    // }
}
