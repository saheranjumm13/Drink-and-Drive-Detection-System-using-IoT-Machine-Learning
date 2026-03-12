#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


const char* ssid = "OnePlus_Nord";
const char* password = "kartik11";
const char* thingspeakApiKey = "UDYR98JFUDROGATX";
const char* thingspeakServer = "http://api.thingspeak.com/update";

#define MQ2_AO_PIN 34
#define RELAY_PIN  5
#define BUZZER_PIN 33
#define LED_PIN    2
#define RXD2 26    
#define TXD2 25    

LiquidCrystal_I2C lcd(0x27, 16, 2);
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); 

const int alcoholThreshold = 500;                  
const unsigned long uploadIntervalMs = 15000UL;    
const unsigned long warmupTimeMs = 5000;
unsigned long lastUpload = 0;
bool drunkDetected = false;

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);

  pinMode(MQ2_AO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Drink & Drive");
  lcd.setCursor(0, 1);
  lcd.print("System Booting");
  delay(2000);

  lcd.clear();
  lcd.print("Warming MQ2...");
  Serial.println("Warming up MQ2 sensor...");
  delay(warmupTimeMs);

  connectWiFi();
  timeClient.begin();
  timeClient.update();

  lcd.clear();
  lcd.print("System Ready..");
  Serial.println("Setup complete.");
  delay(1500);
  lcd.clear();
}

void loop() {
  while (SerialGPS.available()) gps.encode(SerialGPS.read());

  int MQ2_value = analogRead(MQ2_AO_PIN);
  int percent = mapConstrain(MQ2_value, 500, 2000, 0, 100);
  String timestamp = getTimestamp();

  // Determine drunk state 
  bool isDrunk = (MQ2_value > alcoholThreshold || percent >= 30);

  //  LCD Display Logic 
  lcd.clear();
  if (isDrunk) {
    if (percent < 50) {
      lcd.setCursor(0, 0);
      lcd.print("Mild Alcohol:");
      lcd.setCursor(0, 1);
      lcd.printf("%d%% Wait & Relax", percent);
    } else if (percent < 80) {
      lcd.setCursor(0, 0);
      lcd.print("High Alcohol:");
      lcd.setCursor(0, 1);
      lcd.printf("%d%% Don't Drive!", percent);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("CRITICAL LEVEL!");
      lcd.setCursor(0, 1);
      lcd.printf("%d%% STOP NOW!", percent);
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Sober:Safe to Go");
    lcd.setCursor(0, 1);
    lcd.printf("Level: %d%%", percent);
  }

  Serial.println("------------------------------------------------------");
  Serial.println("ML DATA LOG:");
  Serial.printf("Timestamp: %s\n", timestamp.c_str());
  Serial.printf("MQ2 Value: %d\n", MQ2_value);
  Serial.printf("Alcohol Percentage: %d%%\n", percent);

  if (gps.location.isValid()) {
    Serial.printf("GPS: %.6f, %.6f\n", gps.location.lat(), gps.location.lng());
  } else {
    Serial.println("GPS: No Fix");
  }

  if (isDrunk) {
    Serial.println("Driver Status: DRUNK");
    onDrunkDetected(MQ2_value, percent);
    drunkDetected = true;
    // Blink LED only when drunk
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink >= 500) { // Blink faster (0.5 sec)
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastBlink = millis();
}

  } else {
    Serial.println("Driver Status: SOBER");
    drunkDetected = false;
    unlockIgnition();
    digitalWrite(LED_PIN, LOW);
  }

  //if (drunkDetected) {
    //static unsigned long lastBlink = 0;
    //if (millis() - lastBlink >= 500) {
      //digitalWrite(LED_PIN, !digitalRead(LED_PIN)); 
      //lastBlink = millis();
    //}
  //}

  if (WiFi.status() != WL_CONNECTED) connectWiFi();

  if (WiFi.status() == WL_CONNECTED && millis() - lastUpload > uploadIntervalMs) {
    uploadToThingSpeak(timestamp, MQ2_value, percent,
                       gps.location.isValid() ? gps.location.lat() : 0.0,
                       gps.location.isValid() ? gps.location.lng() : 0.0,
                       isDrunk ? 1 : 0);
    lastUpload = millis();
  }

  Serial.printf("CSV_LOG,%s,%d,%d,%.6f,%.6f,%s\n",
                timestamp.c_str(),
                MQ2_value,
                percent,
                gps.location.isValid() ? gps.location.lat() : 0.0,
                gps.location.isValid() ? gps.location.lng() : 0.0,
                isDrunk ? "DRUNK" : "SOBER");

  static unsigned long lastBlink = 0;
  if (millis() - lastBlink >= 5000) { // Blink every 5 sec
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastBlink = millis();
  }

  delay(1500);
}

void connectWiFi() {
  lcd.clear();
  lcd.print("WiFi connecting");
  WiFi.begin(ssid, password);
  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    lcd.clear();
    lcd.print("WiFi connected");
    Serial.println("\nWiFi connected.");
  } else {
    lcd.clear();
    lcd.print("WiFi failed");
    Serial.println("\nWiFi connection failed.");
  }
  delay(1500);
  lcd.clear();
}

void onDrunkDetected(int MQ2_value, int percent) {
  Serial.println("ALCOHOL DETECTED!");
  lockIgnition();

  static bool buzzed = false;
  if (!buzzed) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
      delay(200);
    }
    buzzed = true;
  }
}

void lockIgnition() {
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Ignition locked.");
}

void unlockIgnition() {
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("Ignition unlocked.");
}

void uploadToThingSpeak(const String &timestamp, int MQ2_value, int percent, double lat, double lng, int drunkStatus) {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  String url = String(thingspeakServer) + "?api_key=" + thingspeakApiKey;
  url += "&field1=" + String(MQ2_value);
  url += "&field2=" + String(percent);
  url += "&field3=" + String(lat, 6);
  url += "&field4=" + String(lng, 6);
  url += "&field5=" + urlencode(timestamp);
  url += "&field6=" + String(drunkStatus);

  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.print("ThingSpeak response: ");
    Serial.println(payload);
  } else {
    Serial.print("ThingSpeak upload failed, err=");
    Serial.println(httpCode);
  }
  http.end();
}

String getTimestamp() {
  timeClient.update();
  time_t rawTime = timeClient.getEpochTime();
  struct tm *ti;
  ti = localtime(&rawTime);
  char buf[30];
  sprintf(buf, "%02d-%02d-%04d %02d:%02d:%02d",
          ti->tm_mday, ti->tm_mon + 1, ti->tm_year + 1900,
          ti->tm_hour, ti->tm_min, ti->tm_sec);
  return String(buf);
}

int mapConstrain(int x, int in_min, int in_max, int out_min, int out_max) {
  if (x < in_min) x = in_min;
  if (x > in_max) x = in_max;
  long val = (long)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  if (val < out_min) val = out_min;
  if (val > out_max) val = out_max;
  return (int)val;
}

String urlencode(String str) {
  String encoded = "";
  char c;
  char code0, code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      encoded += '%';
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 = c - 10 + 'A';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}
