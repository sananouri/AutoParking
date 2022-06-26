#include <Wire.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <Servo.h> 

const int IR_recv_pin = 6;
const int open_entry_pin = 7;
const int fee = 1;
const double going_out = 16584943;
const double payed = 16617583;
decode_results results;
bool waiting_for_payment = false;
Servo servo;

IRrecv irrecv(IR_recv_pin);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
  Wire.begin();
  irrecv.enableIRIn(); 
  pinMode(open_entry_pin, OUTPUT);
  servo.attach(8);
  servo.write(0);
}

void loop(){
  if (irrecv.decode(&results)){
    if (results.value == going_out && !waiting_for_payment){
      Wire.requestFrom(1, 1); 
      if(Wire.available()) {
        int from = Wire.read();
        if(from != 0){
          int to = millis() / 1000;
          int time = to - from;
          lcd.clear();
  		  lcd.setCursor(0,0);
          lcd.print("Fee ");
          lcd.print(fee);
          lcd.print("$ Time ");
          lcd.print(time);
          lcd.setCursor(0,1);
          lcd.print("Total ");
          lcd.print(time * fee);
          lcd.print("$");
          waiting_for_payment = true;
        }
      }
    } else if(results.value == payed && waiting_for_payment){
      waiting_for_payment = false;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Good Bye");
      digitalWrite(open_entry_pin, HIGH);
      servo.write(90);
      delay(500);//TODO 15000
      servo.write(0);
      digitalWrite(open_entry_pin, LOW);
      lcd.clear();
    }
    irrecv.resume();
  }
}
 