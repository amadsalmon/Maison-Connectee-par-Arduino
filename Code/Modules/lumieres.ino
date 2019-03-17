#include <SoftwareSerial.h>

const int lumiere1 = 9;
const int lumiere2 = 10;
int luminosite=1;

const int buzzer = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(lumiere1, OUTPUT);
  pinMode(lumiere2, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  clignoterLumieres();
  //sonnetteSON();
  erreurPassword_SON();

   
}

void lumieres(int luminosite) // luminosité de 0 à 100
{
  //int luminosite_255 = 255*luminosite/100; //conversion de la luminositée/100 à luminosité/255 pour le PWM
  int luminosite_255 = luminosite;
  analogWrite(lumiere1,luminosite_255);
  analogWrite(lumiere2,luminosite_255);
  }
 void clignoterLumieres(){
    digitalWrite(lumiere1,HIGH);
    digitalWrite(lumiere2,HIGH);
    delay(200);
    digitalWrite(lumiere1,LOW);
    digitalWrite(lumiere2,LOW);
    delay(200);
    digitalWrite(lumiere1,HIGH);
    digitalWrite(lumiere2,HIGH);
    delay(200);
    digitalWrite(lumiere1,LOW);
    digitalWrite(lumiere2,LOW);
    delay(200);
    digitalWrite(lumiere1,HIGH);
    digitalWrite(lumiere2,HIGH);
    delay(200);
    digitalWrite(lumiere1,LOW);
    digitalWrite(lumiere2,LOW);
    delay(200);
  }

  void sonnetteSON(){
  tone(buzzer, 2000); 
  delay(500);        
  tone(buzzer, 1500);
  delay(600); 
  noTone(buzzer);      
  }

  void erreurPassword_SON(){
    tone(buzzer, 100); 
    delay(50);
    noTone(buzzer); 
    delay(45);         
    tone(buzzer, 100);
    delay(70); 
    noTone(buzzer);
    }
