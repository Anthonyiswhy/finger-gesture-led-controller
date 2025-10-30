#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "WIFI";       // <-- your Wi-Fi name
const char* password = "PASS";   // <-- your Wi-Fi password

AsyncWebServer server(80);

// LED pins
const int leds[] = {5, 18, 17, 19, 22}; // thumb to pinky

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected. IP: " + WiFi.localIP().toString());

  // Route to control LEDs: /leds?thumb=1&index=0&...
  server.on("/leds", HTTP_GET, [](AsyncWebServerRequest *request){
    String fingers[] = {"thumb", "index", "middle", "ring", "pinky"};
    for (int i = 0; i < 5; i++) {
      if (request->hasParam(fingers[i])) {
        String state = request->getParam(fingers[i])->value();
        digitalWrite(leds[i], state == "1" ? HIGH : LOW);
      }
    }
    request->send(200, "text/plain", "LEDs updated.");
  });

  server.begin();
}
void loop() {}
