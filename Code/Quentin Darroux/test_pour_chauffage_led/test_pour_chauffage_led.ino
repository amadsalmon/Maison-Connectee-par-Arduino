#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, 6, NEO_GRB + NEO_KHZ800);
void setup() {
  // put your setup code here, to run once:
int time1 = millis();
strip.begin();
strip.show();
bool chauffage = false; // on définit le huaffage comme étant éteind de base.

}

void loop() {
int time2 = millis();
if( (time2-time1)>=30 000){ // || ajout d'un bouton BT
    if (chauffage == true){
      // setPixelColor(n° de led, Rouge, Vert, Bleu)
      strip.setPixelColor(0,0,0,223);
      strip.setPixelColor(1,0,0,223);
      strip.setPixelColor(2,0,0,223);
      strip.setPixelColor(3,0,0,223);
      chauffage = false;
    }
     else{
      // setPixelColor(n° de led, Rouge, Vert, Bleu)
      strip.setPixelColor(0,223,0,0);
      strip.setPixelColor(1,223,0,0);
      strip.setPixelColor(2,223,0,0);
      strip.setPixelColor(3,223,0,0);
      chauffage = true;
     }
    }
}
