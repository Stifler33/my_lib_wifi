#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <GTimer.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Preferences.h>

#define NAME_CONFIG_WIFI "config_wifi"

void initial_wifi();
void save_config_wifi(String ssid, String password, const char* hostname="", wifi_mode_t wifi_mode=WIFI_STA);

void loop_status_wifi();

void arduino_ota_initial();

void mqtt_initial();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void mqtt_reconnect();
