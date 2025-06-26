#define BLYNK_TEMPLATE_ID "TMPL3Y2oN4mEB"
#define BLYNK_TEMPLATE_NAME "baby care"
#define BLYNK_AUTH_TOKEN "LY36RWAyM2D9gFjvqb9I64jjHe46t-Cl"

#include <WiFi.h>
#include <Wire.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include "MPU6050.h"

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define DHTPIN 21
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

Servo cradleServo;
#define SERVOPIN 15

MPU6050 mpu;
int16_t ax, ay, az;

bool swing = false;
String cryStatus = "Sleeping";
String movementStatus = "Baby Sleeping";

BLYNK_WRITE(V6) {
  swing = param.asInt();
}

void swingCradle() {
  cradleServo.write(45);
  delay(500);
  cradleServo.write(135);
  delay(500);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  Wire.begin();
  mpu.initialize();
  cradleServo.setPeriodHertz(50);
  cradleServo.attach(SERVOPIN, 500, 2400);
}

void loop() {
  Blynk.run();

  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "cry") {
      cryStatus = "Crying";
      Serial.println("Baby is crying");
    } else if (input == "sleep") {
      cryStatus = "Sleeping";
      Serial.println("Baby is sleeping");
    } else {
      Serial.println("Unknown input. Type 'cry' or 'sleep'");
    }
  }

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Blynk.virtualWrite(V7, temp);
  Blynk.virtualWrite(V8, hum);
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  mpu.getAcceleration(&ax, &ay, &az);
  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  if (abs(ax_g) > 0.3 || abs(ay_g) > 0.3 || abs(az_g - 1.0) > 0.3) {
    movementStatus = "Baby Moving";
    Serial.println("Movement is there, baby not sleeped");
  } else {
    movementStatus = "Baby Sleeping";
    Serial.println("No movement, baby sleeping");
  }

  String combinedStatus;
  if (cryStatus == "Crying") {
    combinedStatus = "Crying";
  } else if (movementStatus == "Baby Moving") {
    combinedStatus = "Baby Moving";
  } else {
    combinedStatus = "Sleeping";
  }

  Blynk.virtualWrite(V2, combinedStatus);

  if (swing) swingCradle();
  else cradleServo.write(90);

  delay(1000);
}
