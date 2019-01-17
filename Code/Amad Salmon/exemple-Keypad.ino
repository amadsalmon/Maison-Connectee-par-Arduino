#include <Key.h>
#include <Keypad.h>


/*
 * https://github.com/chauhannaman98/Using-4-4-Keypad-With-Arduino
 *
 * Source_Code.ino
 *
 * Created: July 29,2017
 * Author : Naman Chauhan
 */

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

/*keymap defines the key pressed according to the row and columns just as appears on the keypad*/
char keymap[numRows][numCols]=
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[numCols] = {5,4,3,2}; //connect to the column pinouts of the keypad

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
  Serial.begin(9600);
}

//If key is pressed, this key is stored in keypressed variable
//If key is not equal to NO_KEY, then this key is printed out
//if count=17, then count is reset back to 0 (this means no key is pressed during the whole keypad scan process
void loop()
{
  char keypressed = myKeypad.getKey();
  KeyState keystate = myKeypad.getState();
  
  if (keypressed != NO_KEY);
    {
      Serial.println(keypressed);
    }
}
