
bool send = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0 && !send){
    String incomingData = Serial.readString();
    if(incomingData == "Start Communication"){
      Serial.println(incomingData);
      send = true;
    }
    else{
      Serial.println("Need to receive handshake message");
    }
  }

  if(send){
    Serial.println("B3 C4 D4 A5 A6 A3 E3");
    delay(500); 
  }
}
