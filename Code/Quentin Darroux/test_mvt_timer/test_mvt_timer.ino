/*
 * PIR sensor tester
 */
int ledPin = 3;
int buzzer = 11;
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = LOW;                    // variable for reading the pin status
 
void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
 
  Serial.begin(9600);
}
 
void loop(){
  val = digitalRead(inputPin);  // read input value
  Serial.println("ETape 1");
  if (timer(val)) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH); // turn LED ON
    sireneAlarme(true,buzzer);
    Serial.println("Mouvement détecté !");
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Mouvement détecté !");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } 

  else { 
    Serial.println("pas de mvt");
    digitalWrite(ledPin, LOW); // turn LED OFF
    sireneAlarme(false,buzzer);
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Mouvement stoppé !");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }}

void sireneAlarme(bool etat, int buzzer){
  if(etat){
      tone(buzzer, 2300); 
      delay(200);        
      tone(buzzer, 1700);    
      delay(200);
      //noTone(buzzer);
  }
  else{
    noTone(buzzer);
    }
    
  }

//-----------------------Definition du Timer-------------------------
long TimeIni;
long TimeNow;
bool Detect = true;
bool intrusion;
bool attenteIdentification; 

  bool timer(int Detect) {
if (Detect == HIGH){
  TimeIni= millis();
  while(Detect== HIGH){
    Serial.println("je detetce un truc");
    TimeNow=millis();
    Serial.println(TimeNow);
    if(TimeNow - TimeIni >= 30000){
      //allumer la sirene)
      Serial.println( "INTRU DETECTE");
      return true; }
     else{
      return false;
     }
     Detect = digitalRead(inputPin);
  }
}}
//---------------------------------------------------------------------