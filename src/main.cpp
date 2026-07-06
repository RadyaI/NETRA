#include <Wire.h>
#include <RTClib.h>
#include <ArduinoJson.h>

#include "config.h"
#include "wifi_manager.h"
#include "network_metrics.h"
#include "speed_test.h"
#include "status_classifier.h"

RTC_DS1307 rtc;

void setup() {
  Serial.begin(115200);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("RTC nggak ketemu, cek wiring!");
    while (1);
  }
  Serial.println("RTC siap!");

  connectWiFi();
}

void loop() {
  DateTime now = rtc.now();
  char timestamp[25];
  sprintf(timestamp, "%04d-%02d-%02dT%02d:%02d:%02d",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second());

  int rssi = getRSSI();
  int ping_ms = getPingMs();
  float packet_loss = getPacketLossPercent();
  float download_mbps = getDownloadMbps();
  float upload_mbps = getUploadMbps();
  String status = getStatus(rssi, ping_ms, packet_loss);
  unsigned long uptime_seconds = millis() / 1000;

  StaticJsonDocument<256> doc;
  doc["room_id"] = ROOM_ID;
  doc["timestamp"] = timestamp;
  doc["rssi"] = rssi;
  doc["ping_ms"] = ping_ms;
  doc["packet_loss_percent"] = packet_loss;
  doc["download_mbps"] = download_mbps;
  doc["upload_mbps"] = upload_mbps;
  doc["status"] = status;
  doc["uptime_seconds"] = uptime_seconds;

  String jsonOutput;
  serializeJsonPretty(doc, jsonOutput);

  Serial.println("=== Data yang bakal dikirim ===");
  Serial.println(jsonOutput);
  Serial.println("===============================");

  delay(5000); // testing 5 detik dulu, nanti ganti 300000 (5 menit)
}