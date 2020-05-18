
#define OUTPIN 2

void setup()   {
  pinMode(OUTPIN, OUTPUT);

  digitalWrite(OUTPIN, HIGH);

}

void loop() {

  digitalWrite(OUTPIN, LOW);

  delay(1000);

  digitalWrite(OUTPIN, HIGH);

  delay(2000);
}

