/*
  This code is written for Arduino UNO. This code functions as a magnetic object locator.
  Author: Mitchell Davis
  Hardware Setup Created By Hadley Pound
  Hardware Casing and Design Done By Ben Mahdessian
*/


// This code sets up the lcd display
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // The may need to be changed
                                                            // depending on the setup
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define SCREEN_WIDTH 16
#define SCREEN_HEIGHT 2


// Stores constants for the number of sensors and leds that are connected and used
// by the Arduino
#define NUM_SENSORS 4
#define NUM_LEDS 4


// This variable sets the threshold at which the sensor is considered "activated"
#define SENSOR_THRESHOLD 600


// These store the analog sensor pins (may need to be altered if you use different pins),
// sensor outputs, and led pin variables (may need to be altered if you use different pins)
// NOTE: Although the sensors/leds should be layed out in a grid, they will be treated as a
// 1D Array. This is to simplify the logic and make this scale, but be aware that this is an
// assumption made during logic processing, just make sure if you edit this, that each led pin
// and each analog input are paired by their index
const int sensor_pins[NUM_SENSORS] = {A0, A1, A4, A5}; // MUST be updated if NUM_SENSORS changes
double sensor_output[NUM_SENSORS];
const int led_pins[NUM_LEDS] = {8,10,9,7}; // MUST be updated if NUM_LEDS changes


// This sets each led pin to be output
void setupPins(){
  for (int i = 0; i < NUM_LEDS; i++){
    pinMode(led_pins[i], OUTPUT);
  }
}


// This loops through each sensor pin and reads the analog output
void readSensorOutput(){
  for (int i = 0; i < NUM_SENSORS; i++){
    sensor_output[i] = analogRead(sensor_pins[i]);
  }
}


// This reads the sensors outputs, then using SENSOR_THRESHOLD decides whether
// each LED should be on or off
void setLEDOutput(){
  for (int i = 0; i < NUM_LEDS; i++){
    if (sensor_output[i] >= SENSOR_THRESHOLD){
      digitalWrite(led_pins[i], HIGH);
    }
    else{
      digitalWrite(led_pins[i], LOW);
    }
  }
}


// This uses the sensor outputs to display a locality message on the screen
void setDisplayText(){
  int num_abv_thrshld = 0;
  for (int i = 0; i < NUM_SENSORS; i++){
    if (sensor_output[i] >= SENSOR_THRESHOLD){
      num_abv_thrshld++;
    }
  }


  double percent_active = (double) num_abv_thrshld / (double) NUM_SENSORS;


  lcd.setCursor(0,0);


  if (percent_active < 0.25){
    lcd.print("No Signal");
  }
  else if (percent_active <= 0.3){
    lcd.print("Signal Aquired");
  }
  else if (percent_active <= 0.55){
    lcd.print("Close");
  }
  else if (percent_active <= 0.75){
    lcd.print("Very Close");
  }
  else if (percent_active <= 0.9){
    lcd.print("Almost There");
  }
  else{
    lcd.print("Perfect");
  }
}


// This initializes the system
void setup() {
  lcd.begin(SCREEN_WIDTH,SCREEN_HEIGHT);
  setupPins();
  readSensorOutput(); // must be called to initalize sensor_output
 }


// This runs the system
void loop() {
  readSensorOutput();
  setLEDOutput();
  setDisplayText();
  delay(3000);
  lcd.clear();
 }
