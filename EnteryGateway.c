#include <Wire.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <Servo.h>   

const int IR_recv_pin = 6;
const int open_entry_pin = 7;
const int floor_capacity = 2;
const int floors = 2;
const int total_capacity = floors * floor_capacity;
const double going_in = 16601263;
int parked = 0, counter = 0;
int next_floor = floors, next_spot = floor_capacity;
int time[12];
int next_out_index = 0;
decode_results results;
Servo servo;

IRrecv irrecv(IR_recv_pin);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  Wire.begin(1);
  Wire.onRequest(on_going_out);
  irrecv.enableIRIn(); 
  lcd.begin(16, 2);
  pinMode(open_entry_pin, OUTPUT);
  set_lcd();
  servo.attach(8);
  servo.write(0);
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value == going_in && parked < total_capacity){
      int index = floor_capacity * (floors + 1 - next_floor) - next_spot;
      time[index] = millis() / 1000;
      if(time[index] == 0) time[index]++;
      servo.write(90); 
      digitalWrite(open_entry_pin, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Floor ");
      lcd.print(next_floor);
      lcd.setCursor(0, 1);
      lcd.print("Spot  ");
      lcd.print(next_spot);
      parked++;
      if(next_spot > 1) 
        next_spot--;
      else if(next_floor > 1){
        next_spot = floor_capacity;
        next_floor--;
      } else {
        next_spot = floor_capacity;
        next_floor = floors;
      }
      delay(500);//TODO 15000
      servo.write(0); 
      digitalWrite(open_entry_pin, LOW);
      set_lcd();
    }
    irrecv.resume();
  }
  
}

void on_going_out() {
  if(parked == 0 || time[next_out_index] == 0) Wire.write(0);
  else { 
    parked--;
    set_lcd();
    Wire.write(time[next_out_index]);
    next_out_index = (next_out_index + 1) % total_capacity;
  }
}

void set_lcd() {
  lcd.clear();
  lcd.print("Capacity ");
  lcd.print(total_capacity - parked);
  lcd.setCursor(0, 1);
  lcd.print("Parked   ");
  lcd.print(parked);
}
 