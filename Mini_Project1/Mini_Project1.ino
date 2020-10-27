// Mengatur ledPin menagarah ke pin nomor berapa
const int ledRed =  23;
const int ledYellow = 22;
const int ledGreen = 1;

//Lalu membuat variable untuk menampung waktu terakhir led dinyalakan
unsigned long previousMillis = 0;

//Mengatur interval dari setiap led
const long intRed = 5000;
const long intYellow = 2000;
const long intGreen = 3000;

int switchLed = 0;

//Atur pin diatas menjadi output
void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if(switchLed == 0){
    digitalWrite(ledRed,LOW);
    digitalWrite(ledYellow,HIGH);
    digitalWrite(ledGreen,HIGH);
    switchLed++;
    previousMillis += intRed;
  }
  else{
    if(switchLed == 1 && previousMillis - currentMillis == 0){
      digitalWrite(ledRed,HIGH);
      digitalWrite(ledYellow,LOW);
      switchLed++;
      previousMillis += intYellow;
    }
    else if(switchLed == 2 && previousMillis - currentMillis == 0){
      switchLed++;
      digitalWrite(ledYellow,HIGH);
      digitalWrite(ledGreen,LOW);
      previousMillis += intGreen;
    }
    else if(switchLed == 3 && previousMillis - currentMillis == 0){
      digitalWrite(ledRed,LOW);
      digitalWrite(ledGreen,HIGH);
      switchLed = 1;
      previousMillis += intRed;
    }
  }
}
