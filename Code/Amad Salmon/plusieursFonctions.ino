// AMAD SALMON
//
// CE PROGRAMME EST UN ESSAI VISANT À EXÉCUTER PLUSIEURS COMMANDES EN MÊME TEMPS ET EN BOUCLE.
// En fonction de l'état du système (déverouillé, en attente d'identification, intrus détecté), différents mécanismes vont s'enclencher.
//
#include<Keypad.h> // LIBRAIRIE DE L'INTERFACE KEYPAD
#include<LiquidCrystal.h> // LIBRAIRIE DU LCD
#include<Servo.h>// LIBRARIE DU SERVOMOTEUR
//#include<String.h>

// DÉFINITION DU LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// DÉFINITION DES INDICATEURS
int buzzer =13; // BUZZER (SIRÈNE D'ALARME)
int ledRouge = 9; // LED ROUGE
int ledVerte = 8; // LED VERTE


Servo servo_Motor; 

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
   pinMode(ledRouge,OUTPUT);
   pinMode(ledVerte,OUTPUT);
   pinMode(buzzer, OUTPUT);
   
  lcd.begin(16,2);
  lcd.print("SERRURE CONNECTÉE");
  lcd.setCursor(0,2);
  lcd.print("       PAR:");
  delay(2000);
  lcd.clear();
  lcd.print("    AMAD");
  lcd.setCursor(0,2);
  lcd.print("     SALMON");
  delay(2000);
  lcd.clear();
  servo_Motor.attach(3);
  verrouiller(true);
  delay(1000);
}

//-----------------------  -----------------------

String password = "1234";  // MOT DE PASSE
int position = 0; // VARIABLE POUR DÉTERMINER LA POSITION.
int longueurPassword = 4;

int mauvais = 0; // VARIABLE FOR CALCULATING THE mauvais INPUT.
int tentatives = 0; // VARIABLE POUR COMPTER LE NOMBRE DE TENTATIVES

int dureeAlarme = 0;
int dureeDeverouillage = 0;

bool intrusion = false;
bool toutEstOK = false;
bool attenteIdentification=true;

// En fonction de l'état du système (déverouillé, en attente d'identification, intrus détecté), différents mécanismes vont s'enclencher.
void loop() {
  //--------------------------- TOUT EST OK ----------------------------------
  if(toutEstOK){
    Serial.println("TOUT EST OK. LA PORTE EST OUVERTE.");
    dureeDeverouillage++;
    if(dureeDeverouillage>20){
      dureeDeverouillage=0;
      bool toutEstOK = false;
      bool attenteIdentification=true;
      verrouiller(true);
    }
  }
  //--------------------------- EN ATTENTE D'IDENTIFICATION ----------------------------------
  else if(attenteIdentification){

    Serial.println("ATTENTE d'IDENTIFICATION");
    
    char key=leKeypad.getKey();// ON PREND L'ENTRÉE DU KEYPAD
    String entree[4];
  
   if(key) // SI UNE KEY EST PRESSÉE
   {
    lcd.clear();
    lcd.print("Entrez le password");
    lcd.setCursor(position,2);
    lcd.print(key);
    delay(500);
      if(key == '*' || key == '#')
        {
            Serial.println("VERROUILLAGE PAR *# ");
            position = 0;
            verrouiller(true);
            lcd.clear();
        }
  
      else if(key == password[position])
        {
            entree[position]=key;
            position++;
        }
   
      else if (key != password[position] )
        {// SI mauvais INPUT INCREMENT  mauvais et POSITION.
            mauvais++;
            position ++;
        }
        if(tentatives ==3)  // SI 3 MAUVAISES TENTATIVES
              {
                  tentatives=0;
                  attenteIdentification=false;
                  intrusion = true; /////////////////////////// IMPORTANT
                  Serial.println("3 MAUVAISES TENTATIVES");
              }
  
      if(position == longueurPassword){
            if( mauvais >0 && tentatives <3) // SI IL Y A UNE MAUVAISE ENTREE 
              {
                  tentatives++;
                  mauvais = 0;
                  position = 0;
                  lcd.clear();
                  lcd.print("mauvais");
                  lcd.setCursor(5,2);
                  lcd.print("MOT DE PASSE");
                  Serial.println("Mauvais mot de passe. Réessayez.");
                  delay(1000);        
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
                  toutEstOK = true;
                  
              }
          }
        }
      }
    
    //--------------------------- INTRUSION ----------------------------------
  else if(intrusion){
    Serial.println("MODE INTRUSION");
    INTRUS(true);
    dureeAlarme += 1;
    if(dureeAlarme>=15){
      dureeAlarme=0;
      intrusion=false;
      attenteIdentification=true;
      }
}
}

void verrouiller(bool verrouillee)// FONCTION POUR VERROUILLER LA SERRURE
  {
    if (verrouillee){
      Serial.println("SYSTÈME EN COURS DE VERROUILLAGE");
          digitalWrite(ledRouge, HIGH);
          digitalWrite(ledVerte, LOW);
          delay(1000);
          servo_Motor.attach(3);
          servo_Motor.write(10);
          delay(1000);
          servo_Motor.detach();
          Serial.println("SYSTÈME VERROUILLÉ");
      }
      
    else{
          Serial.println("SYSTÈME EN COURS DE DÉVERROUILLAGE");
          digitalWrite(ledRouge, LOW);
          digitalWrite(ledVerte, HIGH);
          delay(1000);
          servo_Motor.attach(3);
          servo_Motor.write(90);
          delay(1000);
          servo_Motor.detach();
          Serial.println("SYSTÈME DÉVERROUILLÉ");
      }
  }

void sirene(){
  tone(buzzer, 2300); 
  delay(200);        
  tone(buzzer, 1700);
  //noTone(buzzer);    
  delay(200);       

  }

void INTRUS(bool active){
  
  if(active){
   Serial.println("INTRUS !");
   lcd.clear();
   lcd.setCursor(16,0);
   lcd.print("INTRUS");
   lcd.setCursor(16,1);
   lcd.print("DETECTE");
   sirene();
  }
  else{  noTone(buzzer); 
  Serial.println("PLUS D'INTRUS...");}
  }
