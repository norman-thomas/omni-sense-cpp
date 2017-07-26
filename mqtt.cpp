#include "mqtt.h"

namespace mqtt {
  bool maintain_mqtt_connection(Adafruit_MQTT_Client mqtt) {
    bool reconnected = false;
    while (!mqtt.connected()) {
      Serial.print("Attempting MQTT connection...");
      mqtt.will("mqtt/connect", "myName");
      if (mqtt.connect()) {
        Serial.println("connected");
        reconnected = true;
      } else {
        Serial.print("failed, try again in 5 seconds");
        delay(5000);
      }
    }
    return reconnected;
  }

  void maintain_mqtt_subscriptions(Adafruit_MQTT_Client mqtt, std::vector<Adafruit_MQTT_Subscribe> subscriptions, bool reconnected) {
    if (!reconnected)
      return;

    for (Adafruit_MQTT_Subscribe sub : subscriptions) {
      mqtt.subscribe(&sub);
    }
  }
}

