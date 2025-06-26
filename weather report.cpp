#define BLYNK_TEMPLATE_ID "TMPL3mneoOQN1"
#define BLYNK_TEMPLATE_NAME "WEATHER"
#define BLYNK_AUTH_TOKEN "FdvJHlY1WJ1sPa17YSUHxXrZ1sDCoOhD"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define DHTPIN 15
#define MQ2PIN 34
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void sendSensorData() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int gas = analogRead(MQ2PIN);
  Blynk.virtualWrite(V2, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V0, gas);
  if (gas > 600) Blynk.logEvent("report", "Gas level high!");
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" °C | Hum: ");
  Serial.print(hum);
  Serial.print(" % | Gas: ");
  Serial.println(gas);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  timer.setInterval(10000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
