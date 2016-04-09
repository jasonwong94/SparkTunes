const char* note_names[15] = { "C3", "D3", "E3", "F3", "G3", "A3", "B3", "C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5"};

bool piReady = false;

void isRaspberryPiReady(){
  if(Serial.available() > 0){
    String message = Serial.readString();
    if(message = "Ready")
      piReady = true;
    else
      piReady = false; 
  }
}
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!piReady)
    isRaspberryPiReady();
  else{
//      Serial.println("D3 C3 D4");
    for(int i=0; i<15; i++){
      int send = random(0, 2);
      if(send){
        Serial.print(note_names[i]);
        Serial.print(" ");
      }
    }
    Serial.print("\n"); 
    delay(500); 
  }
}
