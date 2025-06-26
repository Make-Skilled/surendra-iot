#define BLYNK_TEMPLATE_ID "TMPL3dM27L4OW"
#define BLYNK_TEMPLATE_NAME "attandence"
#define BLYNK_AUTH_TOKEN "mBRe16AUFuCenokQr3S087gRTDR9RY6H"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <time.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define IR_SENSOR 13

int memberCount = 4;
bool lastState = HIGH;
unsigned long previousMillis = 0;
const unsigned long interval = 2000;
bool alertSent = false;

void setup() {
  Serial.begin(115200);
  pinMode(IR_SENSOR, INPUT_PULLUP);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  configTime(19800, 0, "pool.ntp.org");
}

void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    time_t now = time(nullptr);
    struct tm *t = localtime(&now);
    int hour = t->tm_hour;

    bool currentState = digitalRead(IR_SENSOR);
    if (currentState == LOW && lastState == HIGH) {
      if (memberCount > 0) memberCount--;
    }
    lastState = currentState;

    Serial.print("Room 101 - Members: ");
    Serial.println(memberCount);
    Blynk.virtualWrite(V0, memberCount);

    if ((hour >= 21 || hour < 6) && memberCount < 4) {
      if (!alertSent) {
        Blynk.logEvent("low_member_alert", "Room 101: Only " + String(memberCount) + " members detected.");
        alertSent = true;
      }
    } else {
      alertSent = false;
    }
  }
}
