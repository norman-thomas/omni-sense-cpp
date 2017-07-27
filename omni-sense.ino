#include <ESP8266WiFi.h>

#include <MQTTClient.h>

#include <set>
#include <map>

#include "wifi.h"
#include "mqtt.h"
#include "bme280.h"
#include "ppd42ns.h"
//#include "bmp280.h"
//#include "rain.h"

#include "credentials.h"

//#define MQTT_PREFIX_LOCATION ""
#define MQTT_PREFIX_ROOM "wohnzimmer"
#define MQTT_PREFIX MQTT_PREFIX_ROOM "/sensors"

#define MQTT_TEMPERATURE MQTT_PREFIX "/temperature"
#define MQTT_HUMIDITY MQTT_PREFIX "/humidity"
#define MQTT_PRESSURE MQTT_PREFIX "/pressure"

#define MQTT_DUST_RATIO MQTT_PREFIX "/dustRatio"
#define MQTT_DUST_CONCENTRATION MQTT_PREFIX "/dustConcentration"

//#define MQTT_RAIN MQTT_PREFIX "/rain"

#define MQTT_MEASURE "environment/measure"
#define MQTT_MEASURE_DUST "airquality/measure"

#define MIN_DELAY 0
#define DUST_SAMPLE_TIME_MS 30000
#define PPD_PIN D5

/*
 * Wemos D1 Mini
const int sclPin = D1;
const int sdaPin = D2;
*/

WiFiClient wifiClient;
MQTTClient mqttClient;

const std::vector<const char*> mqtt_subscriptions = { MQTT_MEASURE, MQTT_MEASURE_DUST };

unsigned long lastMeasurementTime = 0;
bme280::Measurement lastMeasurement_bme280;
ppd42ns::Measurement lastMeasurement_ppd42ns;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);

  char myName[10];
  sprintf(myName, "%d", ESP.getChipId());
  Serial.print("ESP Chip ID: ");
  Serial.println(ESP.getChipId());

  mqttClient.begin(MQTT_SERVER, MQTT_SERVERPORT, wifiClient);
  mqttClient.onMessage(process_mqtt_subscriptions);

  bme280::setup();
  wifi::maintain_wifi_connection(WLAN_SSID, WLAN_PASS);
  mqtt::maintain_connection(mqttClient, MQTT_PREFIX_ROOM, mqtt_subscriptions);
}

void process_mqtt_subscriptions(String &topic, String &payload) {
  if (topic.equals(MQTT_MEASURE)) {
    Serial.println("Got MQTT topic 'environment/measure'");
    do_weather();
  }
  else if (topic.equals(MQTT_MEASURE_DUST)) {
    Serial.println("Got MQTT topic 'airquality/measure'");
    do_dust();
  }
}

void do_weather() {
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

void do_dust() {
  long now = millis();
  if (now - lastMeasurementTime > MIN_DELAY) {
    lastMeasurementTime = now;
    lastMeasurement_ppd42ns = ppd42ns::measure(PPD_PIN, DUST_SAMPLE_TIME_MS);
  }

  Serial.print("dust ratio: ");
  Serial.println(lastMeasurement_ppd42ns.ratio, 2);
  Serial.print("dust concentration: ");
  Serial.println(lastMeasurement_ppd42ns.concentration, 2);
  
  mqttClient.publish(MQTT_DUST_RATIO, String(lastMeasurement_ppd42ns.ratio));
  mqttClient.publish(MQTT_DUST_CONCENTRATION, String(lastMeasurement_ppd42ns.concentration));
}

void loop() {
  wifi::maintain_wifi_connection(WLAN_SSID, WLAN_PASS);
  mqtt::maintain_connection(mqttClient, MQTT_PREFIX_ROOM, mqtt_subscriptions);

  mqttClient.loop();
}

