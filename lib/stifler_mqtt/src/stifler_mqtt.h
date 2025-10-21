#pragma once

#include <PubSubClient.h>
#include <GTimer.h>
#include <Preferences.h>
#include <map>

#define NAME_CONFIG_MQTT "config_mqtt"
extern std::map<String, String> pub_topics;
void add_pub_topic(String name_topic, String topic);
void init_brocker();
void loop_mqtt();
