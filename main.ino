#include <Adafruit_ST7735.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
const char* ssid = "HUAWEI-RUY8ND_HiLink";
const char* pass = "helloworldnew";
const char* host = "api.seniverse.com";
const int httpPort = 80;
String reqUserKey = "SfWiAYPlCs1ghFBEE";
String reqLocation = "jingzhou";
String reqUnit = "c";
#define TFT_CS        4
#define TFT_RST       16
#define TFT_DC        5
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  testdrawtext("Yoloboard Version: 1\n\n\n\n\n\n\nPowered By Attacker", ST7735_WHITE, 2);
  delay(5000);
  connectWiFi();
}
void loop() {
  String request = "/v3/weather/now.json?key="
                   + reqUserKey + "&location="
                   + reqLocation + "&language=en&unit="
                   + reqUnit;
  httpRequest(request);
}
void frame(uint16_t bgColor, char *text, uint16_t color, uint8_t size, unsigned long time) {
  tft.fillScreen(bgColor);
  testdrawtext(text, color, size);
  delay(time);
}
void loding() {
  frame(ST7735_BLACK, "NOW LOADING", ST7735_WHITE, 5, 1000);
  frame(ST7735_CYAN, "NOW LOADING\n.", ST7735_WHITE, 5, 1000);
  frame(ST7735_BLUE, "NOW LOADING\n..", ST7735_WHITE, 5, 1000);
  frame(ST7735_ORANGE, "NOW LOADING\n...", ST7735_WHITE, 5, 1000);
  frame(ST7735_YELLOW, "NOW LOADING\n....", ST7735_WHITE, 5, 1000);
}
void connectWiFi() {
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    loding();
  }
  frame(ST7735_BLACK, "\n\n\nConnected\nto the\nInternet!", ST7735_ORANGE, 2, 3000);
}
void httpRequest(String request) {
  WiFiClient client;
  String newRequest = String("GET ") + request + " HTTP/1.1\r\n" + 
                              "Host: " + host + "\r\n" + 
                              "Connection: close\r\n\r\n";
  if (client.connect(host, 80)) {
    client.print(newRequest);
    if (client.find("\r\n\r\n")) {
      frame(ST7735_ORANGE, "Load\ninfomation\nsuccessed", ST7735_WHITE, 2, 2*1000);
    }
    parseInfo(client);
  } else {
    loding();
  }
}
void parseInfo(WiFiClient client){
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, client);
  JsonObject results_0 = doc["results"][0];
  JsonObject results_0_now = results_0["now"];
  const char* results_0_now_text = results_0_now["text"];
  const char* results_0_now_code = results_0_now["code"];
  const char* results_0_now_temperature = results_0_now["temperature"];
  const char* results_0_last_update = results_0["last_update"];
  frame(ST7735_WHITE, (char*) results_0_now_text, ST7735_BLACK, 5, 3*1000);
  frame(ST7735_WHITE, (char*) results_0_now_code, ST7735_ORANGE, 5, 3*1000);
  frame(ST7735_WHITE, (char*) results_0_now_temperature, ST7735_BLUE, 5, 3*1000);
  frame(ST7735_BLACK, (char*) results_0_last_update, ST7735_WHITE, 3, 3*1000);
}
void testdrawtext(char *text, uint16_t color, uint8_t size) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setTextSize(size);
  tft.print(text);
}
