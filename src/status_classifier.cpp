#include "status_classifier.h"

String getStatus(int rssi, int ping_ms, float packet_loss) {
  if (ping_ms == -1) {
    return "Poor"; // ping gagal total = pasti jelek
  }

  if (rssi > -60 && ping_ms < 100 && packet_loss < 5) {
    return "Good";
  } else if (rssi > -75 && ping_ms < 200 && packet_loss < 15) {
    return "Fair";
  } else {
    return "Poor";
  }
}