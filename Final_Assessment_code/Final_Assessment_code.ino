#include<PinChangeInterrupt.h>
#include<LiquidCrystal.h>
#include<MsTimer2.h>
#include<Wire.h>

String Task_info[2][4]={"Task1: spin","Status: run", "Speed: ","", "Task2: square","Status: run", "Speed: ",""};
const String Task_name[3]={"","Task1: Spin","Task2: Square"};
int dir=0, sta=0, Task=0, PressTime=0, buzzer_sta=0, LED_sta=0;
int encoderValue=0;
long position=0;
const int rs = 8, enable = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7,
          Pressbutton_Pin = 2, Startbutton_Pin = 3, buzzer = 10, Microphone=A0, Slide_Switch = A1, 
          ENCODER_A_PIN = 11, ENCODER_B_PIN = 12, LED_Pin = 13;

LiquidCrystal lcd(rs,enable,d4,d5,d6,d7);

void setup() 
{
  pinMode(Pressbutton_Pin,INPUT_PULLUP);
  pinMode(Startbutton_Pin,INPUT_PULLUP);
  pinMode(Slide_Switch, INPUT_PULLUP);
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);
  pinMode(LED_Pin,OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  MsTimer2::set(1, Timer2ISR);
  attachPinChangeInterrupt(digitalPinToPCINT(Pressbutton_Pin), com, RISING);
  attachPinChangeInterrupt(digitalPinToPCINT(ENCODER_A_PIN), read_quadrature, CHANGE);

  Serial.begin(9600);
  lcd.begin(2,16);
  lcd.print("Press button");
  lcd.setCursor(0,1);
  lcd.print("start");
}

void com()
{
  PressTime++;
  if (PressTime==3) {Task=0; PressTime=1;}
  if (PressTime==1) {Task++; LCD_print();}
  if (PressTime==2) {Task++; LCD_print();}
}

void read_quadrature()
{  
  if (digitalRead(ENCODER_A_PIN) == LOW){   
    if (digitalRead(ENCODER_B_PIN) == LOW)
      position++;
  }
  else{
    if (digitalRead(ENCODER_B_PIN) == LOW)
      position--;
  }
  Serial.println(position);
}

void Timer2ISR()
{
  static unsigned int counter = 0;
  static byte state = LOW;
  if (counter < 2400)
  {
    counter++;
    if (Task==1) digitalWrite(LED_Pin, HIGH), digitalWrite(buzzer, HIGH);
    if (Task==2&&counter%250==0) LED_change();
    if (Task==2&&counter%250==0) buzzer_change();
    if (counter%1200==0) LCD_change();
  }
  else counter=0;
}

void buzzer_change()
{
  static byte state = LOW;
  if (buzzer_sta==LOW) digitalWrite(buzzer, HIGH);
  else digitalWrite(buzzer, LOW);
  buzzer_sta=!buzzer_sta;
}

void LCD_change()
{
  static byte Task_page = LOW;
  if (Task_page == LOW)
  {
    lcd.clear();
    lcd.print(Task_info[Task-1][0]);
    lcd.setCursor(0,1);
    lcd.print(Task_info[Task-1][1]);
  }
  else
  {
    lcd.clear();
    lcd.print(Task_info[Task-1][2]);
    lcd.print(position,DEC); lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print(Task_info[Task-1][3]);
  }
  Task_page=!Task_page;
}

void LED_change()
{
  static byte state = LOW;
  if (LED_sta==LOW) digitalWrite(LED_Pin,HIGH);
  else digitalWrite(LED_Pin,LOW);
  LED_sta=!LED_sta;
}

void LCD_print()
{
  lcd.clear();
  lcd.print(Task_name[Task]);
  lcd.setCursor(0,1);
  if (sta==0) lcd.print("Status: run");
  else lcd.print("Status: stop");
}

void judge_direction()
{
  if (dir==0) Task_info[Task-1][3]="Direction: For";
  else Task_info[Task-1][3]="Direction: Bac";
}

void judge_speed()
{
  long sum = 0;
  position=0;
  while(sum<=350)
  {
    sum = 0;
    for(int n=0; n<100; n++) sum += analogRead(Microphone);
    sum = sum/100;
    lcd.clear();
    lcd.print("Speed set: ");
    lcd.print(position,DEC);
    delay(100);
  }
}

void reset()
{
  sta=1;
  LCD_print();
  digitalWrite(LED_Pin,LOW);
}

void loop() 
{
  sta=digitalRead(Startbutton_Pin);
  dir=digitalRead(Slide_Switch);
  if (Task==1&&sta==0)
  {
    LCD_print(); delay(1000);
    judge_speed(); judge_direction();
    MsTimer2::start();
    Task1();
    MsTimer2::stop();
    reset();
  }
  if (Task==2&&sta==0)
  {
    LCD_print(); delay(1000);
    judge_speed(); judge_direction();
    MsTimer2::start();
    Task2();
    MsTimer2::stop();
    reset();
  }
  delay(10);
}

void Task1()
{
  int target, Spin=450, i=0;
  target = 0x80000000 + Spin;
  Wire.beginTransmission(42);
  Wire.write("d1r"); delay(10);
  Wire.write("d3r"); delay(10);
  Wire.write("sa"); delay(10);
  for(i=0;i<=3;i++) Wire.write(46),Wire.write(0);
  Wire.endTransmission();
  while (encoderValue < target) 
  {
    encoderValue = readencoder();
    delay(20);
  }
  Wire.beginTransmission(42);
  Wire.write("ha");
  Wire.endTransmission();
}

void Task2()
{
  int target, Forward=2500, Turn=450, i=0, j=0;
  for(i=0;i<=3;i++)
  {
    Wire.beginTransmission(42);  //straight
    Wire.write("d2f"), delay(10);
    Wire.write("d4f"), delay(10);
    Wire.write("sa"), delay(10);
    for(j=0;j<=3;j++) Wire.write(20), Wire.write(0);
    Wire.endTransmission();
    target=0x80000000 + Forward;
    while (encoderValue < target) encoderValue = readencoder(), delay(20);
    
    Wire.beginTransmission(42); //turn
    Wire.write("d2r"); delay(10);
    Wire.write("d4r"); delay(10);
    Wire.write("sa"); delay(10);
    for(j=0;j<=3;j++) Wire.write(41), Wire.write(0);
    Wire.endTransmission();
    target=0x80000000 + Turn;
    while (encoderValue < target) encoderValue = readencoder(), delay(20);
    Wire.beginTransmission(42);
    Wire.write("ha");
    Wire.endTransmission();
    delay(50);
  }
}

int readencoder()
{
  Wire.beginTransmission(42);
  Wire.write("i");
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(42,8);
  delay(10);
  if(Wire.available()==8)
  {
    encoderValue = (long unsigned int) Wire.read();
    encoderValue += ((long unsigned int) Wire.read() <<8);
    encoderValue += ((long unsigned int) Wire.read() <<16);
    encoderValue += ((long unsigned int) Wire.read() <<24);
  }
}
