#include "Arduino.h"
#include "AX12A.h"
#include <Wire.h>
#include "CytronMotorDriver.h"

#define DirectionPin   (2u)
#define BaudRate      (1000000ul)
#define ID1        (10u)
#define ID2        (62u)
#define ID3        (2u)
#define ID4        (4u)
#define ID5        (52u)
#define ID6        (14u)

// Configure the motor driver.
CytronMD motor(PWM_DIR, 3, 4);  // PWM = Pin 3, DIR = Pin 4.

//Id's of the servo's 
int servos[] = {10, 62, 2, 4, 52, 14}; 
//Id of the vacuum
int vacuum = 5;

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
//increase speed ad increasing speed
void turnVaccuumOn() {
  for (int i = 0; i < 256; i++) { 
      motor.setSpeed(i);
      delay(300 - i); 
    } 
}
//decrease speed ad decreasing speed
void turnVaccuumOff() {
    for (int i = 0; i < 256; i++) { 
      motor.setSpeed(-i);
      delay(300 - i); 
    }  
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
 


void loop(){
  
}
