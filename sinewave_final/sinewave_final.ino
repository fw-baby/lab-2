#include<MsTimer2.h>
#define FREQ_CTL (500)
float t=0.0;
float curve;
float f=0.4;
int f1,f2;


void Init_time2(void);

void Timer2ISR()
{
  t=t+0.01;
  curve=(float)(50*(sin(2*PI*f*t)+1));
  analogWrite (6, curve);
  Serial.println (curve);
}

inline void Init_time2(void)
{
  MsTimer2::set(8000/FREQ_CTL, Timer2ISR);
  MsTimer2::start();
}


void setup()
{
noInterrupts();
Init_time2();
pinMode(6,OUTPUT);
interrupts ();
delay (100);
Serial.begin (9600);
//  Serial.println("frequency is:");
}

void loop()
{
////if (Serial.available()==2)
////f1=(int)Serial.read()-48;
////f1=-(Serial.read()-48+4.4);
//delay(10);
//f2=(int)Serial.read()-48;
////f2=-(Serial.read()-48-2);
//Serial.read();
//f=((double)f1+(double)f2/10)/10;
////Serial.println(f);
////Serial.println("frequency is:");
}

void serialEvent()
{
  MsTimer2::stop();
  float inchar = 0;
  f = 0; 
  delay(10);
   if(Serial.available())
 {   
      while((inchar = Serial.read())!= -1 )
      {   
        if( inchar <= '9' && inchar >= '0')
            f = (float)(f * 10 + inchar - '0');
       }
        f = (float)f/10;
        Serial.println("frequency is:");
        Serial.println(f,5);
        Init_time2();
   }
}
