#include <Servo.h>
 
Servo servoLeft;
Servo servoRight;

void setup() { 

  tone(4, 3000, 1000);
  delay(1000);

  servoLeft.attach(13);
  servoRight.attach(12);

  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);

  Serial.begin(115200);
}
 
void loop() {
   char buffer[100];
   String cmd;
   int  val;

   if ( Serial.available() > 0 ) {
      strcpy(buffer,"");
      val = 0;

      cmd = Serial.readStringUntil('\n');
      sscanf(cmd.c_str(),"%s %i",buffer,&val);

      if ( strcmp(buffer,"forward") == 0 ) {
         servoLeft.writeMicroseconds(1500 + val);
         servoRight.writeMicroseconds(1500 - val);
         Serial.println("ok");
      }

      else if ( strcmp(buffer,"backward") == 0 ) {
         servoLeft.writeMicroseconds(1500 - val);
         servoRight.writeMicroseconds(1500 + val);
         Serial.println("ok");
      }

      else if ( strcmp(buffer,"right") == 0 ) {
         servoLeft.writeMicroseconds(1500 + val);
         servoRight.writeMicroseconds(1500 + val);
         Serial.println("ok");
      }

      else if ( strcmp(buffer,"left") == 0 ) {
         servoLeft.writeMicroseconds(1500 - val);
         servoRight.writeMicroseconds(1500 - val);
         Serial.println("ok");
      }

      else if ( strcmp(buffer,"tone") == 0 ) {
         tone(4, 3000, val);
         Serial.println("ok");
      }

      else if ( strcmp(buffer,"stop") == 0 ) {
         servoLeft.writeMicroseconds(1500);
         servoRight.writeMicroseconds(1500);
         Serial.println("ok");
      }
      else {
         Serial.println("error");
      }
   }
}

