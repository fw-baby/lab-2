int ledPin=10;
int buzzer=7;
int Microphone = A0                                                                                                                                         ;
 
void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(Microphone, INPUT);
 
  Serial.begin(57600);
  
}
 
void loop()
{   
    long sum = 0;
    for(int i=0; i<100; i++)
    {
        sum += analogRead(Microphone);
    }
 
    sum = sum/100;
 
    Serial.println(sum);

  while (sum<=35){
    for(;;){
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin,LOW);
      delay(500);
      
    //continue;
    }
  }
 
}
