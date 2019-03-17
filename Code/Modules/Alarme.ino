int buzzer =11; // Borne de sortie

void setup(){
  pinMode(buzzer,OUTPUT);// Définir la sortie du buzzer
}

void loop(){
 
  tone(buzzer, 2300); 
  delay(200);        
  tone(buzzer, 1700);
  //noTone(buzzer);    
  delay(200);       
  
}

//void frequence(int frequence){
//    digitalWrite(buzzer,HIGH);// Faire du bruit
//    delay(frequence);// Attendre 20ms
//    digitalWrite(buzzer,LOW);// Silence
//    delay(frequence);// Attendre 20ms
//}
