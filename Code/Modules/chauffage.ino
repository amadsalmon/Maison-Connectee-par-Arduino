#include <Adafruit_NeoPixel.h>
#include<SoftwareSerial.h> 

Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, 5, NEO_GRB + NEO_KHZ800);

// ---------- DÉFINITION DU BLUETOOTH ----------
#define RX 7
#define TX 6
SoftwareSerial BlueT(RX,TX);
char Data;

int time1 = millis();
int time2 = millis();
bool chauffage = false; // on définit le huaffage comme étant éteind de base.

void setup() {
    Serial.begin(9600);
    delay(500);
    BlueT.begin(9600);
    // put your setup code here, to run once:
    

}

void loop() {
  
    time2 = millis();
    //Serial.println(time2);
    if (BlueT.available()){Data = BlueT.read();}strip.begin();
    strip.show();
    if (Data =='C') // Pour allumer le chauffage
        {
          Serial.println("DATA C");
            // setPixelColor(n° de led, Rouge, Vert, Bleu)
            // ROUGE
            strip.setPixelColor(0,223,0,0);
            strip.setPixelColor(1,223,0,0);
            strip.setPixelColor(2,223,0,0); 
            strip.setPixelColor(3,223,0,0);
            chauffage = true;
            time2 -= 5000;
        }
        else if(Data =='c') // Pour éteindre le chauffage
        {
          Serial.println("Data = c");
            // setPixelColor(n° de led, Rouge, Vert, Bleu)
            // BLEU
            strip.setPixelColor(0,0,0,223);
            strip.setPixelColor(1,0,0,223);
            strip.setPixelColor(2,0,0,223);
            strip.setPixelColor(3,0,0,223);
            chauffage = false;
            time2 -= 5000;
        }
    
    if( (time2-time1)>=6000 ){ //  ajout d'un bouton BT
      Serial.println("changement chauffage");
        if (chauffage == true){
            // setPixelColor(n° de led, Rouge, Vert, Bleu)
            // BLEU
            strip.setPixelColor(0,0,0,223);
            strip.setPixelColor(1,0,0,223);
            strip.setPixelColor(2,0,0,223);
            strip.setPixelColor(3,0,0,223);
            chauffage = false;
        }
        else{
            // setPixelColor(n° de led, Rouge, Vert, Bleu)
            // ROUGE
            strip.setPixelColor(0,223,0,0);
            strip.setPixelColor(1,223,0,0);
            strip.setPixelColor(2,223,0,0); 
            strip.setPixelColor(3,223,0,0);
            chauffage = true;
        }
        time1 = millis();
    }
    
}
