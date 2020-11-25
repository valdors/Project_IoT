#include <WiFi.h>
#include <ESP32Servo.h>
#include <FirebaseESP32.h>

#define FIREBASE_HOST "https://mini-project4.firebaseio.com/"
#define FIREBASE_AUTH "PqfPtmYK4e8QjnhOq3QfMVMavYd74Su3JVaiL8u9"
#define servoPin 23
#define ldrPin 36

FirebaseData firebaseData;

const char* ssid = "Aralez";
const char* password = "15082000";

Servo servo1;
int static posDegrees = 180;
float adrLuxValue = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

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

void setup(){
  Serial.begin(9600);
  setup_wifi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(1000);
  servo1.attach(servoPin);
}

void loop() {
  Firebase.getString(firebaseData,"/statusButton/strings");
  String btnState = firebaseData.stringData();
  delay(100);
  if (btnState == "ON") {
      adrLuxValue = analogRead(ldrPin);
      if(adrLuxValue > 1000){
        Serial.print("ADR = ");
        Serial.println(adrLuxValue);
        for(; posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
        delay(1);
        }
      }else if(adrLuxValue <= 1000){
        Serial.print("ADR = ");
        Serial.println(adrLuxValue);
        for(; posDegrees <= 180; posDegrees++) {
        servo1.write(posDegrees);
        delay(5);
        }
      }
  String sendData = (String)adrLuxValue;
  Firebase.setString(firebaseData,"/adrLuxValue/strings" , sendData);
  delay(1000);   
  }
}
