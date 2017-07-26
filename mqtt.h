#include <Adafruit_MQTT_Client.h>
#include <vector>

namespace mqtt {
  bool maintain_mqtt_connection(Adafruit_MQTT_Client mqtt);
  void maintain_mqtt_subscriptions(Adafruit_MQTT_Client mqtt, std::vector<Adafruit_MQTT_Subscribe> subscriptions, bool reconnected);
}
