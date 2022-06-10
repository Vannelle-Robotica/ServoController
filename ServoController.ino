#include "Arduino.h"
#include "AX12A.h"
#include <Wire.h>

#define DirectionPin   (2u)
#define BaudRate      (1000000ul)
#define ID1        (32u)
#define ID2        (1u)
#define ID3        (14u)

//Id's of the servo's 
int servos[] = {32, 1}; 
//Id of the vacuum
int vacuum = 6;

void setup()
{
  // Initialize serial & servo's
  Serial.begin(10000000);
  ax12a.begin(BaudRate, DirectionPin, &Serial);

  // Disable endless for all servo's
  for (int servo: servos)
     ax12a.setEndless(servo, OFF);

  // Join I2C bus as slave with address 8
  Wire.begin(0x8);
  
  // Call receiveEvent when data received                
  Wire.onReceive(receiveEvent);
}

void receiveEvent() {
  // Check if Wire is available and discard first byte
  if(!Wire.available()) return;
  Wire.read();

  // Parse id of servo from read wire
  int id = (int)Wire.read() - '0';
  if(id == vacuum){
    turnVaccuumOn();
    delay(5000);
    turnVaccuumOff();
  } else{
      int data;
    
      // Parse angle of servo from read wire
      while (Wire.available()) { 
        data *= 10;
        data += (int) Wire.read() - '0';
      }
      
      // Turn servo to requested angle 
      ax12a.move(servos[id], data);
    }
  }
}
 
void turnVaccuumOn() {
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);

  for (int i = 0; i < 256; i++) { 
    analogWrite(pin, i); 
    delay(20); 
    } 
}

void turnVaccuumOff() {
  digitalWrite(pin, LOW);
}

void loop(){
  
}
