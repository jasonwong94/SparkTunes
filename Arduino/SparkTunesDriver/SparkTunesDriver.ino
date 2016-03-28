// Pin Assignments
// pin numbers of the columns
const int columns[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
// pin numbers of the rows
const int rows[15] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};
// Bypass signal to activate NMOS transistors
const int bypass = 48;
// Play button
const int play_button = 49;
// End Pin Assignments

// Other Constants
const int DEBOUNCE_DELAY = 200; // ms
const char* note_names[15] = { "C4", "D4", "E4", "F4", "G4", "A5", "B5", "C5", "D5", "E5", "F5", "G5", "A6", "B6", "C6" };
enum MODE { STARTUP, COMPOSE, PLAY, SHARE };
// End Other Constants

// Global Variables
// 1 == on, 0 == off. Indexed first by column, then by row
char switch_value[32][15];
// time (in ms since startup) of last switch value change. For debouncing
unsigned long switch_value_last_change[32][15];
// play button variables
char play_button_value;
unsigned long play_button_last_change;
// what mode are we in?
int current_mode = STARTUP;
// End Global Variables

// Reads the given pin, and returns true iff the value of the pin changed from 0->1 in this
// read cycle. Also takes into account debouncing - will not return true within DEBOUNCE_DELAY
// ms of the last time the input toggled/
// pin: the pin to read from
// value: address of the place to store the current pin value
// last_updated: address of the place to store the time if this pin was changed
// now: current time, in ms since startup
int get_button_posedge(int pin, char* value, unsigned long* last_updated, unsigned long now) {
  if (now - *last_updated < DEBOUNCE_DELAY) {
    // debouncing
    return false;
  }
  
  char new_value = digitalRead(pin);
  if (new_value != *value) {
    *value = new_value;
    *last_updated = now;
    return new_value;
  }
  
  return false;
}

// Given an array of 15 chars. Each char is treated as a bool, 1 => note on, 0 => note off
// Sends to Pi to tell it which notes to play
void send_notes(char* notes) {
  int first_send_done = false;
  for (int i = 0; i < 15; i++) {
    if (notes[i]) {
      if (first_send_done) {
        Serial.print(" ");
      }
      Serial.print(note_names[i]);
      first_send_done = true;
    }
  }
  Serial.println(""); // newline
}

// Reads the entire switch array, all at once.
// Updates newly_pressed_switches[15] array such that each entry will be 1 if there is a note
// with that pitch somewhere on the grid which was turned on since the last time the switches were read.
// Returns true if at least one new switch was pressed.
int read_switches(char* newly_pressed_switches) {
  unsigned long now = millis();
  int return_val = false;
  digitalWrite(bypass, LOW); // deactivate bypass transistors
  
  for (int r = 0; r < 15; r++) {
    digitalWrite(rows[r], HIGH); // activate this row
    
    int note_recently_pressed = false;
    for (int c = 0; c < 32; c++) {
      note_recently_pressed |= get_button_posedge(columns[c], &(switch_value[c][r]), &(switch_value_last_change[c][r]), now);
    }
    newly_pressed_switches[r] = note_recently_pressed;
    return_val |= note_recently_pressed;
    
    digitalWrite(rows[r], LOW); // deactivate this row
  }
  
  digitalWrite(bypass, HIGH); // reactivate bypass transistors
  
  return return_val;
}
    

void setup() {
  // Init pins
  
  for (int i = 0; i < 32; i++) {
    pinMode(columns[i], INPUT);
  }
  
  for (int i = 0; i < 15; i++) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], LOW);
  }
  
  pinMode(bypass, OUTPUT);
  digitalWrite(bypass, LOW);
  
  // Init globals
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 15; j++) {
      // NOTE: SWITCHES COULD ALREADY BE ON! AT SOME POINT WE NEED TO UPDATE THESE TO THEIR REAL VALUE!
      switch_value[i][j] = 0;
      
      switch_value_last_change[i][j] = 0;
    }
  }
  play_button_value = false;
  play_button_last_change = 0;
  
  // Init serial
  Serial.begin(115200);
}

void run_startup() {
  // TODO: implement me
}

// This is the mode we are in when no music is playing. The user is free to flip switches.
// When the user flips a switch, the corresponding note will play so that the user can hear
// what it sounds like. Once the user is satisfied with their composition, they can press the
// PLAY button to hear the song.
void run_compose() {
  while (true) {
    unsigned long now = millis();
    
    // if the play button was just pressed, then switch to play mode and exit
    if (get_button_posedge(play_button, &play_button_value, &play_button_last_change, now)) {
      current_mode = PLAY;
      return;
    }
    
    // Quickly read the switches, checking for any new switch presses.
    char new_notes[15];
    int new_switch_pressed = read_switches(new_notes);
    // If there were new switch presses, then send them to the Pi!
    if (new_switch_pressed) {
      send_notes(new_notes);
    }
    
    // Delay. During this time, the switches are being fully lit. So we can't make this too small or else
    // the switches will look dim. But we also can't make it too large, or we won't detect new switch presses
    // quickly enough.
    delay(100); // iunno
  }
}

void run_play() {
  // TODO: implement me
}

void run_share() {
  // TODO: implement me
}

void loop() {
  switch (current_mode) {
    STARTUP: run_startup(); break;
    COMPOSE: run_compose(); break;
    PLAY: run_play(); break;
    SHARE: run_share(); break;
  }
}

//void loop() {
//  pinMode(6, INPUT);
//  pinMode(7, INPUT);
//  
//  int x1, x2, y1, y2, x3;
//  
//  digitalWrite(11, LOW);
//  digitalWrite(12, HIGH);
//  //delay(1);
//  x1 = digitalRead(6);
//  x1 = digitalRead(6);
//  y1 = digitalRead(7);
//  y1 = digitalRead(7);
//  
//  digitalWrite(12, LOW);
//  digitalWrite(11, HIGH);
//  //delay(1);
//  x2 = digitalRead(6);
//  x2 = digitalRead(6);
//  y2 = digitalRead(7);
//  y2 = digitalRead(7);
//  
//  for (int i = 0; i < 30; i++ ) {
//    digitalWrite(11, LOW);
//    digitalWrite(12, LOW);
//    x3 = digitalRead(6);
//    x3 = digitalRead(6);
//    x3 = digitalRead(7);
//    x3 = digitalRead(7);
//  }
//  
//  pinMode(6, OUTPUT);
//  pinMode(7, OUTPUT);
//  digitalWrite(6, LOW);
//  digitalWrite(7, LOW);
//  digitalWrite(11, HIGH);
//  digitalWrite(12, HIGH);
//  
//  Serial.println("--------------------------");
//  Serial.print(x1);
//  Serial.print("    ");
//  Serial.println(y1);
//  Serial.print(x2);
//  Serial.print("    ");
//  Serial.println(y2);
//  
//  delay(1000);
  
  
  
  
  
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
//}
