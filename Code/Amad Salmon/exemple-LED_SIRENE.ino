//
// AMAD SALMON
//

// DÉFINITION DES INDICATEURS
int buzzer =13; // BUZZER (SIRÈNE D'ALARME)
int ledRouge = 7; // LED ROUGE
int ledOrange = 8; // LED ORANGE
int ledVerte = 9; // LED VERTE
void setup() {
  // put your setup code here, to run once:
   pinMode(ledRouge,OUTPUT);
   pinMode(ledOrange,OUTPUT);
   pinMode(ledVerte,OUTPUT);
 
   pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(buzzer, 2300);
  digitalWrite(ledRouge,HIGH); 
  digitalWrite(ledOrange,LOW);
  delay(200);        
  tone(buzzer, 1700);
  digitalWrite(ledRouge,LOW); 
  digitalWrite(ledOrange,HIGH);
  //noTone(buzzer);    
  delay(200);  
}
