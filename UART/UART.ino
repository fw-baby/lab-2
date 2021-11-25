#include <SoftwareSerial.h>

#define rxPin 2
#define txPin 3

SoftwareSerial setSerial(rxPin,txPin);

void setup() {
  // put your setup code here, to run once:
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  
  Serial.begin(9600);

  setSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  setSerial.print('a');
  char c = (char)setSerial.read();
  Serial.println(c);
  delay(100);
}
