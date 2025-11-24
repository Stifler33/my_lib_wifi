// HTML страница для воода ssid, пароля и hostname
const char* HTML_CONTENT = R"raw_html(
<!DOCTYPE html>
<html>
<head>
    <title>Настройка Wi-Fi и Hostname ESP32</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f4f4f4; }
        .container { background-color: #ffffff; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); max-width: 400px; margin: auto; }
        h2 { text-align: center; color: #333; }
        label { display: block; margin-bottom: 5px; font-weight: bold; color: #555; }
        input[type="text"], input[type="password"] { width: 100%; padding: 10px; margin-bottom: 15px; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; }
        input[type="submit"] { background-color: #007bff; color: white; padding: 10px 15px; border: none; border-radius: 4px; cursor: pointer; width: 100%; font-size: 16px; }
        input[type="submit"]:hover { background-color: #0056b3; }
    </style>
</head>
<body>

<div class="container">
    <h2>Настройка Stifler_ESP</h2>
    
    <form action="/save_config" method="POST">
        
        <label for="ssid">Имя сети Wi-Fi (SSID):</label>
        <input type="text" id="ssid" name="ssid" required placeholder="Введите SSID">

        <label for="password">Пароль Wi-Fi (необязательно):</label>
        <input type="password" id="password" name="password" placeholder="Введите пароль">

        <label for="hostname">Имя хоста (Hostname):</label>
        <input type="text" id="hostname" name="hostname" placeholder="Введите имя хоста (напр., esp32-device)">

        <input type="submit" value="Сохранить и подключиться">
    </form>
</div>

</body>
</html>
)raw_html";

const char* HTML_MQTT = R"raw_html(
<!DOCTYPE html>
<html>
<head>
    <title>Настройка MQTT Брокера ESP32</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
            background-color: #f4f4f4;
        }
        .container {
            background-color: #ffffff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            max-width: 400px;
            margin: auto;
        }
        h2 {
            text-align: center;
            color: #333;
            margin-bottom: 20px;
        }
        label {
            display: block;
            margin-bottom: 5px;
            font-weight: bold;
            color: #555;
        }
        input[type="text"], input[type="password"], input[type="number"] {
            width: 100%;
            padding: 10px;
            margin-bottom: 15px;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box; 
        }
        input[type="submit"] {
            background-color: #28a745; /* Зеленый цвет для MQTT */
            color: white;
            padding: 10px 15px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            width: 100%;
            font-size: 16px;
        }
        input[type="submit"]:hover {
            background-color: #1e7e34;
        }
        .note {
            font-size: 0.9em;
            color: #888;
            margin-top: -10px;
            margin-bottom: 10px;
            display: block;
        }
    </style>
</head>
<body>

<div class="container">
    <h2>Настройка MQTT</h2>
    
    <form action="/save_mqtt_config" method="POST">
        
        <label for="mqtt_server">Адрес Брокера (IP/Hostname):</label>
        <input type="text" id="mqtt_server" name="mqtt_server" required placeholder="Напр., 192.168.1.10 или broker.hivemq.com">

        <label for="mqtt_port">Порт Брокера:</label>
        <input type="number" id="mqtt_port" name="mqtt_port" required value="1883" placeholder="Обычно 1883 (или 8883 для SSL)">
        
        <hr style="border: 0; border-top: 1px solid #eee; margin: 20px 0;">

        <label for="mqtt_client_id">Client ID (ClientID):</label>
        <input type="text" id="mqtt_client_id" name="mqtt_client_id" placeholder="Оставьте пустым, чтобы использовать Hostname ESP32">
        <span class="note">Уникальный идентификатор для подключения.</span>

        <label for="mqtt_user">Логин (Username):</label>
        <input type="text" id="mqtt_user" name="mqtt_user" placeholder="Оставьте пустым, если не требуется">

        <label for="mqtt_pass">Пароль (Password):</label>
        <input type="password" id="mqtt_pass" name="mqtt_pass" placeholder="Оставьте пустым, если не требуется">

        <input type="submit" value="Сохранить настройки MQTT">
    </form>
</div>

</body>
</html>
)raw_html";