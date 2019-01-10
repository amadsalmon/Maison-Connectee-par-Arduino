// 
// AMAD SALMON
//
// Programme servant à  montrer le fonctionnement d'une LED RGB 
//

int ledRouge= 12;
int ledVerte = 11;
int ledBleue = 10;

void setup() {
  pinMode(ledRouge, OUTPUT);
  pinMode(ledVerte, OUTPUT);
  pinMode(ledBleue, OUTPUT);
}

void loop() {
  setCouleur(255, 0, 0); // En rouge
  delay(1000);
  setCouleur(0, 255, 0); // en vert
  delay(1000);
  setCouleur(0, 0, 255); // En bleu
  delay(1000);
  setCouleur(255, 255, 255); // En blanc
  delay(1000);
  setCouleur(170, 0, 255); // En violet
  delay(1000);
}

void setCouleur(int valeurRouge, int valeurVerte, int valeurBleu) {
  analogWrite(ledRouge, valeurRouge);
  analogWrite(ledVerte, valeurVerte);
  analogWrite(ledBleue, valeurBleu);
}
