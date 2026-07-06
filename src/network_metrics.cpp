#include "network_metrics.h"
#include "config.h"
#include <WiFi.h>
#include <ESP32Ping.h>

int getRSSI() {
  return WiFi.RSSI();
}

int getPingMs() {
  bool success = Ping.ping(PING_HOST, 4);
  if (success) {
    return Ping.averageTime();
  }
  return -1; // -1 artinya ping gagal total
}

float getPacketLossPercent() {
  int sent = 4;
  int received = 0;

  for (int i = 0; i < sent; i++) {
    if (Ping.ping(PING_HOST, 1)) {
      received++;
    }
  }

  return ((sent - received) / (float)sent) * 100.0;
}