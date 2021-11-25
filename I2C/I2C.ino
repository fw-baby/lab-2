#include "Wire.h"
#define address 0x44

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  }

void loop() {
  // put your main code here, to run repeatedly:
  char val = 'a';
  Wire.beginTransmission(address);
  Wire.write(val);
  Wire.endTransmission();
  delay(100);
 }
