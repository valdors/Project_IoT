#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

const char* ssid = "Aralez";
const char* password = "15082000";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
#define servoPin 23
#define ldrPin 36
int static posDegrees = 180;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
Servo servo1;
int adrLuxValue = 0;
char payL;

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
   payL = (char)payload[0];
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      client.subscribe("ButtonOnOff");
    } else {
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  servo1.attach(servoPin);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (payL == '1') {
      adrLuxValue = analogRead(ldrPin);
      if(adrLuxValue > 2000){
        Serial.print("ADR = ");
        Serial.println(adrLuxValue);
        for(; posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
        delay(1);
        }
      }else if(adrLuxValue <= 2000){
        Serial.print("ADR = ");
        Serial.println(adrLuxValue);
        for(; posDegrees <= 180; posDegrees++) {
        servo1.write(posDegrees);
        delay(5);
        }
      }
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    snprintf (msg, MSG_BUFFER_SIZE, "%d", adrLuxValue);
    client.publish("adrLux", msg);
  }
  delay(1000);   
  }
}
