#include <Wire.h>
#include <WiFi.h>

/* MYOSA */
#include <AccelAndGyro.h>
#include <TempAndHumidity.h>
#include <BarometricPressure.h>
#include <LightProximityAndGesture.h>
#include <AirQuality.h>
#include <oled.h>
#include <Actuator.h>

/* Heartbeat */
#include "MAX30105.h"
#include "heartRate.h"

/* IR Temp */
#include <Adafruit_MLX90614.h>

/* =========================
   SENSOR OBJECTS
========================= */

AccelAndGyro imu;
TempAndHumidity sht31;
BarometricPressure bmp180;
LightProximityAndGesture apds;
AirQuality air;
oLed display(128,64);
Actuator actuator;

MAX30105 particleSensor;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

/* =========================
   PINS
========================= */

#define ECG_PIN 34
#define GSR_PIN 35
#define BUZZER_PIN 25

/* =========================
   VARIABLES
========================= */

float baselineSkinTemp = 0;
float baselineHumidity = 0;
float baselineGSR = 0;

float adRisk = 0;

bool calibrated = false;

long lastBeat = 0;
float beatsPerMinute;
int beatAvg;

unsigned long calibrationStart;

/* =========================
   SETUP
========================= */

void setup() {

  Serial.begin(115200);

  Wire.begin();

  pinMode(BUZZER_PIN, OUTPUT);

  /* OLED */
  display.begin();
  display.clearDisplay();

  display.setCursor(0,0);
  display.print("AYRA Initializing");
  display.display();

  /* MYOSA Sensors */
  imu.begin();
  sht31.begin();
  bmp180.begin();
  apds.begin();
  air.begin();
  actuator.ping();

  /* MAX30102 */
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {

    Serial.println("MAX30102 NOT FOUND");

    while (1);
  }

  particleSensor.setup();

  /* MLX90614 */
  mlx.begin();

  calibrationStart = millis();

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Calibrating...");
  display.display();
}

/* =========================
   LOOP
========================= */

void loop() {

  if(!calibrated) {

    calibrateSystem();
    return;
  }

  readSensors();

  calculateADRisk();

  updateOLED();

  handleAlerts();

  delay(200);
}

/* =========================
   CALIBRATION
========================= */

void calibrateSystem() {

  if(millis() - calibrationStart < 30000) {

    baselineSkinTemp += mlx.readObjectTempC();

    baselineHumidity += sht31.getRelativeHumidity(false);

    baselineGSR += analogRead(GSR_PIN);

    delay(100);

  } else {

    baselineSkinTemp /= 300.0;
    baselineHumidity /= 300.0;
    baselineGSR /= 300.0;

    calibrated = true;

    Serial.println("Calibration Complete");
  }
}

/* =========================
   SENSOR VARIABLES
========================= */

float jerkIndex;
float skinTempDrop;
float humidityRise;
float gsrRise;
float heartRisk;
float spasmRisk;

/* =========================
   READ SENSORS
========================= */

void readSensors() {

  /* ======================
     MPU6050
  ====================== */

  float ax = imu.getAccelX(false);
  float ay = imu.getAccelY(false);
  float az = imu.getAccelZ(false);

  jerkIndex = sqrt(ax*ax + ay*ay + az*az);

  /* ======================
     GSR
  ====================== */

  int gsrRaw = analogRead(GSR_PIN);

  gsrRise = gsrRaw - baselineGSR;

  /* ======================
     Skin Temp
  ====================== */

  float skinTemp = mlx.readObjectTempC();

  skinTempDrop = baselineSkinTemp - skinTemp;

  /* ======================
     Humidity
  ====================== */

  float humidity = sht31.getRelativeHumidity(false);

  humidityRise = humidity - baselineHumidity;

  /* ======================
     MAX30102 Heart Rate
  ====================== */

  long irValue = particleSensor.getIR();

  if(checkForBeat(irValue)) {

    long delta = millis() - lastBeat;

    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if(beatsPerMinute < 255 && beatsPerMinute > 20) {

      beatAvg = (beatAvg + beatsPerMinute) / 2;
    }
  }

  /* ======================
     ECG
  ====================== */

  int ecgValue = analogRead(ECG_PIN);

  /* ======================
     Gesture Dismiss
  ====================== */

  char* gesture = apds.getGesture(false);

  if(strcmp(gesture, "LEFT") == 0 ||
     strcmp(gesture, "RIGHT") == 0) {

    adRisk = 0;

    noTone(BUZZER_PIN);

    Serial.println("Alert dismissed");
  }

  /* ======================
     DEBUG
  ====================== */

  Serial.println("====== AYRA DATA ======");

  Serial.print("Heart BPM: ");
  Serial.println(beatsPerMinute);

  Serial.print("Skin Temp Drop: ");
  Serial.println(skinTempDrop);

  Serial.print("GSR Rise: ");
  Serial.println(gsrRise);

  Serial.print("Humidity Rise: ");
  Serial.println(humidityRise);

  Serial.print("Jerk Index: ");
  Serial.println(jerkIndex);

  Serial.print("ECG: ");
  Serial.println(ecgValue);

  Serial.println("=======================");
}

/* =========================
   AI RISK FUSION
========================= */

void calculateADRisk() {

  /* HR Spike */

  heartRisk = normalize(beatsPerMinute, 80, 140);

  /* Spasm */

  spasmRisk = normalize(jerkIndex, 1.2, 4.0);

  /* GSR */

  float gsrRisk = normalize(gsrRise, 50, 500);

  /* Temp */

  float tempRisk = normalize(skinTempDrop, 0.5, 5);

  /* Humidity */

  float humidityRisk = normalize(humidityRise, 2, 20);

  /* Fusion */

  adRisk =
      (heartRisk * 25.0) +
      (spasmRisk * 20.0) +
      (gsrRisk * 25.0) +
      (tempRisk * 15.0) +
      (humidityRisk * 15.0);

  if(adRisk > 100)
    adRisk = 100;

  if(adRisk < 0)
    adRisk = 0;
}

/* =========================
   OLED DISPLAY
========================= */

void updateOLED() {

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0,0);
  display.print("AYRA AD MONITOR");

  display.setCursor(0,12);
  display.print("Risk:");
  display.print(adRisk);

  display.setCursor(0,24);
  display.print("HR:");
  display.print(beatsPerMinute);

  display.setCursor(64,24);
  display.print("GSR:");
  display.print(gsrRise);

  display.setCursor(0,36);
  display.print("Temp:");
  display.print(skinTempDrop);

  display.setCursor(64,36);
  display.print("Spasm:");
  display.print(jerkIndex);

  display.display();
}

/* =========================
   ALERT SYSTEM
========================= */

void handleAlerts() {

  if(adRisk >= 60) {

    tone(BUZZER_PIN, 2500);

    display.clearDisplay();

    display.setTextSize(2);

    display.setCursor(10,20);
    display.print("AD ALERT");

    display.display();

    Serial.println("!!! HIGH RISK AD DETECTED !!!");

  } else {

    noTone(BUZZER_PIN);
  }
}

/* =========================
   NORMALIZATION
========================= */

float normalize(float value, float minVal, float maxVal) {

  if(value < minVal)
    value = minVal;

  if(value > maxVal)
    value = maxVal;

  return (value - minVal) / (maxVal - minVal);
}