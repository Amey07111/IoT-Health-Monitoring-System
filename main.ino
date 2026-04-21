#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================= Sensors =================
MAX30105 particleSensor;
TinyGPSPlus gps;

// ================= GSM & GPS UART =================
#define RXD2 16
#define TXD2 17

#define RXD1 4
#define TXD1 5

// ================= Button =================
#define BUTTON 18

// ================= Variables =================
long lastBeat = 0;
float bpm = 0;
float avgBPM = 0;

// ================= Setup =================
void setup() {
  Serial.begin(115200);

  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // GPS
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // GSM

  Wire.begin(21, 22);

  pinMode(BUTTON, INPUT_PULLUP);

  // OLED INIT
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  // HEART SENSOR INIT
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    display.println("MAX30105 Error");
    display.display();
    while (1);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
}

// ================= LOOP =================
void loop() {

  // ---------- HEART RATE ----------
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    bpm = 60 / (delta / 1000.0);

    // smoothing filter
    avgBPM = (avgBPM * 0.8) + (bpm * 0.2);
  }

  // ---------- GPS ----------
  while (Serial1.available()) {
    gps.encode(Serial1.read());
  }

  // ---------- DISPLAY ----------
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Health Monitor");

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("BPM:");
  display.println((int)avgBPM);

  if (gps.location.isValid()) {
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.println("GPS OK");
  } else {
    display.setCursor(0, 50);
    display.println("No GPS Fix");
  }

  display.display();

  // ---------- BUTTON ----------
  if (digitalRead(BUTTON) == LOW) {
    delay(50); // debounce

    if (digitalRead(BUTTON) == LOW) {
      sendSMS();
      delay(5000);
    }
  }
}

// ================= SEND SMS =================
void sendSMS() {

  float lat = gps.location.isValid() ? gps.location.lat() : 0.0;
  float lon = gps.location.isValid() ? gps.location.lng() : 0.0;

  String msg = "EMERGENCY ALERT!\n";

  msg += "Heart Rate: ";
  msg += String((int)avgBPM);
  msg += " BPM\n";

  msg += "Location:\n";

  if (gps.location.isValid()) {
    msg += "https://maps.google.com/?q=";
    msg += String(lat, 6);
    msg += ",";
    msg += String(lon, 6);
  } else {
    msg += "GPS Not Available";
  }

  // GSM COMMANDS
  Serial2.println("AT");
  delay(500);

  Serial2.println("AT+CMGF=1");
  delay(500);

  Serial2.println("AT+CMGS=\"+91XXXXXXXXXX\"");
  delay(500);

  Serial2.print(msg);
  delay(500);

  Serial2.write(26); // CTRL+Z

  // OLED message
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(2);
  display.println("SMS SENT");
  display.display();

  delay(2000);
}
