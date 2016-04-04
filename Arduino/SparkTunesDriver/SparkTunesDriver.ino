// Pin Assignments
// pin numbers of the columns
const int columns[32] = {
  22, 24, 26, 28, 30, 32, 34, 36, 
  38, 40, 42, 44, 46, 48, 50, 52,
  23, 25, 27, 29, 31, 33, 35, 37,
  39, 41, 43, 45, 47, 49, 51, 53
  };
// pin numbers of the rows
const int rows[15] = {
  14, 15, 16, 17, 18, 19, 20, 21,  //can't use 0-7, because pins 0 and 1 are used for serial transmission
  8, 9, 10, 11, 12, 13,
  A8
};
// Bypass signal to activate NMOS transistors
const int bypass = A15;
// Play button
const int play_button = A7;
// Tempo input
const int tempo_pot = A0; // NEEDS TO BE ANALOG PIN
// End Pin Assignments

// Other Constants
const int DEBOUNCE_DELAY = 200; // ms
const char* note_names[15] = { "C4", "D4", "E4", "F4", "G4", "A5", "B5", "C5", "D5", "E5", "F5", "G5", "A6", "B6", "C6" };
enum MODE { STARTUP, COMPOSE, PLAY, SHARE };
// Tempo-related constants
const unsigned long RES_MIN = 0;
const unsigned long RES_MAX = 1000000;
const unsigned long WAIT_MIN = 250; // corresponds to bmp of 240
const unsigned long WAIT_MAX = 2000; // corresponds to bmp of 30
// How many times to play a song before automatically stopping
const int MAX_PLAY_TIMES = 5;

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
MODE current_mode = STARTUP;
//flag to indicate if raspberry pi is ready to receive signal
bool piReady = false;
// End Global Variables

// Test the transistors controlling the rows
void test_rows(){
  for(int i=0; i<15; i++)
    digitalWrite(rows[i], HIGH);
  delay(1500);
  for(int i=0; i<15; i++)
    digitalWrite(rows[i], LOW);
  delay(1500);
}

// Test the transistors controlloing the columns
void test_columns(){
  for(int i=0; i<32; i++)
    digitalWrite(columns[i], HIGH);
  delay(1500);
  for(int i=0; i<32; i++)
    digitalWrite(columns[i], LOW);
  delay(1500);
}


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

// Reads the entire switch array, without taking into account debouncing or previous values or anything complicated
int basic_read_switches() {
  digitalWrite(bypass, LOW); // deactivate bypass transistors
  
  for (int r = 0; r < 15; r++) {
    digitalWrite(rows[r], HIGH); // activate this row
    for (int c = 0; c < 32; c++) {
      switch_value[c][r] = digitalRead(columns[c]);
    }
    digitalWrite(rows[r], LOW); // deactivate this row
  }
  
  digitalWrite(bypass, HIGH); // reactivate bypass transistors
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

// gets the tempo, in # milliseconds to wait between notes
int get_tempo() {
  // Assuming the circuit is 5V --- 1M ---(probe here)--- POT --- GND
  // Assuming POT varies linearly from RES_MIN to RES_MAX
  // Assuming our wait time should also vary linearly from WAIT_MIN to WAIT_MAX
  // Assuming larger bmp => faster
  // V = 5V * POT / (POT + 1M)
  // V(POT + 1M) = 5 * POT
  // V * 1M = (5 - V) * POT
  // POT = (V * 1M) / (5 - V)
  // POT = (V_arduino*5/1024 * 1M) / (5 - V_arduino*5/1024)
  // POT = (V_arduino * 1M) / (1024 - V_arduino)
  
  // Get current voltage and convert to scaled resistance
  int voltage = analogRead(tempo_pot);
  if (voltage < 0) voltage = 0; // is this even possible?
  if (voltage >= 1024) voltage = 1023; // is this even possible?
  unsigned long pot_value = (voltage * 1000000) / (1024 - voltage);
  
  if (pot_value < RES_MIN) pot_value = RES_MIN;
  if (pot_value > RES_MAX) pot_value = RES_MAX;
  
  return WAIT_MAX - ((pot_value - RES_MIN) * (WAIT_MAX - WAIT_MIN) / (RES_MAX - RES_MIN));
}

//checks to see if the Raspberry Pi is ready
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
  pinMode(play_button, INPUT);
  pinMode(tempo_pot, INPUT);
  
  // Init globals
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 15; j++) {
      switch_value_last_change[i][j] = 0;
    }
  }
  basic_read_switches(); // sets initial switch values
  play_button_value = false;
  play_button_last_change = 0;
  
  // Init serial
  Serial.begin(115200);
}

void run_startup() {
// for circuitry
//  test_rows();
//  test_columns();
  test_switches();
  basic_read_switches();
  
  for(int i=0; i<15; i++){
    Serial.print("Testing row: ");
    Serial.print(i);
    for(int j=0; j<32; j++){
      Serial.print(switch_value_last_change[i][j]);
    }
    Serial.println();
  }

//  run_compose();

}

//used to check if all the switches are working (the switches need to be on)
void test_switches(){
  for(int row=0 ; row<15; row++){
    digitalWrite(rows[row], HIGH);
    delay(500);
    digitalWrite(rows[row], LOW);
  }
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
    delay(50); // iunno
  }
}

// In this mode, we play the current song the user has written.
// We stop when either:
//   a) we've played the song the maximum # of times
//   b) the user presses the play button again
void run_play() {
  for (int i = 0; i < MAX_PLAY_TIMES; i++) {
    for (int beat = 0; beat < 32; beat++) {
      unsigned long start_beat = millis();
      
      // if the play button was just pressed, then stop playing and switch back to compose mode
      if (get_button_posedge(play_button, &play_button_value, &play_button_last_change, start_beat)) {
        current_mode = COMPOSE;
        return;
      }
      
      // Read the switches
      basic_read_switches();
      
      // Get the current tempo
      int tempo = get_tempo();
      
      // Play the notes in the current beat
      char current_notes[15];
      for (int note = 0; note < 15; note++) {
        current_notes[note] = switch_value[beat][note];
      }
      send_notes(current_notes);
      unsigned long after_send = millis();
      
      unsigned long time_to_wait = tempo - (after_send - start_beat);
      if (time_to_wait > 0) delay(time_to_wait);
    }
  }
}

void run_share() {
  // TODO: implement me
}

void loop() {
  //IMPORTANT: if you are testing through a computer and not the Pi, 
  //you must send "Ready" through the serial monitor
  
  if(!piReady){
   isRaspberryPiReady();
  }
  else{
    if(current_mode == STARTUP)
      run_startup();
    else if(current_mode == COMPOSE)
      run_compose();
    else if(current_mode == PLAY)
      run_play();
    else if(current_mode == SHARE)
      run_share();
    else
      run_startup(); 
  }
}

