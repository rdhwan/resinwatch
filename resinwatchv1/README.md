# ResinWatchV1

<p align="center">
  <img alt="main logo" src="https://raw.githubusercontent.com/rdhwan/resinwatch/main/src/mainUI.png" width="200">
  <br>
</p>

## Wiring Diagram
<img alt="sketch" src="https://raw.githubusercontent.com/rdhwan/resinwatch/main/src/sketch.png" width="500"></img>

If you change your button input pin, don't forget to change `buttonPin`. In my case i use D6, thus the raw pin is 12.

## Hardware Used 
- ESP8266 board (i use NodeMCU v3 lolin).
- SSD1306 128x64 monochrome OLED display.
- Generic 4/2 pins button (yep no need pullup resistor on this, using the builtin `INPUT_PULLUP`).

## Library Dependencies
- adafruit/Adafruit BusIO@^1.11.6
- adafruit/Adafruit SSD1306@^2.5.3
- adafruit/Adafruit GFX Library@^1.11.1
- bblanchon/ArduinoJson@^6.19.4

If you're using PlatformIO, i have provided the config files.

## Configuration
Please edit these following strings in `resinwatchv1.ino` to your needs:
```c
// Line 12
const char *ssid = "!YOUR_WIFI_SSID_HERE";
const char *password = "!YOUR_WIFI_PASSWORD_HERE";

// Line 16
const char *serverURL = "http://!YOUR_DOMAIN:8000/api/!YOUR_GENSHIN_UID";
// Use your hostname/IP if you are hosting on the same network
// same genshin uid from api .env

// LINE 161
http.addHeader("Authorization", "Basic !YOUR_BASE64_ENCODED_HTTPBASIC_AUTH_PASSWORD");
// BASE64 encoded APP_PASSWORD, from api .env
```

## Project Images
<img alt="image2" src="https://raw.githubusercontent.com/rdhwan/resinwatch/main/src/img2.jpg" width="500">
<img alt="image3" src="https://raw.githubusercontent.com/rdhwan/resinwatch/main/src/img3.jpg" width="500">
