// AMAD SALMON
//
// CE PROGRAMME EST UN ESSAI VISANT À EXÉCUTER PLUSIEURS COMMANDES EN MÊME TEMPS ET EN BOUCLE.
// En fonction de l'état du système (déverouillé, en attente d'identification, intrus détecté), différents mécanismes vont s'enclencher.
//
// VERSION POUR ARDUINO MEGA - 17/01/19
//
#include<Keypad.h> // LIBRAIRIE DE L'INTERFACE KEYPAD
#include<LiquidCrystal.h> // LIBRAIRIE DU LCD
#include<Servo.h>// LIBRARIE DU SERVOMOTEUR
#include<SoftwareSerial.h> 

// ---------- DÉFINITION DU BLUETOOTH ----------
#define RX 7
#define TX 6
SoftwareSerial BlueT(RX,TX);
char Data;


// DÉFINITION DU LCD
const int rs = 45, en = 43, d4 = 47, d5 = 49, d6 = 51, d7 = 53;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// DÉFINITION DES INDICATEURS
const int buzzer = 36; // BUZZER (SIRÈNE D'ALARME)
const int ledRouge = 11; // Pin de la led rouge de la LED RGB
const int ledVerte = 12; // Pin de la led verte de la LED RGB
 const int ledBleue = 13; // Pin de la led bleue de la LED RGB


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
byte rowPins[ROWS] = {38, 40, 42, 44}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {46, 48, 50, 52}; //connect to the column pinouts of the keypad

//INITIALISATION D'UNE INSTANCE DE CLASSE Keypad
Keypad leKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//------------------DEFINITION DES VARIABLES POUR LE BOUTON DE L'ENTREE---------------
const int boutonPin = 30; // je suppose qu'elle est libre ? 
int etatBouton = 0;


//-------------------------------  SETUP  ------------------------------
void setup()
{  
  Serial.begin(9600);
  delay(500);
  BlueT.begin(9600);

  pinMode(buzzer, OUTPUT);
  pinMode(ledRouge, OUTPUT);
  pinMode(ledVerte, OUTPUT);
  pinMode(ledBleue, OUTPUT);
  pinMode(boutonPin, INPUT);
  
  loquet.attach(10);

  lcd.begin(16,2);
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
  //--------------------------- OPEN/LOCK BOUTON ------------------------------------
  etatBouton = digitalRead(boutonPin);
  Serial.println(etatBouton);
  if (BlueT.available()){ Data = BlueT.read(); }
  
  
  if ( Data =='U'){ //etatBouton == HIGH ||
    if (serrureDeverouillee == true ){
      Serial.println("BT A FERME LA PORTE");
      verrouiller(true);
      delay(1000);
      serrureDeverouillee = false;
      attenteIdentification=true;
    }
    else {
      Serial.println("BT A OUVERT LA PORTE");
      verrouiller(false);
      delay(1000);
      serrureDeverouillee = true;
      attenteIdentification=false;
    }
  } 
  
  //--------------------------- SERRURE DÉVERROUILLÉE ----------------------------------
  if(serrureDeverouillee){
    Serial.println("Permission. LA PORTE EST OUVERTE.");
    
    etatLed('v');
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VOUS POUVEZ ENTRER.");
    //dureeDeverouillage++;
    /*
    if(dureeDeverouillage>80) // On garde la serrure déverouillée le temps de 80 tours de loop.
    {
      verrouiller(true); 
      serrureDeverouillee = false;
      attenteIdentification=true; 
      dureeDeverouillage=0;
    }*/
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
      lcd.print("Entrez le password");
      lcd.print("****");
      lcd.setCursor(position,1);
      lcd.print(key);
      delay(500);
      if(key == '*' || key == '#') // Pour verrouiller manuellement le système
        {
          Serial.println("VERROUILLAGE PAR *# ");
          position = 0;
          verrouiller(true);
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
                  delay(30);
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
                  
              }
          }
        }
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
            setTexte("SYSTÈME DEJA",0);
            setTexte("VERROUILLÉ",1);
            }
          loquet.write(90);
          delay(1000);
          etatLed('o');
          
          
          Serial.println("SYSTÈME VERROUILLÉ");
          lcd.clear();
          setTexte("SYSTÈME VERROUILLÉ",0);
          attenteIdentification = true;
      }
      
    else{
          Serial.println("SYSTÈME EN COURS DE DÉVERROUILLAGE");
          lcd.clear();
          setTexte("En cours de",0);
          setTexte("déverrouillage...",1);
          etatLed('j');
          delay(1000);
          loquet.write(160);
          delay(1000);
          etatLed('v');
          lcd.clear();
          setTexte("SYSTÈME DÉVEROUILLÉ",0);
          attenteIdentification = true;
      }
  }


///--------------------------- FONCTIONS INDICATRICES --------------------------- 

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
    setTexte("DÉTÉCTÉ",1);
   etatLed('r');
   sirene();
  }

  // Fonction qui commande la LED RGB  
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

void setTexte(String s,int ligne){
  lcd.setCursor(1,ligne);
  lcd.print(s);
  Serial.print(s);
  }
