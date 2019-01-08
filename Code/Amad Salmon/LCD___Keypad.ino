#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal liquid_crystal_display(rs, en, d4, d5, d6, d7);
char password[4];
char initial_password[4],new_password[4];
int i=0;
int relay_pin = 10;
char key_pressed=0;
const byte rows = 4; 
const byte columns = 4; 
char hexaKeys[rows][columns] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte row_pins[rows] = {A0,A1,A2,A3};
byte column_pins[columns] = {A4,A5,3,2};   
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void setup(){
  pinMode(relay_pin, OUTPUT);
  liquid_crystal_display.begin(16,2);
  liquid_crystal_display.print(" AMAD SALMON");
  liquid_crystal_display.setCursor(0,1);
  liquid_crystal_display.print("Electronic Lock ");
  delay(2000);
  liquid_crystal_display.clear();
  liquid_crystal_display.print("Enter Password");
  liquid_crystal_display.setCursor(0,1);
  initialpassword();
}


void loop(){
liquid_crystal_display.print("Enter Password:");
  
  digitalWrite(relay_pin, HIGH);
  key_pressed = keypad_key.getKey();
  if(key_pressed=='#')
    change();
  if (key_pressed){
    password[i++]=key_pressed;
    liquid_crystal_display.print(key_pressed);
      }
      
  if(i==4){
    delay(200);
    
    for(int j=0;j<4;j++)
      initial_password[j]=EEPROM.read(j);

    if(!(strncmp(password, initial_password,4))){

      liquid_crystal_display.clear();
      liquid_crystal_display.print("Pass Accepted");
      digitalWrite(relay_pin, LOW);
      delay(2000);
      liquid_crystal_display.setCursor(0,1);
      liquid_crystal_display.print("Pres # to change");
      delay(2000);
      liquid_crystal_display.clear();
      liquid_crystal_display.print("Enter Password:");
      liquid_crystal_display.setCursor(0,1);
      i=0;
    }

    else{

      digitalWrite(relay_pin, HIGH);
      liquid_crystal_display.clear();
      liquid_crystal_display.print("Wrong Password");
      liquid_crystal_display.setCursor(0,1);
      liquid_crystal_display.print("Pres # to Change");
      delay(2000);
      liquid_crystal_display.clear();
      liquid_crystal_display.print("Enter Password");
      liquid_crystal_display.setCursor(0,1);
      i=0;
    }

  }

}

void change(){
  int j=0;
  liquid_crystal_display.clear();
  liquid_crystal_display.print("Current Password");
  liquid_crystal_display.setCursor(0,1);
  
  while(j<4){

    char key=keypad_key.getKey();
    if(key){
      new_password[j++]=key;
      liquid_crystal_display.print(key);
    }
    key=0;
  }

  delay(500);




  if((strncmp(new_password, initial_password, 4)))

  {

    liquid_crystal_display.clear();

    liquid_crystal_display.print("Wrong Password");

    liquid_crystal_display.setCursor(0,1);

    liquid_crystal_display.print("Try Again");

    delay(1000);

  }

  else

  {

    j=0;

    liquid_crystal_display.clear();

    liquid_crystal_display.print("New Password:");

    liquid_crystal_display.setCursor(0,1);

    while(j<4)

    {

      char key=keypad_key.getKey();

      if(key)

      {

        initial_password[j]=key;

        liquid_crystal_display.print(key);

        EEPROM.write(j,key);

        j++;

     

      }

    }

    liquid_crystal_display.print("Pass Changed");

    delay(1000);

  }

  liquid_crystal_display.clear();

  liquid_crystal_display.print("Enter Password");

  liquid_crystal_display.setCursor(0,1);

  key_pressed=0;

}




void initialpassword(){
  for(int j=0;j<4;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
    initial_password[j]=EEPROM.read(j);

}
