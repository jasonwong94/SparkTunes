
void setup() {
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  
  
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  
  //digitalWrite(6, LOW);
  //digitalWrite(7, LOW);
  
  Serial.begin(115200);
}

void loop() {
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  
  int x1, x2, y1, y2, x3;
  
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  //delay(1);
  x1 = digitalRead(6);
  x1 = digitalRead(6);
  y1 = digitalRead(7);
  y1 = digitalRead(7);
  
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  //delay(1);
  x2 = digitalRead(6);
  x2 = digitalRead(6);
  y2 = digitalRead(7);
  y2 = digitalRead(7);
  
  for (int i = 0; i < 30; i++ ) {
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    x3 = digitalRead(6);
    x3 = digitalRead(6);
    x3 = digitalRead(7);
    x3 = digitalRead(7);
  }
  
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  
  Serial.println("--------------------------");
  Serial.print(x1);
  Serial.print("    ");
  Serial.println(y1);
  Serial.print(x2);
  Serial.print("    ");
  Serial.println(y2);
  
  delay(1000);
  
//  while (1) {
//    digitalWrite(7, HIGH);   
//    delay(1000);
//    
//    for (int i = 0; i < 1; i++) {
//      for (int j = 0; j < 32; j++) {
//        digitalWrite(7, j == 0 ? HIGH : LOW);
//        digitalWrite(8, j % 2 == 0 ? HIGH : LOW);
//        digitalWrite(9, j % 2 == 0 ? LOW : HIGH);
//      }
//    }
//  }
}
