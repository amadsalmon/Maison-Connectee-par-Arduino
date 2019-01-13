long TimeIni;
long TimeNow;
bool Detect = true;
bool intrusion;
bool attenteIdentification;
void setup() {
}

void loop() {
if (Detect == true){
  TimeIni= millis();
  while(Detect){
    TimeNow=millis();
    if(TimeNow - TimeIni >= 30000){
      //allumer la sirene)
      Serial.println( "INTRU DETECTE");
      intrusion = HIGH; }
     else{
      return attenteIdentification = HIGH;
     }
  }
}

}
