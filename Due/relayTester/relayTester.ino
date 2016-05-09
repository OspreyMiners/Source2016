
void setup() {
  for(int i = 53;i > 36;i--){
    pinMode(i, OUTPUT);
    digitalWrite(i,HIGH);
  }
  for(int i = 53;i > 36;i--){
    pinMode(i, OUTPUT);
    digitalWrite(i,LOW);
    delay(1000);
    digitalWrite(i,HIGH);
  }
}

// the loop function runs over and over again forever
void loop() {
}
