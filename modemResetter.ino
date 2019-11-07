/*
  NTPClient
  esp8266Ping
*/

#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

const char* ssid     = "****";
const char* password = "****";

const char* remote_host = "www.google.com";

RTC_PCF8523 rtc;

const int daysInterval = 0;
const int hoursInterval = 0;
const int minutesInterval = 10;
const int secondsInterval = 0;

unsigned long currentUNIXtime;
unsigned long futureUNIXtime;

const int ledPin = 0;
const int relayPin = 16;
const int SD_CS = 15;

String dataString;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(relayPin, LOW);

//  Serial.begin(115200);
//  delay(200);
//
//  Serial.println();
//  Serial.println();
//  Serial.print("Connecting to WiFi: ");
//  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
//  Serial.print("WiFi connected with IP ");
//  Serial.println(WiFi.localIP());
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);

//  Serial.println("Checking SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CS)) {
//    Serial.println("SD card failed, or not present");
    while (1) {
      digitalWrite(ledPin, HIGH);
      delay(1000);
    }
  }
//  Serial.println("SD card ready.");

//  if (! rtc.begin()) {
//    Serial.println("RTC not available");
//  }

  DateTime now = rtc.now();
  DateTime future (now + TimeSpan(daysInterval, hoursInterval, minutesInterval, secondsInterval));

  currentUNIXtime = now.unixtime();
  futureUNIXtime = future.unixtime();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED) {
//    Serial.print("WiFi disconected, reconnecting.");
    DateTime now = rtc.now();
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("wifi disconnected at: ");
      dataFile.print(now.hour(), DEC);
      dataFile.print(':');
      dataFile.print(now.minute(), DEC);
      dataFile.print(':');
      dataFile.print(now.second(), DEC);
      dataFile.print(", ");
      dataFile.print(now.year(), DEC);
      dataFile.print('-');
      dataFile.print(now.month(), DEC);
      dataFile.print('-');
      dataFile.println(now.day(), DEC);
      dataFile.close();
    }
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
//      Serial.print(".");
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      delay(200);
    }
    digitalWrite(ledPin, LOW);
//    Serial.println();
//    Serial.print("WiFi connected with ip ");
//    Serial.println(WiFi.localIP());
    now = rtc.now();
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("wifi connected at: ");
      dataFile.print(now.hour(), DEC);
      dataFile.print(':');
      dataFile.print(now.minute(), DEC);
      dataFile.print(':');
      dataFile.print(now.second(), DEC);
      dataFile.print(", ");
      dataFile.print(now.year(), DEC);
      dataFile.print('-');
      dataFile.print(now.month(), DEC);
      dataFile.print('-');
      dataFile.println(now.day(), DEC);
      dataFile.close();
    }
  }

  DateTime now = rtc.now();
  currentUNIXtime = now.unixtime();

  if (currentUNIXtime >= futureUNIXtime) {
    DateTime future (now + TimeSpan(daysInterval, hoursInterval, minutesInterval, secondsInterval));
    futureUNIXtime = future.unixtime();
    if (! Ping.ping(remote_host)) {
//      Serial.println("Ping unsuccessful.");
//      Serial.println("Restarting modem.");
      digitalWrite(relayPin, HIGH);
      DateTime now = rtc.now();
      File dataFile = SD.open("datalog.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print("Modem reset at: ");
        dataFile.print(now.hour(), DEC);
        dataFile.print(':');
        dataFile.print(now.minute(), DEC);
        dataFile.print(':');
        dataFile.print(now.second(), DEC);
        dataFile.print(", ");
        dataFile.print(now.year(), DEC);
        dataFile.print('-');
        dataFile.print(now.month(), DEC);
        dataFile.print('-');
        dataFile.println(now.day(), DEC);
        dataFile.close();
      }
      delay(30000);
      digitalWrite(relayPin, LOW);
    }
//    else {
//      Serial.println("Successful ping");
//    }
  }
  delay(500);
}

void dataLog(void) {
//bring datalog into its own function

}
