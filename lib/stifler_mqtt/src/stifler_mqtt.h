#pragma once

#include <PubSubClient.h>
#include <GTimer.h>
#include <Preferences.h>
#include <map>
#include <functional>

#define NAME_CONFIG_MQTT "config_mqtt"
extern std::map<String, const char*> pub_topics;
extern std::map<String, const char*> sub_topics;
void add_pub_topic(String name_topic, const char* topic);
void add_sub_topic(String name_topic, const char* topic, std::function<void(String)> handler=nullptr);
// void add_handler(const char* topic, std::function<void()> handler);
void init_brocker();
void loop_mqtt();
void public_data(String name_topic, const char* payload);
