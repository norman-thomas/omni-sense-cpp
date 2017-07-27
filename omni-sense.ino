#include <ESP8266WiFi.h>

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include "wifi.h"
#include "mqtt.h"
#include "bmp280.h"
#include "rain.h"

#include "credentials.h"

WiFiClient wifiClient;
Adafruit_MQTT_Client mqttClient(&wifiClient, AIO_SERVER, AIO_SERVERPORT);
Adafruit_MQTT_Publish pressure = Adafruit_MQTT_Publish(&mqttClient, "wohnzimmer/pressure", 1);
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqttClient, "wohnzimmer/temp2", 1);
Adafruit_MQTT_Publish rainWater = Adafruit_MQTT_Publish(&mqttClient, "wohnzimmer/rain", 1);
Adafruit_MQTT_Subscribe measure = Adafruit_MQTT_Subscribe(&mqttClient, "airquality/measure", 1);

const std::vector<Adafruit_MQTT_Subscribe> mqtt_subscriptions = { measure };

unsigned long lastMeasurement = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);

  bmp280::setup();
  wifi::maintain_wifi_connection(WLAN_SSID, WLAN_PASS);
  bool reconnected = mqtt::maintain_mqtt_connection(mqttClient);
  mqtt::maintain_mqtt_subscriptions(mqttClient, mqtt_subscriptions, reconnected);
}

void process_mqtt_subscriptions() {
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqttClient.readSubscription(5000))) {
    if (subscription == &measure) {
      Serial.print(F("Got: "));
      Serial.println((char *)measure.lastread);
    }
  }
}

void loop() {
  wifi::maintain_wifi_connection(WLAN_SSID, WLAN_PASS);
  bool reconnected = mqtt::maintain_mqtt_connection(mqttClient);
  mqtt::maintain_mqtt_subscriptions(mqttClient, mqtt_subscriptions, reconnected);

  process_mqtt_subscriptions();

  long now = millis();
  if (now - lastMeasurement > 10000) {
    lastMeasurement = now;
  
    double temp = bmp280::bmp.readTemperature();
    double pres = bmp280::bmp.readPressure();
    int r = rain::measure(A0);

    Serial.println(r);
    Serial.println(temp, 2);
    Serial.println(pres, 2);
    
    rainWater.publish(r);
    pressure.publish(pres);
    temperature.publish(temp);
  }
}

