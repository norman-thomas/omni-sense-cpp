#include <ESP8266WiFi.h>

#include <MQTTClient.h>

#include <set>
#include <map>

#include "wifi.h"
#include "mqtt.h"
#include "bme280.h"
//#include "bmp280.h"
//#include "rain.h"

#include "credentials.h"

//#define MQTT_PREFIX_LOCATION ""
#define MQTT_PREFIX_ROOM "wohnzimmer"
#define MQTT_PREFIX MQTT_PREFIX_ROOM "/sensors"

#define MQTT_TEMPERATURE MQTT_PREFIX "/temperature"
#define MQTT_HUMIDITY MQTT_PREFIX "/humidity"
#define MQTT_PRESSURE MQTT_PREFIX "/pressure"
//#define MQTT_RAIN MQTT_PREFIX "/rain"

#define MQTT_MEASURE "environment/measure"

#define MIN_DELAY 0

/*
 * Wemos D1 Mini
const int sclPin = D1;
const int sdaPin = D2;
*/

WiFiClient wifiClient;
MQTTClient mqttClient;

const std::vector<const char*> mqtt_subscriptions = { MQTT_MEASURE };

unsigned long lastMeasurementTime = 0;
bme280::Measurement lastMeasurement_bme280;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);

  mqttClient.begin(MQTT_SERVER, MQTT_SERVERPORT, wifiClient);
  mqttClient.onMessage(process_mqtt_subscriptions);

  bme280::setup();
  wifi::maintain_wifi_connection(WLAN_SSID, WLAN_PASS);
  mqtt::maintain_connection(mqttClient, MQTT_PREFIX_ROOM, mqtt_subscriptions);
}

void process_mqtt_subscriptions(String &topic, String &payload) {
  if (topic.equals(MQTT_MEASURE)) {
    Serial.println("Got MQTT topic 'measure'");
    do_all();
  }
}

void do_all() {
  long now = millis();
  if (now - lastMeasurementTime > MIN_DELAY) {
    lastMeasurementTime = now;
    lastMeasurement_bme280 = bme280::measure();
  }

  Serial.print("temperature: ");
  Serial.println(lastMeasurement_bme280.temperature, 2);
  Serial.print("humidity: ");
  Serial.println(lastMeasurement_bme280.humidity, 2);
  Serial.print("pressure: ");
  Serial.println(lastMeasurement_bme280.pressure, 2);
  Serial.print("altitude: ");
  Serial.println(lastMeasurement_bme280.altitude, 2);
  
  //rainWater.publish(r);
  mqttClient.publish(MQTT_TEMPERATURE, String(lastMeasurement_bme280.temperature));
  mqttClient.publish(MQTT_HUMIDITY, String(lastMeasurement_bme280.humidity));
  mqttClient.publish(MQTT_PRESSURE, String(lastMeasurement_bme280.pressure));
}

void loop() {
  wifi::maintain_wifi_connection(WLAN_SSID, WLAN_PASS);
  mqtt::maintain_connection(mqttClient, MQTT_PREFIX_ROOM, mqtt_subscriptions);

  mqttClient.loop();
}

