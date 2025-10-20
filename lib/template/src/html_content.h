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
