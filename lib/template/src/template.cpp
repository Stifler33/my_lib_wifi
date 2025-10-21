#include "template.h"
#include "html_content.h"

bool flag_led_status = false;
bool true_position_reset_button = LOW;

Preferences members;
WebServer server(80);
void handleRoot();
void handleSaveConfig();
void handle_root_mqtt();
void handle_save_settings_mqtt();
void save_settings_mqtt(String address, String user, String password, uint16_t port, String client_id);
uint8_t pin_reset;

void led_on(){
  if (!flag_led_status){
    flag_led_status = true;
    digitalWrite(2, HIGH);
  }
}

void led_off(){
  if (flag_led_status){
    flag_led_status = false;
    digitalWrite(2, LOW);
  }
}

void switch_led_status(){
  if (flag_led_status){
    led_off();
  }else led_on();
}

bool is_press_btn_reset(){
  return digitalRead(pin_reset) == true_position_reset_button;  
}

// таймер для моргания светодиодом
GTimer<millis> tmr_blink(100, false, GTMode::Timeout);

GTimer<millis> tmr_reset(2000, false, GTMode::Timeout);

/**
 * Функция задает параметры по умолчанию если конфигурации нет в памяти
 */
void init_config_wifi(){
  members.begin(NAME_CONFIG_WIFI, true);
  if (members.isKey("ssid")){
    return;
  }
  members.end();
  members.begin(NAME_CONFIG_WIFI, false);
  members.putString("ssid", "");
  members.putString("password", "");
  members.putString("hostname", "");
  members.putInt("wifi_mode", WIFI_AP);  
  members.end();
  members.begin(NAME_CONFIG_WIFI, true);
}

void init_config_mqtt(){
  members.begin(NAME_CONFIG_MQTT, true);
  if (members.isKey("address")){
    members.end();
    return;
  }
  members.end();
  members.begin(NAME_CONFIG_MQTT, false);
  members.putString("address", "");
  members.putString("user", "");
  members.putString("password", "");
  members.putString("client_id", "");
  members.putInt("port", 1883);
  members.end();  
}

/*
 * Функция инициализирует все необходимые компоненты:
 *
 * подключение к WiFi, задает режим работы WiFi
 * 
 * пин для сброса конфигурации
 *
 * Данные для подключения она достает из памяти
 *  
 * функция не блокирующая и она ничего не ждет
*/
void init_stif(){
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  init_config_mqtt();
  init_config_wifi();    
  String ssid = members.getString("ssid", "");
  wifi_mode_t wifi_mode = wifi_mode_t(members.getInt("wifi_mode", 0));
  String hostname = members.getString("hostname", "");  
  String password = members.getString("password", "");
  members.end();

  if (hostname != ""){
    WiFi.setHostname(hostname.c_str());    
  }
  
  if (wifi_mode == WIFI_STA){
    WiFi.mode(wifi_mode);
    WiFi.begin(ssid, password);
    server.on("/", HTTP_GET, handle_root_mqtt);
    server.on("/save_mqtt_config", HTTP_POST, handle_save_settings_mqtt);
    server.begin();
  }else if (wifi_mode == WIFI_AP){
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32_APP");
    server.on("/", HTTP_GET, handleRoot);
    server.on("/save_config", HTTP_POST, handleSaveConfig);
    server.begin();
  }

}

/**
 * Сохраняет конфигурацию подключения к Wifi в EEPRROM
 */
void save_config_wifi(String ssid, String password, const char* hostname, wifi_mode_t wifi_mode){
  members.begin(NAME_CONFIG_WIFI, false);
  members.putString("ssid", ssid);
  members.putString("password", password);
  members.putString("hostname", hostname);
  members.putInt("wifi_mode", wifi_mode);
  members.end();
}

void blink_led(int timeout){
  if (!tmr_blink.running()){
    tmr_blink.setTime(timeout);
    tmr_blink.start();    
  }
  if (tmr_blink){
    switch_led_status();
  }  
}

/**
 * цикл для отслеживания нажатия на кнопку reset
 */
void reset_loop(){
  if (is_press_btn_reset() && !tmr_reset.running()){
    tmr_reset.start();    
  }
  if (!is_press_btn_reset()){
    tmr_reset.stop();
  }
  if (tmr_reset){
    if (is_press_btn_reset()){
      led_off();
      save_config_wifi("", "", "", WIFI_AP);
      save_settings_mqtt("", "", "", 1883, "");
      ESP.restart();
    }    
  }
}

/**
 * Цикл для отслеживания подключения к Wifi и OTA
 */
bool loop_status_wifi(){
  reset_loop();
  switch (WiFi.getMode())
  {
  case WIFI_STA:    
    if (WiFi.status() != WL_CONNECTED){
      blink_led(100); 
      return false;     
    }else{
      led_on();
      server.handleClient();
      ArduinoOTA.handle();
    }
    return true;
    break;
  case WIFI_AP:
    server.handleClient();
    blink_led(500);
    return false;
    break;
  default:
    return false;
    break;
  }
}

uint32_t last_ota_time = 0;
/**
 * Функция инициализирует arduino ota
 */
void arduino_ota_initial(){
  ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }
      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      blink_led(20);
      if (millis() - last_ota_time > 500) {
        Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
        last_ota_time = millis();
      }
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.begin();
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", HTML_CONTENT);
}

void handle_root_mqtt(){
  server.send(200, "text/html; charset=UTF-8", HTML_MQTT);
}

void handleSaveConfig() {
  String ssid = "";
  String password = "";
  String hostname = "";

  // Проверяем, что это POST-запрос и что есть аргументы (данные формы)
  if (server.method() == HTTP_POST) {
    // Проходим по всем аргументам (полям формы)
    for (int i = 0; i < server.args(); i++) {
      if (server.argName(i) == "ssid") {
        ssid = server.arg(i);
      } else if (server.argName(i) == "password") {
        password = server.arg(i);
      } else if (server.argName(i) == "hostname") {
        hostname = server.arg(i);
      }
    }
    
    save_config_wifi(ssid, password, hostname.c_str());

    // Ответ клиенту после сохранения
    String response = "<h1>Сохранение завершено!</h1><p>Устройство попытается подключиться к сети: **" + ssid + "** с именем хоста: **" + hostname + "**.</p><p>Отключитесь от сети ESP32_Config и проверьте подключение в мониторе порта.</p>";
    server.send(200, "text/html; charset=UTF-8", response);
    
    // В реальном проекте, здесь вы обычно делаете перезагрузку или переключаете режим WiFi
    delay(1000);
    ESP.restart();

  } else {
    // Если это не POST-запрос, перенаправляем на главную
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  }
}

/**
 * Функция устанавливает пин для сброса конфигурации подключения к Wifi
 * 
 * ее надо обязательно запускать в блоке setup
 * @param pin пин для сброса конфигурации
 * @param true_position_button передаем LOW или HIGH в зависимости от того какое положение считаем нажатым
 */
void set_pin_reset(uint8_t pin, bool true_position_button){
  pin_reset = pin;
  true_position_reset_button = true_position_button;
}

void handle_save_settings_mqtt(){
  String address;
  uint16_t port;
  String client_id;
  String user;
  String password;

  if (server.method() == HTTP_POST){
    for (int i=0; i < server.args(); i++){
      if (server.argName(i) == "mqtt_server"){
        address = server.arg(i);
      }
      if (server.argName(i) == "mqtt_port"){
        port = server.arg(i).toInt();
      }
      if (server.argName(i) == "mqtt_client_id"){
        client_id = server.arg(i);
      }
      if (server.argName(i) == "mqtt_user"){
        user = server.arg(i);
      }
      if (server.argName(i) == "mqtt_pass"){
        password = server.arg(i);
      }
    }
  }

  String response = String("<h1>Сохранение завершено!</h1>") + \
  "<p>address: " + address + "</p>" + \
  "<p>port: " + port + "</p>" + \
  "<p>user: " + user + "</p>"  + \
  "<p>password: " + password + "</p>" + \
  "<p>client_id: " + client_id + "</p>";
  server.send(200, "text/html; charset=UTF-8", response);
  
  save_settings_mqtt(address, user, password, port, client_id);
  ESP.restart();
}

void save_settings_mqtt(String address, String user, String password, uint16_t port, String client_id){
  members.begin(NAME_CONFIG_MQTT);
  members.putString("address", address);
  members.putString("user", user);
  members.putString("password", password);
  members.putString("client_id", client_id);
  members.putInt("port", port);
  members.end();
}
