String str;
int led_pin=13;
void setup()
{
  pinMode(led_pin, OUTPUT);
  Serial.begin(115200);  //115200 is the baud rate UNO board communicate with Wifi Shield
 
}
void loop()
{
 str= Serial.readString(); //get message from MQTT

 if(str.length()>0 && str=="ON") {
      digitalWrite(led_pin, HIGH); 
      Serial.println("LED IS ON");
   }
  if(str.length()>0 && str=="OFF") {
      digitalWrite(led_pin, LOW); 
      Serial.println("LED IS OFF");
   }
}

