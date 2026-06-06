#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Arduino.h>

MPU6050 mpu;

// WiFi credentials
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// HiveMQ Cloud credentials
const char* mqtt_server = "YOUR_CLUSTER.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "765KV";
const char* mqtt_pass = "transistor";

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      // connected
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  mpu.initialize();

  setup_wifi();

  espClient.setInsecure(); // skip certificate validation (for testing)

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float gForce = sqrt(ax*ax + ay*ay + az*az) / 16384.0;

  // JSON payload
  String payload = "{";
  payload += "\"ax\":" + String(ax) + ",";
  payload += "\"ay\":" + String(ay) + ",";
  payload += "\"az\":" + String(az) + ",";
  payload += "\"g\":" + String(gForce);
  payload += "}";

  client.publish("vehicle/crash/data", payload.c_str());

  Serial.println(payload);

  delay(1000);
}