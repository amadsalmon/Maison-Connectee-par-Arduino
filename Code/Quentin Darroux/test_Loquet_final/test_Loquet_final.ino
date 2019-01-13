#include<Servo.h>;
Servo Loquet;
bool verrouillee = true;
int angle; // Utile lors du controle de l'orientation du Loquet.
void setup() {

Loquet.attach(9);
Loquet.write(0);
}

void loop() {
  
if (verrouillee==true){
      Serial.println("SYSTÈME EN COURS DE VERROUILLAGE");

          angle = Loquet.read();
          if ( angle ==0){
            Serial.println("LE SYSTEM EST DEJA VEROUILLE");}
          Loquet.write(0);
          delay(500);
          Serial.println("SYSTÈME VERROUILLÉ");
      }
      
    else{
          Serial.println("SYSTÈME EN COURS DE DÉVERROUILLAGE");


          Loquet.write(90);
          delay(500);
          Serial.println("SYSTÈME DÉVERROUILLÉ");
      }
  if ( verrouillee == true){
    verrouillee = false;
  }
  else{
    verrouillee = true;}
  
}
