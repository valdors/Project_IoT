#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

const int ledPinFirst = 23;
const int ledPinSecond = 22;
const int ledPinThird = 19;
const int btnPin = 0;
int btnState=0;
float tempCelcius;

void setup() {
  pinMode(ledPinFirst, OUTPUT);
  pinMode(ledPinSecond, OUTPUT);
  pinMode(ledPinThird, OUTPUT);
  pinMode(btnPin, INPUT);
  digitalWrite(ledPinFirst, HIGH);
  digitalWrite(ledPinSecond, HIGH);
  digitalWrite(ledPinThird, HIGH);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  btnState = digitalRead(btnPin);
  if(btnState == LOW){
    Serial.println("ON");
    delay(2000);
    tempDetect();
  }else{
    Serial.println("OFF");
    digitalWrite(ledPinFirst, HIGH);
    digitalWrite(ledPinSecond, HIGH);
    digitalWrite(ledPinThird, HIGH);
    delay(2000);
  }
  delay(100);
}

void tempDetect(){
    //memberikan delay 2 detik untuk memulai pengukuran suhu
    delay(2000);
    //membaca temperatur menjadi celcius
    tempCelcius = dht.readTemperature();
    //mengecek apakah gagal mengukur suhu
    if(isnan(tempCelcius)){
      Serial.println("gagal mengukur suhu");
      Serial.println("kembali melakukan pengukuran mengukur");
      tempDetect();
    }
    //jika sukses mengukur suhu
    else{
      Serial.print(F("Temperature: "));
      Serial.print(tempCelcius);
      Serial.print("°C \n");
      checkTemp();
      delay(2000);
      //memberi delay 2 detik untuk mengecek apakah button kembali ditekan atau tidak
    }
}

//method untuk mengecek suhu dan menyalakan led
void checkTemp(){
  delay(500);
  if(tempCelcius < 35){
    digitalWrite(ledPinFirst,LOW);
    digitalWrite(ledPinSecond, HIGH);
    digitalWrite(ledPinThird, HIGH);
    Serial.println("Aman");
  }
  else if(tempCelcius >= 35.00 && tempCelcius <= 50.00){
    digitalWrite(ledPinFirst,LOW);
    digitalWrite(ledPinSecond,LOW);
    digitalWrite(ledPinThird, HIGH);
    Serial.println("Siaga");
  }
  else  if(tempCelcius > 50.00){
    digitalWrite(ledPinFirst,LOW);
    digitalWrite(ledPinSecond,LOW);
    digitalWrite(ledPinThird,LOW);
    Serial.println("Darurat");
  }
}
