#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// SSID
const char *ssid = "!YOUR_WIFI_SSID_HERE";
const char *password = "!YOUR_WIFI_PASSWORD_HERE";

// API
const char *serverURL = "http://!YOUR_DOMAIN/api/!YOUR_GENSHIN_UID";
String dataReading;

// JSON
StaticJsonDocument<256> doc;

// BUTTON ganti ke 12 - D6 klo NodeMCU!
const int buttonPin = 12;
// variables will change:
int buttonState = 0;

// SCREEN
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 'logo3', 128x64px
const unsigned char epd_bitmap_logo [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0f, 0xf0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x03, 0xc0, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x01, 0xc0, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x01, 0x80, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x81, 0x81, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 
  0x7f, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x0f, 0xc0, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x00, 
  0x78, 0xf0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x07, 0x81, 0x83, 0x80, 0x01, 0xc0, 0x00, 0x07, 0x00, 
  0x78, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc3, 0xc7, 0x00, 0x01, 0xc0, 0x00, 0x07, 0x00, 
  0x78, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc3, 0xc7, 0x00, 0x01, 0xc0, 0x00, 0x07, 0x00, 
  0x78, 0x78, 0x1c, 0x01, 0xe4, 0x08, 0x0b, 0xc3, 0xc3, 0xc7, 0x0f, 0x81, 0xe0, 0x0e, 0x07, 0xb8, 
  0x78, 0x78, 0x7f, 0x87, 0xfc, 0x78, 0x7f, 0xe1, 0xc7, 0xe7, 0x1f, 0xc3, 0xfc, 0x7f, 0x87, 0xfe, 
  0x78, 0xf0, 0xf3, 0x87, 0x0c, 0x78, 0x7c, 0xf1, 0xe7, 0xe6, 0x38, 0xe1, 0xc0, 0xf1, 0xc7, 0x8e, 
  0x7f, 0xf0, 0xe1, 0xc7, 0x04, 0x38, 0x38, 0x71, 0xe6, 0xe6, 0x30, 0xe1, 0xc0, 0xe1, 0xc7, 0x0e, 
  0x7f, 0xc0, 0xe1, 0xc7, 0xc0, 0x38, 0x38, 0x71, 0xee, 0xfe, 0x00, 0xe1, 0xc1, 0xe0, 0xc7, 0x0e, 
  0x79, 0xe1, 0xff, 0xc3, 0xf0, 0x38, 0x38, 0x70, 0xfc, 0xfe, 0x0f, 0xe1, 0xc1, 0xe0, 0x07, 0x0e, 
  0x78, 0xe1, 0xff, 0xc1, 0xf8, 0x38, 0x38, 0x70, 0xfc, 0x7e, 0x1e, 0xe1, 0xc1, 0xc0, 0x07, 0x0e, 
  0x78, 0xf1, 0xe0, 0x00, 0x7c, 0x38, 0x38, 0x70, 0xfc, 0x7c, 0x38, 0xe1, 0xc1, 0xe0, 0x07, 0x0e, 
  0x78, 0x70, 0xe0, 0x00, 0x1c, 0x38, 0x38, 0x70, 0x78, 0x7c, 0x38, 0xe1, 0xc1, 0xe0, 0x07, 0x0e, 
  0x78, 0x78, 0xf0, 0x44, 0x1c, 0x38, 0x38, 0x70, 0x78, 0x3c, 0x38, 0xe1, 0xe0, 0xf0, 0x47, 0x0e, 
  0x70, 0x3c, 0x7f, 0xc7, 0xfc, 0x38, 0x38, 0x70, 0x78, 0x3c, 0x3f, 0xf1, 0xfc, 0xff, 0xc7, 0x0e, 
  0x60, 0x1e, 0x3f, 0x01, 0xf0, 0x38, 0x38, 0x70, 0x30, 0x3c, 0x1e, 0x70, 0xf8, 0x3f, 0x07, 0x0e, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xce, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x9e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1d, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'resin', 16x16
const unsigned char epd_bitmap_resin [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x0e, 0x00, 0x0e, 0x00, 
  0x0e, 0x00, 0x0f, 0x08, 0x07, 0xf8, 0x07, 0xf0, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

  // 'commission', 16x16
const unsigned char epd_bitmap_commission [] PROGMEM = {
  0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x02, 0x40, 0x0e, 0x70, 0x0c, 0x30, 0x18, 0x18, 0x61, 0x86, 
  0x73, 0xce, 0x1f, 0xf8, 0x0f, 0xf0, 0x0f, 0xf0, 0x03, 0xc0, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00
};

void drawOled(String usernameLvl, String uid, String resinCount, String commissionCount) {
  display.clearDisplay();
  Serial.println(F("Drawing"));
  
  display.drawRoundRect(0, 0, 128, 64, 2, WHITE);

  display.drawBitmap(
    34,
    28,
    epd_bitmap_resin, 16, 16, 1);

  display.drawBitmap(
    34,
    44,
    epd_bitmap_commission, 16, 16, 1);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // RESIN
  display.setCursor(54, 34);
  display.println(resinCount);

  // DAILY
  display.setCursor(54, 50);
  display.println(commissionCount);

    // first line
  display.setCursor((SCREEN_WIDTH - usernameLvl.length()*6) / 2, 4);
  display.println(usernameLvl);

  display.setCursor((SCREEN_WIDTH - uid.length()*6) / 2, 14);
  display.println(uid);

  display.display(); 
};

String httpGETRequest(const char *serverURL) {
  Serial.println(F("HTTP GET Request"));
  Serial.println(serverURL);

  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverURL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Basic !YOUR_BASE64_ENCODED_HTTPBASIC_AUTH_PASSWORD");
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
      return payload;
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  return "";
}


void setup() {
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.drawBitmap(
    0, 
    0, 
    epd_bitmap_logo, 128, 64, 1);
  display.display();

  // init wifi

  WiFi.begin(ssid, password);
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println();
  Serial.println(F("WiFi connected with IP: "));
  Serial.println(WiFi.localIP());

  // Init buttonPin PULLUP
  pinMode(buttonPin, INPUT_PULLUP);
    
  display.ssd1306_command(SSD1306_DISPLAYOFF);
  Serial.println("Display turned OFF");
}


void loop() {

  // check if pushbutton is pressed
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {

    display.ssd1306_command(SSD1306_DISPLAYON);
    Serial.println("Button Pressed + Display turned ON");

    display.clearDisplay();
    display.drawBitmap(
      0, 
      0, 
      epd_bitmap_logo, 128, 64, 1);
    display.display();

    // fetch data from api
    dataReading = httpGETRequest(serverURL);

    DeserializationError error = deserializeJson(doc, dataReading);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    if (doc["code"] != 200) {
      Serial.println(F("Error: Something went wrong with the data"));
      // Serial.println(doc["message"]);
      return;
    }

    JsonObject data = doc["data"];
    
    display.clearDisplay();
    drawOled(data["username_lvl"], data["uid"], data["resin"], data["commission"]); // draw content on screen
    delay(10000); // add 10s delay to show content on screen

    display.ssd1306_command(SSD1306_DISPLAYOFF);
    Serial.println("Display turned OFF");
  }
}
