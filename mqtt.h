#include <MQTTClient.h>
#include <vector>

namespace mqtt {
  bool maintain_connection(MQTTClient &mqtt, const char* client_id, const std::vector<const char*> &topics);
  void maintain_subscriptions(MQTTClient &mqtt, const std::vector<const char*> &topics);
}
