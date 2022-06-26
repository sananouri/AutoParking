// 2nd Floor 2nd spot
#include <IRremote.h>

const int ir_recv_pin = 11;
const int led_pin = 10;
const double spot = 16586983; //key 7
bool filled = false;
decode_results results;
IRrecv irrecv(ir_recv_pin);

void setup() {
  irrecv.enableIRIn();
  pinMode(led_pin, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value == spot){
      filled = !filled;
      if(filled)
        digitalWrite(led_pin, HIGH);
      else 
        digitalWrite(led_pin, LOW);
    } 
    irrecv.resume();
  } 
}
