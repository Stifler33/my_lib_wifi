#include <stifler_wifi.h>
#include <stifler_mqtt.h>

#define PIN_RELAY 4

void relay_switch(String message){
  if (message == "on"){
    digitalWrite(PIN_RELAY, HIGH);
    public_data("relay_status", "on");
  }
  if (message == "off"){
    digitalWrite(PIN_RELAY, LOW);
    public_data("relay_status", "off");
  }
}

void setup(){
    pinMode(PIN_RELAY, OUTPUT);
    set_pin_reset(0, LOW);
    init_stif();
    arduino_ota_initial();
    add_pub_topic("connect", "esp32/connect");
    add_pub_topic("relay_status", "stifler/home/bathroom/relay/status");
    add_sub_topic("relay_switch", "stifler/home/bathroom/relay", relay_switch);
    init_brocker();
}

void loop(){
    if (loop_status_wifi()){
        loop_mqtt();
    }    
}
