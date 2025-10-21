#include "stifler_mqtt.h"
#include <WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);
Preferences flash;
std::map<String, String> pub_topics;
GTimer<millis> tmr_mqtt_loop(1000, true, GTMode::Interval);

struct Subscribe_data
{
    String login;
    String password;
    String address;
    uint16_t port;
    // String topics[];
}sub_data;

void get_settings(){
    if (flash.begin(NAME_CONFIG_MQTT, true)){
        sub_data.address = flash.getString("address", "");
        sub_data.login = flash.getString("user", "");
        sub_data.password = flash.getString("password", "");
        sub_data.port = flash.getInt("port", 1883);
        flash.end();
    }else{
        Serial.println("open flash error");
    } 
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Сообщение получено от топика: ");
  Serial.println(topic);
  
  // Вывод содержимого сообщения
  Serial.print("Содержимое: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void init_brocker(){
    get_settings();
    client.setServer(sub_data.address.c_str(), sub_data.port);
    client.setCallback(callback);
}

void reconnect() {
  // Цикл, пока не подключимся
  if (!client.connected()) {    
    // Попытка подключения
    if (client.connect("my_id", sub_data.login.c_str(), sub_data.password.c_str())) {         
      // Отправляем первое сообщение о статусе      
      client.publish(pub_topics["connect"].c_str(), "ON");
    }
  }
}

void loop_mqtt(){
    if (tmr_mqtt_loop && !client.connected()){
        reconnect();
    }
}

void add_pub_topic(String name_topic, String topic){
    pub_topics[name_topic] = topic;
}