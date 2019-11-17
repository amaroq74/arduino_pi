
String str;

int r0_pin=4;
int r1_pin=5;
int r2_pin=6;
int r3_pin=7;

void setup()
{
  pinMode(r0_pin, OUTPUT);
  pinMode(r1_pin, OUTPUT);
  pinMode(r2_pin, OUTPUT);
  pinMode(r3_pin, OUTPUT);
  Serial.begin(115200);  //115200 is the baud rate UNO board communicate with Wifi Shield
 
}

void loop()
{
 str= Serial.readString(); //get message from MQTT

 if(str.length()>0 && str=="R0 ON") {
      digitalWrite(r0_pin, HIGH); 
      Serial.println("RO ON");
  }

 if(str.length()>0 && str=="R0 OFF") {
      digitalWrite(r0_pin, LOW); 
      Serial.println("RO OFF");
  }

 if(str.length()>0 && str=="R1 ON") {
      digitalWrite(r1_pin, HIGH); 
      Serial.println("R1 ON");
  }

 if(str.length()>0 && str=="R1 OFF") {
      digitalWrite(r1_pin, LOW); 
      Serial.println("R1 OFF");
  }

 if(str.length()>0 && str=="R2 ON") {
      digitalWrite(r2_pin, HIGH); 
      Serial.println("R2 ON");
  }

 if(str.length()>0 && str=="R2 OFF") {
      digitalWrite(r2_pin, LOW); 
      Serial.println("R2 OFF");
  }

 if(str.length()>0 && str=="R3 ON") {
      digitalWrite(r3_pin, HIGH); 
      Serial.println("R3 ON");
  }

 if(str.length()>0 && str=="R3 OFF") {
      digitalWrite(r3_pin, LOW); 
      Serial.println("R3 OFF");
  }

}

