//
// AMAD SALMON
//
// CE PROGRAMME N'EST PAS À JOUR : CONSULTER codeMaster-vMega.ino
// CE PROGRAMME NE SERA PAS UTILISÉ CAR IL EST FAIT POUR UNE CARTE UNO, QUI NE PEUT FOURNIR ASSEZ DE PINS. 
//
// IL EXÉCUTE PLUSIEURS COMMANDES EN MÊME TEMPS ET EN BOUCLE.
// En fonction de l'état du système (déverouillé, en attente d'identification, intrus détecté), différents mécanismes vont s'enclencher.
//

#include<Keypad.h> // LIBRAIRIE DE L'INTERFACE KEYPAD
#include<LiquidCrystal.h> // LIBRAIRIE DU LCD
#include<Servo.h>// LIBRARIE DU SERVOMOTEUR


// DÉFINITION DU LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// DÉFINITION DES INDICATEURS
const int buzzer =13; // BUZZER (SIRÈNE D'ALARME)
const int ledRouge= 12; // Pin de la led rouge de la LED RGB
const int ledVerte = 11; // Pin de la led verte de la LED RGB
 const int ledBleue = 22; // Pin de la led bleue de la LED RGB


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
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//INITIALISATION D'UNE INSTANCE DE CLASSE Keypad
Keypad leKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


//-------------------------------  SETUP  ------------------------------
void setup()
{
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT);
  pinMode(ledRouge, OUTPUT);
  pinMode(ledVerte, OUTPUT);
  pinMode(ledBleue, OUTPUT);
  
  loquet.attach(10);

  lcd.begin(16,2);
  lcd.print("SERRURE CONNECTÉE");
  delay(2000);
  lcd.clear();
  verrouiller(true);
  etatLed('o');
  delay(1000);
}

//----------------------- DÉFINITION DE VARIABLES UTILITAIRES -----------------------

String password = "1234";  // MOT DE PASSE
int position = 0; // VARIABLE POUR DÉTERMINER LA POSITION.
int longueurPassword = 4;

int mauvais = 0; // VARIABLE FOR CALCULATING THE mauvais INPUT.
int tentatives = 0; // VARIABLE POUR COMPTER LE NOMBRE DE TENTATIVES

long dureeAlarme = 0;
long dureeDeverouillage = 0;

//----------------------- DÉFINITION DES VARIABLES D'ÉTAT -----------------------
// Le système a 3 états : serrureDeverouillee, attenteIdentification, et intrusion.
// En fonction de l'état du système, différents mécanismes s'enclencheront.
// Par défaut, le système est en état attenteIdentification.
bool intrusion = false;
bool serrureDeverouillee = false;
bool attenteIdentification=true;


void loop() {
  //--------------------------- SERRURE DÉVERROUILLÉE ----------------------------------
  if(serrureDeverouillee){
    Serial.println("Permission. LA PORTE EST OUVERTE.");
    dureeDeverouillage++;
    if(dureeDeverouillage>20) // On garde la serrure déverouillée le temps de 20 tours de loop.
    {
      dureeDeverouillage=0;
      bool serrureDeverouillee = false;
      bool attenteIdentification=true; 
      verrouiller(true); 
    }
  }

  //--------------------------- EN ATTENTE D'IDENTIFICATION ----------------------------------
  else if(attenteIdentification){

    Serial.println("ATTENTE d'IDENTIFICATION");
    
    char key=leKeypad.getKey();  // On récupère l'entrée de chaque key tapée au keypad
    String entree[longueurPassword];  // On stocke l'entrée dans une variable tableau de même taille que le password
  
   if(key) // SI UNE KEY EST PRESSÉE
   {
    lcd.clear();
    lcd.print("Entrez le password");
    lcd.setCursor(position,2);
    lcd.print(key);
    delay(500);
      if(key == '*' || key == '#') // Pour verrouiller manuellement le système
        {
            Serial.println("VERROUILLAGE PAR *# ");
            position = 0;
            verrouiller(true);
            lcd.clear();
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
            if( mauvais >0 && tentatives <3) // EN CAS DE MAUVAIS PASSWORD TAPÉ
              {
                  tentatives++;
                  mauvais = 0;
                  position = 0;
                  etatLed('o');
                  lcd.clear();
                  lcd.print("Mauvais");
                  lcd.setCursor(5,2);
                  lcd.print("mot de passe. Reessayez.");
                  Serial.println("Mauvais mot de passe. Réessayez.");
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
                  lcd.print("MOT DE PASSE");
                  lcd.setCursor(6,2);
                  lcd.print("ACCEPTE");
                  Serial.println("PASSWORD ACCEPTÉ");
                  
                  delay(2000);
                  lcd.clear();
                  lcd.print("DEVEROUILLAGE");
                  Serial.println("DÉVEROUILLAGE");
                  delay(2000);
                  verrouiller(false);

                  attenteIdentification = false;
                  serrureDeverouillee = true;
                  
              }
          }
        }
      }
    
  //--------------------------- INTRUSION ----------------------------------
  else if(intrusion){
    Serial.println("MODE INTRUSION");
    INTRUS();
    dureeAlarme += 1;
    if(dureeAlarme>15) // L'alarme intrusion dure 15 loops
    {
      dureeAlarme=0;
      intrusion=false;
      attenteIdentification=true;
    }
}
}

///--------------------------- FONCTION POUR VERROUILLER LA SERRURE --------------------------- 
void verrouiller(bool verrouillee)
  {
    if (verrouillee){
      Serial.println("SYSTÈME EN COURS DE VERROUILLAGE");
          etatLed('j');
          angle = loquet.read();
          delay(1000);
          if ( angle ==0){
            Serial.println("LE SYSTEM EST DEJA VEROUILLE");}
          loquet.write(0);
          delay(1000);
          etatLed('o');
          Serial.println("SYSTÈME VERROUILLÉ");
      }
      
    else{
          Serial.println("SYSTÈME EN COURS DE DÉVERROUILLAGE");
          etatLed('b');
          delay(1000);
          loquet.write(90);
          delay(1000);
          etatLed('v');
          Serial.println("SYSTÈME DÉVERROUILLÉ");
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
   Serial.println("INTRUS !");
   lcd.clear();
   lcd.setCursor(16,0);
   lcd.print("INTRUS");
   lcd.setCursor(16,1);
   lcd.print("DETECTE");
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
        setCouleur(255, 108, 0);
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






    
