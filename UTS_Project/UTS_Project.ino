#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <BlynkSimpleEsp32.h>

char auth[] = "d7oMDiPN_fP1Fog6K19_meGEzWzx3cwM";
char ssid[] = "Aralez";
char pass[] = "15082000";

#define ldrPin 36
#define ledPin 23
#define ledPinSecond 22
BlynkTimer timer;
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
static float celciusSave, luxSave;
int btnState;

BLYNK_WRITE(V5)
{
  celciusSave = param.asFloat();
}

BLYNK_WRITE(V4)
{
  luxSave = param.asFloat();
}

BLYNK_WRITE(V8)
{
  btnState = param.asInt();
}

void sendCelcius()
{
  WidgetLED ledSuhu(V7);
  ledSuhu.on();
  float celcius = dht.readTemperature();
  Blynk.virtualWrite(V1, celcius);
  Blynk.virtualWrite(V3, celcius);
  Serial.print(F("Temperature: "));
  Serial.print(celcius);
  Serial.print("°C \n");
  if(celcius < celciusSave){
    digitalWrite(ledPinSecond, LOW);
    Blynk.setProperty(V7, "color", "#91D18B");
    Serial.println("Suhu Save");
  }else{
    digitalWrite(ledPinSecond, HIGH);
    Blynk.setProperty(V7, "color", "#D54062");
    Serial.println("Suhu Not Save");
  }
}

void sendLux()
{
  WidgetLED ledLux(V6);
  ledLux.on();
  float luxValue = analogRead(ldrPin);
  float luxConv = 0.009768 * luxValue + 10;
  Blynk.virtualWrite(V2, luxConv);
  Blynk.virtualWrite(V0, luxConv);
  Serial.println(luxValue);
  if(luxValue < luxSave){
    digitalWrite(ledPin, LOW);
    Blynk.setProperty(V6, "color", "#91D18B");
    Serial.println("Cahaya Save");
  }else{
    digitalWrite(ledPin, HIGH);
    Blynk.setProperty(V6, "color", "#D54062");
    Serial.println("Cahaya Not Save");
  }
}

void setup()
{
  Serial.begin(9600);
  
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,43,76), 8080);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinSecond, OUTPUT);
  timer.setInterval(1000L, sendLux);
  timer.setInterval(1000L, sendCelcius);
  dht.begin();
}

void loop()
{
  Blynk.run();
  if(btnState == 1)
  {
   timer.run(); 
  }
}
