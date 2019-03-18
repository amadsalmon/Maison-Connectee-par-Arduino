#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Servo serrure;

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("TEST");
  lcd.setCursor(2,1);
  lcd.print("VERROUILLER");
  serrure.attach(10);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}


void loop()
{
  serrure.write(50);
      lcd.setCursor(3,0);
  lcd.print("050");
  Serial.println("je dois écrire");
   delay(500);


  serrure.write(100);
    lcd.setCursor(3,0);
  lcd.print("100");
  delay(500);

  serrure.write(150);
      lcd.setCursor(3,0);
  lcd.print("150");
  delay(500);

  serrure.write(100);
      lcd.setCursor(3,0);
  lcd.print("100");
  delay(500); 
  
}
