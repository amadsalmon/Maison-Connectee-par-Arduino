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


//----------------------- DÉFINITION DU KEYPAD -----------------------
const byte LIGNES = 4;
const byte COLONNES = 4;

// INTERFACE 
char keys[LIGNES][COLONNES]={ 
 {'1','2','3','A'},
 {'4','5','6', 'B'},
 {'7','8','9', 'C'},
 {'*','0','#', 'D'}
 };
 
byte lignePin[LIGNES]={4,5,6,7}; // LIGNES DE PINS DU KEYPAD
byte colonnePin[COLONNES]={8,9,10,11}; // COLONNES DE PINS KEYPAD

// MAPPAGE DU KEYPAD 
Keypad keypad = Keypad(makeKeymap(keys),lignePin,colonnePin,LIGNES,COLONNES); 

//-----------------------  -----------------------

String password = "1234";  // MOT DE PASSE
int position = 0; // VARIABLE POUR DÉTERMINER LA POSITION.
int longueurPassword = 4;

int mauvais = 0; // VARIABLE FOR CALCULATING THE mauvais INPUT.
int tentatives = 0; // VARIABLE POUR COMPTER LE NOMBRE DE TENTATIVES

Servo servo_Motor; 

//-------------------------------  SETUP  ------------------------------
void setup()
{
   pinMode(ledRouge,OUTPUT);
   pinMode(ledVerte,OUTPUT);
   
  lcd.begin(16,2);
  lcd.print("SERRURE CONNECTÉE");
  lcd.setCursor(0,2);
  lcd.print("       PAR:");
  delay(2000);
  lcd.clear();
  lcd.print("AMAD");
  lcd.setCursor(0,2);
  lcd.print("    SALMON");
  delay(2000);
  lcd.clear();
  servo_Motor.attach(3);
  verrouiller(true);
  delay(1000);
  pinMode(buzzer, OUTPUT);
}

//--------------------------- LOOP ----------------------------------
void loop(){
  INTRUS();
  
  noTone(buzzer);  
  lcd.clear();
  lcd.print("Entrez password:");
  delay(100);
  
 char pressed=keypad.getKey();// ON PREND L'ENTRÉE DU KEYPAD
 String key[3];
  
 if(pressed) // SI UNE KEY EST PRESSÉE
 {
  lcd.clear();
  lcd.print("Entrez le mdp :");
  lcd.setCursor(position,2);
  lcd.print(pressed);
  delay(500);
    if(pressed == '*' || pressed == '#')
      {
          position = 0;
          verrouiller(true);
          lcd.clear();
      }

    else if(pressed == password[position])
      {
          key[position]=pressed;
          position++;
      }
 
    else if (pressed != password[position] )
      {// IN CASE OF mauvais INPUT INCREMENT BOTH mauvais AND POSITION.
          mauvais++;
          position ++;
      }

    if(position == longueurPassword){
          if( mauvais >0) // SI IL Y A UNE MAUVAISE ENTREE 
            {
                tentatives++;
                mauvais = 0;
                position = 0;
                lcd.clear();
                lcd.print("mauvais");
                lcd.setCursor(5,2);
                lcd.print("MOT DE PASSE");
                delay(1000);
                verrouiller(true);
            }

          else if(position == longueurPassword && mauvais == 0)
            {// SI LE 
                position = 0;
                mauvais = 0;
                lcd.clear();
                lcd.print("MOT DE PASSE");
                lcd.setCursor(6,2);
                lcd.print("ACCEPTE");
                delay(2000);
                lcd.clear();
                lcd.print("DEVEROUILLAGE");
                delay(2000);
                verrouiller(false);
            }
          if(tentatives ==3)// SI 3 MAUVAISES TENTATIVES
            {
                tentatives=0;
                buzzer_beep();
                delay(500);
            }
        }
   } 
}


void buzzer_beep(){
   digitalWrite(buzzer,HIGH);
   delay(1000);
   digitalWrite(buzzer,LOW);
   delay(1000);
   lcd.clear();
   lcd.setCursor(16,0);
   lcd.print("INTRUS");
   lcd.setCursor(16,1);
   lcd.print("DETECTE");
   delay(2000);
   while(1)
   {
   lcd.scrollDisplayLeft();
   delay(200);
   }
}

void sirene(){
  tone(buzzer, 2300); 
  delay(200);        
  tone(buzzer, 1700);
  //noTone(buzzer);    
  delay(200);       

  }

void INTRUS(){
  sirene();
   lcd.clear();
   lcd.setCursor(16,0);
   lcd.print("INTRUS");
   lcd.setCursor(16,1);
   lcd.print("DETECTE");
   sirene();
   //delay(2000);
//   while(1)
//   {
//   lcd.scrollDisplayLeft();
//   delay(200);
   //}
  }

  void verrouiller(int verrouillee)// FONCTION POUR VERROUILLER LA SERRURE
  {
    if (verrouillee){
          digitalWrite(ledRouge, HIGH);
          digitalWrite(ledVerte, LOW);
          delay(1000);
          servo_Motor.attach(3);
          servo_Motor.write(10);
          delay(1000);
          servo_Motor.detach();
      }
      
    else{
          digitalWrite(ledRouge, LOW);
          digitalWrite(ledVerte, HIGH);
          delay(1000);
          servo_Motor.attach(3);
          servo_Motor.write(90);
          delay(1000);
          servo_Motor.detach();
      }
  }
