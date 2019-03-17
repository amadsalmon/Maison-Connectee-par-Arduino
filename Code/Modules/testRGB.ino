const int ledRouge = 10; // Pin de la led rouge de la LED RGB
const int ledVerte = 11; // Pin de la led verte de la LED RGB
const int ledBleue = 12; // Pin de la led bleue de la LED RGB

void setup() {
  // put your setup code here, to run once:
    pinMode(ledRouge, OUTPUT);
  pinMode(ledVerte, OUTPUT);
  pinMode(ledBleue, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  setCouleur(255, 0, 255);
}

 void etatLed(char etat){
    switch(etat){
      case 'v': // vert
        setCouleur(0, 255, 0);
        break;
      case 'r': // rouge
        setCouleur(255,0,0);
        break;
      case 'o': // orange
        setCouleur(255, 140, 0);
        break;
      case 'j': // jaune
        setCouleur(241, 196, 15);
        break;
      case 'b': // blanc
        setCouleur(255, 255, 255);
        break;
      case 'e':
      setCouleur(0, 0, 0);
      break;
    }
  }

  void setCouleur(int valeurRouge, int valeurVerte, int valeurBleu) {
    analogWrite(ledRouge, valeurRouge);
    analogWrite(ledVerte, valeurVerte);
    analogWrite(ledBleue, valeurBleu);
}
