// AMAD SALMON
//
// CE PROGRAMME EST UN ESSAI VISANT À EXÉCUTER PLUSIEURS COMMANDES EN MÊME TEMPS ET EN BOUCLE.
// En fonction de l'état du système (déverouillé, en attente d'identification, intrus détecté), différents mécanismes vont s'enclencher.
//
// VERSION POUR ARDUINO MEGA - 17/01/19
//
#include <Key.h>
#include <Keypad.h> // LIBRAIRIE DE L'INTERFACE KEYPAD
#include <LiquidCrystal_I2C.h> // LIBRAIRIE DU LCD
#include <Servo.h>// LIBRARIE DU SERVOMOTEUR
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h> // Librairie du la bande LED


// ---------- DÉFINITION DU BLUETOOTH ----------
#define RX 50
#define TX 52
SoftwareSerial BlueT(RX,TX);
char Data;

// DÉFINITION DU LCD
LiquidCrystal_I2C lcd(0x27,20,4);  
// set the LCD address to 0x27 for a 16 chars and 2 line display

// DÉFINITION DES INDICATEURS
const int ledRouge = 10; // Pin de la led rouge de la LED RGB
const int ledVerte = 11; // Pin de la led verte de la LED RGB
const int ledBleue = 12; // Pin de la led bleue de la LED RGB
const int buzzer = 13; // BUZZER (SIRÈNE D'ALARME)

const int lumiere1 = 6;
const int lumiere2 = 5;
bool absence = true;
int luminosite;

// CHAUFFAGE
const int ledChauffage = 9; // Pin de la barre de LED représentant le chauffage
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, 9, NEO_GRB + NEO_KHZ800);
int time1;
int time2;
bool chauffage = false; // on définit le chauffage comme étant éteind de base.



Servo loquet; 
int angle; // Utile lors du controle de l'orientation du loquet.

//----------------------- DÉFINITION DU KEYPAD -----------------------

const byte ROWS = 4; //QUATRE LIGNES
const byte COLS = 4; // QUATRE COLONNES

// INTERFACE
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {53, 51, 49, 47}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {45, 43, 41, 39}; //connect to the column pinouts of the keypad

//INITIALISATION D'UNE INSTANCE DE CLASSE Keypad
Keypad leKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//------------------BOUTON D'ENTRÉE ---------------
const int boutonPin = 7; 
int etatBouton = 0;


//-------------------------------  SETUP  ------------------------------
void setup()
{
  
  Serial.begin(9600);
  BlueT.begin(9600);

  pinMode(buzzer, OUTPUT);
  pinMode(ledRouge, OUTPUT);
  pinMode(ledVerte, OUTPUT);
  pinMode(ledBleue, OUTPUT);
  pinMode(lumiere1, OUTPUT);
  pinMode(lumiere2, OUTPUT);
  pinMode(boutonPin, INPUT);

  strip.begin();
  strip.show();
  time1 = millis();
  
  loquet.attach(8);

  lcd.init();
  lcd.backlight();
  lcd.print("SERRURE CONNECTEE");
  delay(2000);
  lcd.clear();
  verrouiller(true);
  etatLed('o');
  delay(1000);

  
  
}

//----------------------- DÉFINITION DE VARIABLES UTILITAIRES -----------------------

String password = "2623";  // MOT DE PASSE (<=>AMAD)
int position = 0; // VARIABLE POUR DÉTERMINER LA POSITION.
int longueurPassword = 4;

int mauvais = 0; // VARIABLE FOR CALCULATING THE mauvais INPUT.
int tentatives = 0; // VARIABLE POUR COMPTER LE NOMBRE DE TENTATIVES

long dureeAlarme = 0;
long dureeDeverouillage = 0;
long dureeAttente = 0;

//----------------------- DÉFINITION DES VARIABLES D'ÉTAT -----------------------
// Le système a 3 états : "serrureDeverouillee", "attenteIdentification", et "intrusion".
// En fonction de l'état du système, différents mécanismes s'enclencheront.
// Par défaut, le système est en état attenteIdentification.
bool intrusion = false;
bool serrureDeverouillee = false;
bool attenteIdentification=true;


void loop() {
  time2 = millis();
  if (BlueT.available()){ Data = BlueT.read(); }
  
  if ( Data =='L') // LUMIÈRES
  {
    luminosite = BlueT.parseInt();
    lumieres(luminosite);
    } 
  
  
  //---------- OPEN/LOCK BOUTON -----------------
  etatBouton = digitalRead(boutonPin);
  Serial.println(etatBouton);
  if ( Data =='U' || etatBouton == HIGH){
    if (serrureDeverouillee == true ){
      verrouiller(true);
      delay(1000);
      serrureDeverouillee = false;
    }
    else {
      verrouiller(false);
      delay(1000);
      serrureDeverouillee = true;
    }
  } 
  //----------------- CHAUFFAGE -----------------
  if (Data =='C') // Pour allumer le chauffage
  {
    Serial.println("DATA C");
    // setPixelColor(n° de led, Rouge, Vert, Bleu)
    // ROUGE
    strip.setPixelColor(0,223,0,0);
    strip.setPixelColor(1,223,0,0);
    strip.setPixelColor(2,223,0,0); 
    strip.setPixelColor(3,223,0,0);
    strip.show();
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
    strip.show();
    chauffage = false;
    time2 -= 5000;
  }
  Serial.println(time1);
  Serial.println(time2); 
  if( (time2-time1)>=10000 ){ 
    if (chauffage == true){
        // setPixelColor(n° de led, Rouge, Vert, Bleu)
        // BLEU
        strip.setPixelColor(0,0,0,223);
        strip.setPixelColor(1,0,0,223);
        strip.setPixelColor(2,0,0,223);
        strip.setPixelColor(3,0,0,223);
        strip.show();
        chauffage = false;
    }
    else{
        // setPixelColor(n° de led, Rouge, Vert, Bleu)
        // ROUGE
        strip.setPixelColor(0,223,0,0);
        strip.setPixelColor(1,223,0,0);
        strip.setPixelColor(2,223,0,0); 
        strip.setPixelColor(3,223,0,0);
        strip.show();
        chauffage = true;
    }
    time1 = millis();
  }

  //--------------------------- EN ATTENTE D'IDENTIFICATION ----------------------------------
  if(attenteIdentification){

      etatLed('o');

    Serial.println("ATTENTE d'IDENTIFICATION");
    
    
    char key=leKeypad.getKey();  // On récupère l'entrée de chaque key tapée au keypad
    String entree[longueurPassword];  // On stocke l'entrée dans une variable tableau de même taille que le password
  
    if(key) // SI UNE KEY EST PRESSÉE
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Entrez le MDP");
      lcd.setCursor(0,1);
      lcd.print("****"); // à inverser
      lcd.setCursor(position,1);
      lcd.print(key);
      delay(500);
      if(key == '*' || key == '#') // Pour verrouiller manuellement le système
        {
          Serial.println("VERROUILLAGE PAR *# ");
          position = 0;
          verrouiller(true);

        // ON ÉTEINT LES LUMIERES
        absence=false;
        luminosite = 0;
        lumieres(luminosite);

        // On éteint le chauffage
        strip.setPixelColor(0,0,0,0);
        strip.setPixelColor(1,0,0,0);
        strip.setPixelColor(2,0,0,0);
        strip.setPixelColor(3,0,0,0);
        strip.show();
        chauffage = false;

        }
      if(key == 'D')
        {
          sonnette();
          }
      else if(key == password[position]) // En cas d'entrée correcte
        {
          entree[position]=key;
          position++;
        }
   
      else if (key != password[position] ) // En cas d'entrée incorrecte
        {
            mauvais++;
            position ++;
        }
  
        if(position == longueurPassword){
            if( mauvais > 0  &&  tentatives < 3) // EN CAS DE MAUVAIS PASSWORD TAPÉ
              {
                  tentatives++;
                  mauvais = 0;
                  position = 0;
                  lcd.clear();
                  setTexte("Mauvais password.",0);
                  setTexte("Reessayez.",1);
                  etatLed('r');
                  erreurPassword_SON();
                  delay(100);
                  etatLed('o');
                  delay(500);        
              }
        if(tentatives == 3)  // SI 3 MAUVAISES TENTATIVES
        {
                  tentatives=0; // on réinitialise le nombre de tentatives à 0 
                  attenteIdentification=false;
                  intrusion = true; // On active le mode "INTRUSION"
                  Serial.println("3 MAUVAISES TENTATIVES");
              }
  
            else if(position == longueurPassword && mauvais == 0){ 
                  position = 0;
                  mauvais = 0;
                  lcd.clear();
                  setTexte("MOT DE PASSE",0);
                  setTexte("ACCEPTE",1);
                  
                  delay(1000);
                  lcd.clear();
                  setTexte("DEVEROUILLAGE",0);
                  setTexte("EN COURS",1);
                  delay(1500);

                  verrouiller(false);

                  attenteIdentification = false;
                  serrureDeverouillee = true;

                  // ON ALLUME LES LUMIERES
                  absence=false;
                  luminosite = 255;
                  lumieres(luminosite);
                  
            }
          }
         }
        }
    else{ 
      attenteIdentification = true;
      serrureDeverouillee == false;
    }
    
  //--------------------------- INTRUSION ----------------------------------
  if(intrusion){
    Serial.println("MODE INTRUSION");
    INTRUS();
    dureeAlarme++;
    if(dureeAlarme>15) // L'alarme intrusion dure 30 loops
    {
      dureeAlarme=0;
      noTone(buzzer);
      intrusion=false;
      attenteIdentification=true;
    }
  }
} 

///--------------------------- FONCTION POUR VERROUILLER LA SERRURE --------------------------- 
void verrouiller(bool verrouillee)
  {
    if (verrouillee){
          etatLed('j');
          angle = loquet.read();
          delay(1000);
          if ( angle ==0){
            lcd.clear();
            setTexte("SYSTEME DEJA",0);
            setTexte("VERROUILLE",1);
            }
          loquet.write(4);
          delay(1000);
          etatLed('o');
          
          
          Serial.println("SYSTEME VERROUILLE");
          lcd.clear();
          setTexte("SYSTEME",0);
          setTexte("VERROUILLE",1);
          attenteIdentification = true;
      }
      
    else{
          Serial.println("SYSTEME EN COURS DE DÉVERROUILLAGE");
          lcd.clear();
          setTexte("En cours de",0);
          setTexte("deverouillage",1);
          etatLed('j');
          delay(1000);
          loquet.write(90);
          delay(1000);
          etatLed('v');
          lcd.clear();
          setTexte("SYSTEME",0);
          setTexte("DEVEROUILLE",1);
          attenteIdentification = true;
      }
  }


///--------------------------- FONCTIONS INDICATRICES --------------------------- 

// Fonction pour allumer les deux lumières à la luminosité souhaitée
void lumieres(int luminosite) // luminosité de 0 à 100
{
  //int luminosite_255 = 255*luminosite/100; //conversion de la luminositée/100 à luminosité/255 pour le PWM

  analogWrite(lumiere1,luminosite);
  analogWrite(lumiere2,luminosite);
  }

void clignoterLumieres(bool absence){
    digitalWrite(lumiere1,HIGH);
    digitalWrite(lumiere2,HIGH);
    delay(200);
    digitalWrite(lumiere1,LOW);
    digitalWrite(lumiere2,LOW);
    delay(200);
    digitalWrite(lumiere1,HIGH);
    digitalWrite(lumiere2,HIGH);
    delay(200);
    digitalWrite(lumiere1,LOW);
    digitalWrite(lumiere2,LOW);
    delay(200);
    digitalWrite(lumiere1,HIGH);
    digitalWrite(lumiere2,HIGH);
    delay(200);
    digitalWrite(lumiere1,LOW);
    digitalWrite(lumiere2,LOW);
    delay(200);
    if(!absence){
      lumieres(luminosite);
    }
  }

 void sonnetteSON(){
  tone(buzzer, 2000); 
  delay(500);        
  tone(buzzer, 1500);
  delay(600); 
  noTone(buzzer);      
  }
  
void sonnette(){
  lcd.clear();
  Serial.println("Invité");
  setTexte("Veuillez patienter... ",0);
  setTexte("J'arrive...",1);
  clignoterLumieres(absence);
  sonnetteSON();
}

void erreurPassword_SON(){
    tone(buzzer, 100); 
    delay(50);
    noTone(buzzer); 
    delay(45);         
    tone(buzzer, 100);
    delay(70); 
    noTone(buzzer);
    }

// Fonction qui fait imiter au buzzer le son d'une sirène d'alarme grace à des fréquences particulières
void sirene(){
  tone(buzzer, 2300); 
  delay(200);        
  tone(buzzer, 1700);
  delay(200);       
  }

// Fonction à activer lorsque le système entre dans le mode "intrusion"
//  - Affiche un message d'alerte
//  - Déclenche la sirène d'alarme par sirene()
//  - Allume la LED en rouge
void INTRUS(){
  lcd.clear();
   Serial.println("INTRUS !");
    setTexte("INTRUS",0);
    setTexte("DETECTE",1);
   etatLed('r');
   sirene();
  }

  // Fonction qui commande la LED RGB  
  void etatLed(char etat){
    switch(etat){
      case 'v': // vert
        setCouleur(255, 0, 255);
        break;
      case 'r': // rouge
        setCouleur(0,255,255);
        break;
      case 'o': // orange
        setCouleur(0,115, 255);
        break;
      case 'j': // jaune
        setCouleur(14, 59, 240);
        break;
      case 'b': // blanc
        setCouleur(0, 0, 0);
        break;
      case 'e':
      setCouleur(255, 255, 255);
      break;
    }
  }

  void setCouleur(int valeurRouge, int valeurVerte, int valeurBleu) {
    analogWrite(ledRouge, valeurRouge);
    analogWrite(ledVerte, valeurVerte);
    analogWrite(ledBleue, valeurBleu);
}

void setTexte(String s,int ligne){
  lcd.setCursor(0,ligne);
  lcd.print(s);
  Serial.print(s);
  }





    
