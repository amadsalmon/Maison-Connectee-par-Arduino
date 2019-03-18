#include <Servo.h>
Servo Locket;
void setup() {
Serial.begin(9600);
Locket.attach(10);

}

void loop() {
Serial.println("salut");
Locket.write(0);
int angle = Locket.read();
Serial.println(angle);
delay(1000);
Locket.write(80);
angle = Locket.read();
Serial.println(angle);
delay(1000);
Locket.write(130);
angle = Locket.read();
Serial.println(angle);
delay(1000);
angle = Locket.read();
Serial.println(angle);
delay(1000);
}
