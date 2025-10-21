#pragma once

#include <PubSubClient.h>
#include <GTimer.h>
#include <Preferences.h>
#include <map>
#include <functional>

#define NAME_CONFIG_MQTT "config_mqtt"
extern std::map<String, const char*> pub_topics;
extern std::map<String, const char*> sub_topics;

/**
 * сохраняем топик для публикации в него данных
 * @param name_topic короткое имя топика
 * @param topic полное имя топика используемое для публикации сообщения
 */
void add_pub_topic(String name_topic, const char* topic);

/**
 * указываем на какой топик надо подписаться и какое действие надо выполнить 
 * 
 * если получили сообщение в этом топике
 * @param name_topic короткое имя топика
 * @param topic полное имя топика на который будет подписка
 * @param handler функция типа void() которая будет выполняться. функция не должна ничего возвращать, и не должна иметь параметров
 */
void add_sub_topic(String name_topic, const char* topic, std::function<void()> handler=nullptr);

/**
 * Функция запускает mqtt клиента
 * 
 * данные берет из flash памяти
 */
void init_brocker();

/**
 * основной цикл работы mqtt сервера
 */
void loop_mqtt();
