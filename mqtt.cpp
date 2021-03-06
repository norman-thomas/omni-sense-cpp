#include "mqtt.h"

namespace mqtt {
  bool maintain_connection(MQTTClient &mqtt, const char* client_id, const std::vector<const char*> &topics) {
    if (mqtt.connected())
      return false;

    bool reconnected = false;
    unsigned int count = 0;
    while (!mqtt.connected()) {
      Serial.print("Attempting MQTT connection with client_id=");
      Serial.print(client_id);
      Serial.println("...");
      if (mqtt.connect(client_id)) {
        Serial.println();
        Serial.println("connected");
        maintain_subscriptions(mqtt, topics);
        reconnected = true;
      } else {
        count++;
        Serial.print(".");
        Serial.print(count);
        Serial.print(".");
        delay(1000);
      }
    }
    Serial.println("done.");
    return reconnected;
  }

  void maintain_subscriptions(MQTTClient &mqtt, const std::vector<const char*> &topics) {
    for (auto topic : topics) {
      mqtt.subscribe(topic);
    }
  }
}

