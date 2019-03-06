#include<SoftwareSerial.h> 
#define RX 7
#define TX 6
SoftwareSerial BlueT(RX,TX);
char Data;
char DIGIT;
String CODE;

void setup() {
  Serial.begin(9600); 
  delay(500);
  BlueT.begin(9600);
  Serial.println("Bonjour -Pret pour les commandes AT");
  delay(500); 
}

void loop(){
  if (BlueT.available()){
    Data = BlueT.read(); 
     //if (Data=='A') {
      DIGIT=BlueT.parseInt();
      CODE += DIGIT; 
      Serial.println(CODE); 
      //} 
    }
}
