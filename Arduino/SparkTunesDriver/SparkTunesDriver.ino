#include <Adafruit_NeoPixel.h>
#include "SparkTunes_LedStrips.h"

// Pin Assignments
// pin numbers of the columns
const int columns[32] = {
  22, 24, 26, 28, 30, 32, 34, 36, //panel 1 
  38, 40, 42, 44, 46, 48, 50, 52,  //panel 2 
  23, 25, 27, 29, 31, 33, 35, 37, //panel 3 
  39, 41, 43, 45, 47, 49, 51, 53, //panel 4
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
const int play_button = 7;
// Tempo input
const int tempo_pot = A0; // NEEDS TO BE ANALOG PIN
// Adafruit LED strips
const int LEDstripPin = 4;

// End Pin Assignments

// Other Constants
//const int LEDS_PER_STRIP = 30;
//const int NUM_STRIPS = 2;
const long DEBOUNCE_DELAY = 200; // ms
const char* note_names[15] = { "C3", "D3", "E3", "F3", "G3", "A3", "B3", "C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5"};
enum MODE { STARTUP, COMPOSE, PLAY, SHARE };
// Tempo-related constants
const unsigned long RES_MIN = 0;
const unsigned long RES_MAX = 1000000;
const unsigned long WAIT_MIN = 100; // corresponds to bmp of quarter = 150, if our notes are sixteenths
const unsigned long WAIT_MAX = 600; // 500 corresponds to bmp of quarter = 30, if our notes are sisteenths. Add a little bit because reasons.
// How many times to play a song before automatically stopping
const int MAX_PLAY_TIMES = 5;
const int TAKE_BREAK_AFTER_ROWS = 1;
const int BREAK_LENGTH = 5; // ms
const int READ_DELAY = 24;

// End Other Constants

// Global Variables
//Adafruit LED strips
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS_PER_STRIP*NUM_STRIPS, LEDstripPin, NEO_GRB+NEO_KHZ800);

// 1 == on, 0 == off. Indexed first by column, then by row
char switch_value[32][15];
// time (in ms since startup) of last switch value change. For debouncing
unsigned long switch_value_last_change[32][15];
// play button variables
char play_button_value;
unsigned long play_button_last_change;
// what mode are we in?
MODE current_mode = COMPOSE;
//flag to indicate if raspberry pi is ready to receive signal
bool piReady = false;

//debug flag
bool debug = true;
bool circuitry_test = false;
// End Global Variables

// print only if we're debugging 
void debug_print(String message){
  if(debug)
    Serial.println(message);
}


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
  if (pin == play_button) {
    Serial.print("Last time: ");
    Serial.println(*last_updated);
    Serial.print("Now: ");
    Serial.println(now);
  }
  if ( (now - *last_updated) < DEBOUNCE_DELAY) {
    // debouncing
    return false;
  }
  
  char new_value = digitalRead(pin);
  if (pin == play_button) {
    Serial.print("Current: ");
    Serial.println(new_value);
    Serial.print("Previous: ");
    Serial.println(*value);
  }
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
  debug_print("Notes: " + String(notes) );
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
  // Kill everything
  for (int r = 0; r < 15; r++) {
    digitalWrite(rows[r], LOW);
  }
  digitalWrite(bypass, LOW); // deactivate bypass transistors
  delay(1); // remove me!

  for (int r = 0; r < 15; r++) {
    digitalWrite(rows[r], HIGH); // activate this row
    
    // read a bunch of stuff for no reason
    for (int x = 0; x < READ_DELAY; x++) {
      digitalRead(rows[r]);
    }
    
    for (int c = 0; c < 32; c++) {
      switch_value[c][r] = (digitalRead(columns[c]) == HIGH);
    }
    digitalWrite(rows[r], LOW);
    
    if ((r+1) % TAKE_BREAK_AFTER_ROWS == 0) {
      // Take a break!
      for (int r = 0; r < 15; r++) {
        digitalWrite(rows[r], HIGH);
      }
      digitalWrite(bypass, HIGH); // reactivate bypass transistors
      delay(1); // remove me!
      
      delay(BREAK_LENGTH);
      
      // Okay back to work!
      digitalWrite(bypass, LOW); // deactivate bypass transistors
      delay(1); // remove me!
      for (int r = 0; r < 15; r++) {
        digitalWrite(rows[r], LOW);
      }
    }
  }
  
  // Restart everything
  for (int r = 0; r < 15; r++) {
    digitalWrite(rows[r], HIGH);
  }
  digitalWrite(bypass, HIGH); // reactivate bypass transistors
  delay(1); // remove me!
}

// Reads the entire switch array, all at once.
// Updates newly_pressed_switches[15] array such that each entry will be 1 if there is a note
// with that pitch somewhere on the grid which was turned on since the last time the switches were read.
// Returns true if at least one new switch was pressed.
int read_switches(char* newly_pressed_switches) {
  unsigned long now = millis();
  int return_val = false;
  
  // Kill everything
  for (int r = 0; r < 15; r++) {
    digitalWrite(rows[r], LOW);
  }
  digitalWrite(bypass, LOW); // deactivate bypass transistors
  delay(1); // remove me!
  
  for (int r = 0; r < 15; r++) {
    digitalWrite(rows[r], HIGH); // activate this row
    
    // read a bunch of stuff for no reason
    for (int x = 0; x < READ_DELAY; x++) {
      digitalRead(rows[r]);
    }
    
    int note_recently_pressed = false;
    for (int c = 0; c < 32; c++) {
      //delay(1);
      note_recently_pressed |= get_button_posedge(columns[c], &(switch_value[c][r]), &(switch_value_last_change[c][r]), now);
    }
    newly_pressed_switches[r] = note_recently_pressed;
    return_val |= note_recently_pressed;
    
    digitalWrite(rows[r], LOW); // deactivate this row
    
    if ((r+1) % TAKE_BREAK_AFTER_ROWS == 0) {
      // Take a break!
      for (int r = 0; r < 15; r++) {
        digitalWrite(rows[r], HIGH);
      }
      digitalWrite(bypass, HIGH); // reactivate bypass transistors
      delay(1); // remove me!
      
      delay(BREAK_LENGTH);
      
      // Okay back to work!
      digitalWrite(bypass, LOW); // deactivate bypass transistors
      delay(1); // remove me!
      for (int r = 0; r < 15; r++) {
        digitalWrite(rows[r], LOW);
      }
    }
  }
  
  // Restart everything
  for (int r = 0; r < 15; r++) {
    digitalWrite(rows[r], HIGH);
  }
  digitalWrite(bypass, HIGH); // reactivate bypass transistors
  delay(1); // remove me!
  
//  int row = 0;
//  Serial.print("Row: " + String(row)+ " ");
//  for(int c=0; c<32; c++){
//    Serial.print(switch_value[c][row]);
//    Serial.print("-");
//  }
//  Serial.println();
  
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
  double pot_value = ((double)voltage * 5/1024)/ (5 - ((double)voltage*5/1024)) *1000000;
  
  if (pot_value < RES_MIN) pot_value = RES_MIN;
  if (pot_value > RES_MAX) pot_value = RES_MAX;
  
  int tempo = WAIT_MIN + ((pot_value - RES_MIN) * (WAIT_MAX - WAIT_MIN) / (RES_MAX - RES_MIN));
  //Serial.print("Tempo: ");
  //Serial.println(tempo);
  //Serial.print("Voltage: ");
  //Serial.println(voltage);

  return tempo;
}

//checks to see if the Raspberry Pi is ready
void isRaspberryPiReady(){
  if(Serial.available() > 0){
    String message = Serial.readString();
    if(message = "Ready"){
      piReady = true;
      ledStrips_setColour(&strip, strip.Color(0, 255, 0) );
    }
    else{
      piReady = false; 
      ledStrips_setColour(&strip, strip.Color(0, 0, 255) );
    }
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
  pinMode(play_button, INPUT_PULLUP);
  pinMode(tempo_pot, INPUT);

  // Init led strips
  strip.begin();
  strip.show();

  //test strips
  ledStrips_test(&strip);
  
  // Init globals
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 15; j++) {
      switch_value_last_change[i][j] = 0;
    }
  }
  basic_read_switches(); // sets initial switch values
  play_button_value = true;
  play_button_last_change = 0;

  // Init serial
  Serial.begin(115200);
  debug_print("Debug mode ON");
}

int basic_check_row(int row) {
  digitalWrite(bypass, LOW); // deactivate bypass transistors
  delay(1); // remove me!

  while (true) {

    for (int r = 0; r < 15; r++) {
      digitalWrite(rows[r], row == r ? HIGH : LOW);
    }
    
    for (int c = 0; c < 32; c++) {
      switch_value[c][row] = (digitalRead(columns[c]) == HIGH) ? '1' : '0';
    }
  
    Serial.print("Row: " + String(row)+ " ");
    for(int c=0; c<32; c++){
      Serial.print(switch_value[c][row]);
      Serial.print("-");
    }
    Serial.println();
  }
  
  //digitalWrite(bypass, HIGH); // reactivate bypass transistors
}

int basic_check_all_rows() {
  digitalWrite(bypass, LOW); // deactivate bypass transistors
  delay(1); // remove me!

  int row = 0;

  while (true) {

    if (Serial.available() > 0) {
      String s = Serial.readString();
      row = s.toInt();
    }

    for (int r = 0; r < 15; r++) {
      digitalWrite(rows[r], row == r ? HIGH : LOW);
    }
    
    for (int c = 0; c < 32; c++) {
      switch_value[c][row] = (digitalRead(columns[c]) == HIGH) ? '1' : '0';
    }
  
    Serial.print("Row: " + String(row)+ " ");
    for(int c=0; c<32; c++){
      Serial.print(switch_value[c][row]);
      Serial.print("-");
    }
    Serial.println();
  }
  
  //digitalWrite(bypass, HIGH); // reactivate bypass transistors
}

void run_startup() {
  int circuitry_test = false;
  if(circuitry_test)
    run_circuitry_test();
  
  test_switches();
  basic_check_all_rows();

  delay(100);

  //just want to make sure it's reading the right value
  print_switchboard_results();
}

//

void print_switchboard_results(){
  int column_to_test = 31;
  for(int c=0; c<32; c++){
    Serial.print("Column: " + String(c)+ " ");
    for(int r=0; r<15; r++){
      Serial.print(switch_value[c][r]);
      Serial.print("-");
    }
    Serial.println();
  }
  Serial.println();
}

//this is used to check if the transistors are properly working 
void run_circuitry_test(){
  test_rows();
  digitalWrite(bypass, HIGH);
  delay(1); // remove me!
  debug_print("Testing bypass");
  delay(500);
  digitalWrite(bypass, LOW);
  delay(1); // remove me!
}

//used to check if all the switches are working (the switches need to be on)
void test_switches(){
  while (true) {
    for(int row=0 ; row<15; row++){
      digitalWrite(rows[row], HIGH);
      delay(500);
      digitalWrite(rows[row], LOW);
    }
  }
}

// This is the mode we are in when no music is playing. The user is free to flip switches.
// When the user flips a switch, the corresponding note will play so that the user can hear
// what it sounds like. Once the user is satisfied with their composition, they can press the
// PLAY button to hear the song.
void run_compose() {
  char ignored_notes[15];
  read_switches(ignored_notes);
  while (true) {
    debug_print("run_compose");
    unsigned long now = millis();
    
    // if the play button was just pressed, then switch to play mode and exit
    if (get_button_posedge(play_button, &play_button_value, &play_button_last_change, now)) {
      current_mode = PLAY;
      return;
    }

    ledStrips_displayComposeStatus(&strip);
    
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
    //delay(50); // iunno
  }
}

// In this mode, we play the current song the user has written.
// We stop when either:
//   a) we've played the song the maximum # of times
//   b) the user presses the play button again
void run_play() {
  debug_print("run_play");
  
  unsigned long start_beat = millis();
  // Play the first beat
  basic_read_switches();
  char current_notes[15];
  for (int note = 0; note < 15; note++) {
    current_notes[note] = switch_value[0][note];
  }
  send_notes(current_notes);
  
  unsigned long beat = 1;
  while (beat < 32 * MAX_PLAY_TIMES) { // play the rest of the beats
    
    // if the play button was just pressed, then stop playing and switch back to compose mode
    if (get_button_posedge(play_button, &play_button_value, &play_button_last_change, millis())) {
      current_mode = COMPOSE;
      return;
    }
    
    // Read the switches
    basic_read_switches();
    
    // Get the current tempo
    int tempo = get_tempo();
    
    if ((millis() - start_beat) >= tempo) {
      start_beat = millis();

  for (int i = 0; i < MAX_PLAY_TIMES; i++) {
    ledStrips_displayIterations(&strip, i);
    debug_print("------Iteration: " + String(i) + "-------");
    for (int beat = 0; beat < 32; beat++) {
      unsigned long start_beat = millis();
      
      //start flashing the led corresponding to the beat here
      //TODO: call function
      
      // if the play button was just pressed, then stop playing and switch back to compose mode
      if (get_button_posedge(play_button, &play_button_value, &play_button_last_change, start_beat)) {
        current_mode = COMPOSE;
        return;
      }
      
      // Read the switches
      basic_read_switches();
      
      // Get the current tempo
      int tempo = get_tempo();
      
>>>>>>> cbab949699e0cd3f3dda6384dc4cf7f720b78ccf
      // Play the notes in the current beat
      char current_notes[15];
      for (int note = 0; note < 15; note++) {
        current_notes[note] = switch_value[beat % 32][note];
      }
      send_notes(current_notes);
      beat++;
    }
  }
    
  current_mode = COMPOSE;
}

void run_share() {
  // TODO: implement me
}

void loop() {
  //IMPORTANT: if you are testing through a computer and not the Pi, 
  //you must send "Ready" through the serial monitor
  
  //basic_check_all_rows();
  
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

