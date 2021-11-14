
#include<LiquidCrystal.h>
LiquidCrystal lcd(13,12,2,3,4,5);
void setup(){
    lcd.begin(16,2);
}
void loop(){
    lcd.clear();
    /**
     * 温度
     */
    lcd.setCursor(0,0);
    lcd.print("Tem(");
    lcd.print((char)223);//显示o符号
    lcd.print("C):");
    lcd.setCursor(8,0);
    lcd.print(32.00);
    /**
     * 湿度
     */
    lcd.setCursor(0,1);
    lcd.print("Hum(");
    lcd.print((char)37);//显示%符号
    lcd.print("):");
    lcd.setCursor(7,1);
    lcd.print(32);
    /**
     * 天数
     */
    lcd.setCursor(10,1);
    lcd.print("Day:");
    lcd.setCursor(14,1);
    lcd.print(22);
    delay(5000);
}
