#include <template.h>
#include <stifler_mqtt.h>

void setup(){
    set_pin_reset(12, HIGH);
    init_stif();
    arduino_ota_initial();
    add_pub_topic("connect", "esp32/connect");
    add_sub_topic("test_sub", "esp32/test_sub");
    init_brocker();
}

void loop(){
    if (loop_status_wifi()){
        loop_mqtt();
    }    
}