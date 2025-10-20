#include "template.h"

bool flag_led_status = false;

WiFiClient wifiClient;
PubSubClient client(wifiClient);
Preferences members;
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

// таймер для моргания светодиодом
GTimer<millis> tmr_blink(100, false, GTMode::Timeout);

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
  members.putInt("wifi_mode", 1);  
  members.end();
  members.begin(NAME_CONFIG_WIFI, true);
}

/*
 * Функция реализует подключение к WiFi и задает заданный режим работы WiFi
 *
 * Все данные для подключения она достает из памяти
 *  
 * функция не блокирующая и она ничего не ждет
*/
void initial_wifi(){  
  pinMode(2, OUTPUT);
  init_config_wifi();  
  String ssid = members.getString("ssid", "");
  wifi_mode_t wifi_mode = wifi_mode_t(members.getInt("wifi_mode", 0));
  const char* hostname = members.getString("hostname", "").c_str();  
  String password = members.getString("password", "");
  members.end();

  if (hostname != ""){
    WiFi.setHostname(hostname);
  }
  
  if (ssid != ""){
    WiFi.mode(wifi_mode);
    WiFi.begin(ssid, password);
  }else{
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32_APP");
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
 * Цикл для отслеживания подключения к Wifi и OTA
 */
void loop_status_wifi(){    
  if (WiFi.status() != WL_CONNECTED){
    blink_led(100);
  }else{
    led_on();
    ArduinoOTA.handle();
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