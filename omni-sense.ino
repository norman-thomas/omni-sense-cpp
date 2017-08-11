#include <ESP8266WiFi.h>

#include <MQTTClient.h>

#include <set>
#include <map>

//#define INCLUDE_DHT22
//#define INCLUDE_BME280
//#define INCLUDE_BMP280
//#define INCLUDE_PPD42NS
//#define INCLUDE_RAIN
#define INCLUDE_SI7021
#define INCLUDE_TSL2561
//#define INCLUDE_TSL2591

#include "wifi.h"
#include "mqtt.h"
#include "utils.h"

#ifdef INCLUDE_BME280
  #include "bme280.h"
#endif
#ifdef INCLUDE_BMP280
  #include "bmp280.h"
#endif
#ifdef INCLUDE_SI7021
  #include "si7021.h"
#endif
#ifdef INCLUDE_TSL2561
  #include "tsl2561.h"
#endif
#ifdef INCLUDE_TSL2591
  #include "tsl2591.h"
#endif
#ifdef INCLUDE_PPD42NS
  #include "ppd42ns.h"
#endif
#ifdef INCLUDE_RAIN
  #include "rain.h"
#endif

#include "credentials.h"

#define MQTT_PREFIX_LOCATION ""
#define MQTT_PREFIX_ROOM "wohnzimmer"
#define MQTT_PREFIX MQTT_PREFIX_LOCATION MQTT_PREFIX_ROOM "/sensors"

#define MQTT_TEMPERATURE MQTT_PREFIX "/temperature"
#define MQTT_HUMIDITY MQTT_PREFIX "/humidity"
#define MQTT_PRESSURE MQTT_PREFIX "/pressure"
#define MQTT_LUMINOSITY MQTT_PREFIX "/lux"
#define MQTT_VISIBLE MQTT_PREFIX "/visible"
#define MQTT_IR MQTT_PREFIX "/ir"
#define MQTT_UV MQTT_PREFIX "/uv"


#define MQTT_JSON MQTT_PREFIX "/json"
#define MQTT_ENVIRONMENT MQTT_JSON "/environment"
#define MQTT_AIRQUALITY MQTT_JSON "/airquality"

#define MQTT_DUST_RATIO MQTT_PREFIX "/dustRatio"
#define MQTT_DUST_CONCENTRATION MQTT_PREFIX "/dustConcentration"

#define MQTT_RAIN MQTT_PREFIX "/rain"

#define MQTT_MEASURE_ENVIRONMENT "triggers/measure/environment"
#define MQTT_MEASURE_DUST "triggers/measure/airquality"
#define MQTT_MEASURE_SCHEDULE MQTT_MEASURE_ENVIRONMENT "/set_interval"
#define MQTT_MEASURE_SCHEDULE_DUST MQTT_MEASURE_DUST "/set_interval"

#define MIN_DELAY 0
#define DUST_SAMPLE_TIME_MS 30000
#define PPD_PIN D5

/*
 * Wemos D1 Mini
const int sclPin = D1;
const int sdaPin = D2;
*/

String mqtt_client_id = "";

WiFiClient wifiClient;
MQTTClient mqttClient(512);

const std::vector<const char*> mqtt_subscriptions = {
  MQTT_MEASURE_ENVIRONMENT,
  MQTT_MEASURE_SCHEDULE
  #ifdef INCLUDE_PPD42NS
  , MQTT_MEASURE_DUST
  , MQTT_MEASURE_SCHEDULE_DUST
  #endif
};

std::map<String, String> environment, airquality;

unsigned long lastEnvironmentTime = 0;
unsigned long lastDustTime = 0;

unsigned long lastCronTime = 0;
unsigned long lastDustCronTime = 0;

long scheduled = 60; // default: 60sec
long scheduledDust = 300; // default: 300sec

#ifdef INCLUDE_BME280
  bme280::Measurement lastMeasurement_bme280;
#endif
#ifdef INCLUDE_SI7021
  si7021::Measurement lastMeasurement_si7021;
#endif
#ifdef INCLUDE_TSL2561
  tsl2561::Measurement lastMeasurement_tsl2561;
#endif
#ifdef INCLUDE_TSL2591
  tsl2591::Measurement lastMeasurement_tsl2591;
#endif
#ifdef INCLUDE_PPD42NS
  ppd42ns::Measurement lastMeasurement_ppd42ns;
#endif

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);

  mqtt_client_id = String(MQTT_PREFIX_ROOM) + "-" + String(ESP.getChipId());
  Serial.print("ESP Chip ID: ");
  Serial.println(ESP.getChipId());

  mqttClient.begin(MQTT_SERVER, MQTT_SERVERPORT, wifiClient);
  mqttClient.onMessage(process_mqtt_subscriptions);

#ifdef INCLUDE_BME280
  bme280::setup();
#endif
#ifdef INCLUDE_SI7021
  si7021::setup();
#endif
#ifdef INCLUDE_TSL2591
  tsl2591::setup();
#endif
#ifdef INCLUDE_TSL2561
  tsl2561::setup();
#endif

  maintain_connections();

  Serial.println("Switching off internal LED");
  digitalWrite(BUILTIN_LED, HIGH);

  Serial.println("Performing an initial measurement");
  do_basic();
  do_dust();
}

void process_mqtt_subscriptions(String &topic, String &payload) {
  if (topic.equals(MQTT_MEASURE_ENVIRONMENT)) {
    Serial.println("Got MQTT topic 'triggers/measure/environment'");
    do_basic();
  }
  else if (topic.equals(MQTT_MEASURE_DUST)) {
    Serial.println("Got MQTT topic 'triggers/measure/airquality'");
    do_dust();
  }
  else if (topic.equals(MQTT_MEASURE_SCHEDULE)) {
    Serial.println("Got MQTT topic 'triggers/measure/environment/set_interval'");
    do_basic();
    schedule(scheduled, payload);
  }
  else if (topic.equals(MQTT_MEASURE_SCHEDULE_DUST)) {
    Serial.println("Got MQTT topic 'triggers/measure/airquality/set_interval'");
    do_dust();
    schedule(scheduledDust, payload);
  }
}

void schedule(long &seconds, String &payload) {
  if (payload.length() == 0 || payload.toInt() <= 0) {
    seconds = 0;
    Serial.println("Disabled scheduling");
  }
  else if (payload.toInt() > 0) {
    seconds = payload.toInt();
    Serial.print("Enabling scheduling, interval (secs) = ");
    Serial.println(seconds);
  }
}

void do_basic() {
  unsigned long now = millis();
  if (now - lastEnvironmentTime > MIN_DELAY) {
    lastEnvironmentTime = now;
#ifdef INCLUDE_BME280
    lastMeasurement_bme280 = bme280::measure(environment);
#endif
#ifdef INCLUDE_SI7021
    lastMeasurement_si7021 = si7021::measure(environment);
#endif
#ifdef INCLUDE_TSL2561
    lastMeasurement_tsl2561 = tsl2561::measure(environment);
#endif
#ifdef INCLUDE_TSL2591
    lastMeasurement_tsl2591 = tsl2591::measure(environment);
#endif
  }

  String json = utils::toJSON(environment);
  if (json.length() > 0) {
    mqttClient.publish(MQTT_ENVIRONMENT, json, false, 1);
  }

#ifdef INCLUDE_BME280
  Serial.print("temperature: ");
  Serial.println(lastMeasurement_bme280.temperature, 2);
  Serial.print("humidity: ");
  Serial.println(lastMeasurement_bme280.humidity, 2);
  Serial.print("pressure: ");
  Serial.println(lastMeasurement_bme280.pressure, 2);
  Serial.print("altitude: ");
  Serial.println(lastMeasurement_bme280.altitude, 2);

  mqttClient.publish(MQTT_TEMPERATURE, String(lastMeasurement_bme280.temperature), true, 1);
  mqttClient.publish(MQTT_HUMIDITY, String(lastMeasurement_bme280.humidity), true, 1);
  mqttClient.publish(MQTT_PRESSURE, String(lastMeasurement_bme280.pressure), true, 1);
  mqttClient.publish(MQTT_PRESSURE, String(lastMeasurement_bme280.pressure), true, 1);
#endif

#ifdef INCLUDE_SI7021
  Serial.print("temperature: ");
  Serial.println(lastMeasurement_si7021.temperature, 2);
  Serial.print("humidity: ");
  Serial.println(lastMeasurement_si7021.humidity, 2);

  //rainWater.publish(r);
  mqttClient.publish(MQTT_TEMPERATURE, String(lastMeasurement_si7021.temperature), true, 1);
  mqttClient.publish(MQTT_HUMIDITY, String(lastMeasurement_si7021.humidity), true, 1);
#endif

#ifdef INCLUDE_TSL2561
  Serial.print("lux: ");
  Serial.println(lastMeasurement_tsl2561.lux);
  Serial.print("visible: ");
  Serial.println(lastMeasurement_tsl2561.visible);
  Serial.print("IR: ");
  Serial.println(lastMeasurement_tsl2561.ir);

  mqttClient.publish(MQTT_LUMINOSITY, String(lastMeasurement_tsl2561.lux), true, 1);
  mqttClient.publish(MQTT_VISIBLE, String(lastMeasurement_tsl2561.visible), true, 1);
  mqttClient.publish(MQTT_IR, String(lastMeasurement_tsl2561.ir), true, 1);
#endif

#ifdef INCLUDE_TSL2591
  Serial.print("lux: ");
  Serial.println(lastMeasurement_tsl2591.lux);
  Serial.print("visible: ");
  Serial.println(lastMeasurement_tsl2591.visible);
  Serial.print("infrared: ");
  Serial.println(lastMeasurement_tsl2591.ir);

  mqttClient.publish(MQTT_LUMINOSITY, String(lastMeasurement_tsl2591.lux), true, 1);
  mqttClient.publish(MQTT_VISIBLE, String(lastMeasurement_tsl2591.visible)), true, 1;
  mqttClient.publish(MQTT_IR, String(lastMeasurement_tsl2591.ir), true, 1);
#endif
}

void do_dust() {
  unsigned long now = millis();
  if (now - lastDustTime > MIN_DELAY) {
    lastDustTime = now;
#ifdef INCLUDE_PPD42NS
    lastMeasurement_ppd42ns = ppd42ns::measure(PPD_PIN, DUST_SAMPLE_TIME_MS, airquality);
#endif
  }

  String json = utils::toJSON(airquality);
  if (json.length() > 0) {
    mqttClient.publish(MQTT_AIRQUALITY, json, false, 1);
  }

#ifdef INCLUDE_PPD42NS
  Serial.print("dust ratio: ");
  Serial.println(lastMeasurement_ppd42ns.ratio, 2);
  Serial.print("dust concentration: ");
  Serial.println(lastMeasurement_ppd42ns.concentration, 2);
  
  mqttClient.publish(MQTT_DUST_RATIO, String(lastMeasurement_ppd42ns.ratio), true, 1);
  mqttClient.publish(MQTT_DUST_CONCENTRATION, String(lastMeasurement_ppd42ns.concentration), true, 1);
#endif
}

void maintain_connections() {
  wifi::maintain_wifi_connection(WLAN_SSID, WLAN_PASS);
  mqtt::maintain_connection(mqttClient, mqtt_client_id.c_str(), mqtt_subscriptions);
}

void loop() {
  maintain_connections();

  mqttClient.loop();
  delay(10);

  if (scheduled > 0) {
    unsigned long now = millis();
    if (now - lastCronTime >= scheduled * 1000) {
      Serial.println("Running scheduled environment measurement");
      lastCronTime = now;
      do_basic();
    }
  }

#ifdef PPD42NS
  if (scheduledDust > 0) {
    unsigned long now = millis();
    if (now - lastDustCronTime >= scheduledDust * 1000) {
      Serial.println("Running scheduled airquality measurement");
      lastDustCronTime = now;
      do_dust();
    }
  }
#endif
}

