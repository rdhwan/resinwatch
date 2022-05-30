# ResinWatch

<p align="center">
  <img alt="main logo" src="https://raw.githubusercontent.com/rdhwan/resinwatch/main/src/mainLogo.png" width="250">
  <br>
  <b>Genshin mini-stats tracker project using ESP8266 and SSD1306 OLED display</b>
  <br>
  <br>
</p>

## About
This is my "amateur/beginner" attempt to understanding the basic of NodeMCU (ESP8266). You may find bad code or wrong parameters since im not a C/C++ programmer (most on python hehe).

Also using FastApi and genshin.py as backend service.

## How it works
0. Connect to the provided wifi on setup.
1. When the button is pressed, the logo shows up.
2. Fetch some data from api using ESP8266HTTPClient store it as string.
3. Parse the string data using ArduinoJson library.
4. Render the parsed data nicely on the OLED display.
5. After 10s of displaying, the display will turn off.

## Hardware Used 
- ESP8266 board (i use NodeMCU v3 lolin)
- SSD1306 128x64 monochrome OLED display
- Generic 4/2 pins button (yep no need pullup resistor on this, i used the builtin `INPUT_PULLUP`)

## Projects
This project consist of two sub-project :
- resinwatchv1
- resinwatch-api

Please refer to respective readme inside.

## Project Documentation
<img alt="image2" src="https://raw.githubusercontent.com/rdhwan/resinwatch/main/src/img2.jpg" width="500">
<img alt="image3" src="https://raw.githubusercontent.com/rdhwan/resinwatch/main/src/img3.jpg" width="500">
[![resinwatch youtube](https://img.youtube.com/vi/2ba3liDg4jw/0.jpg)](https://www.youtube.com/watch?v=2ba3liDg4jw)

## License
This project is licensed under the MIT.
