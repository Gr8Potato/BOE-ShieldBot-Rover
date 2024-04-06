#include <Servo.h>                                                              // include servo library  
Servo servoLeft;                                                                // declare left and right servos
Servo servoRight;
 
void setup()                                                                    // built-in initialization block
{
  pinMode(13, INPUT);  pinMode(12, OUTPUT);                                     // left IR LED & Receiver
  pinMode(3, INPUT);  pinMode(2, OUTPUT);                                       // right IR LED & Receiver
  pinMode(8, OUTPUT); pinMode(7, OUTPUT);                                       // indicator LEDs
 
  tone(4, 3000, 1000);                                                          // play tone for 1 second
  delay(1000);                                                                  // delay to finish tone
 
  servoLeft.attach(11);                                                         // attach left signal to pin 13
  servoRight.attach(10);                                                        // attach right signal to pin 12
}  
 
void loop()                                                                     // main loop auto-repeats
{
 
  int irLeft = irDetect(12, 13, 44000);                                         // check for object on left
  int irRight = irDetect(2, 3, 44000);                                          // check for object on right
  digitalWrite(8, !irLeft);                                                     // LED states opposite of IR
  digitalWrite(7, !irRight);
  if((irLeft == 0) && (irRight == 0))                                           // if both sides detect
  {
    maneuver(-200, -200, 20);                                                   // backward 20 milliseconds
  }
  else if(irLeft == 0)                                                          // if only left side detects
  {
    maneuver(200, -200, 20);                                                    // right for 20 ms
  }
  else if(irRight == 0)                                                         // if only right side detects
  {
    maneuver(-200, 200, 20);                                                    // left for 20 ms
  }
  else                                                                          // otherwise, no IR detects
  {
    maneuver(200, 200, 20);                                                     // forward 20 ms
  }
}
 
int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);                                                  // IRLED 38 kHz for at least 1 ms
  delay(1);                                                                      // wait 1 ms
  int ir = digitalRead(irReceiverPin);                                           // IR receiver -> ir variable
  delay(1);                                                                      // down time before recheck
  return ir;                                                                     // return 1 no detect, 0 detect
}  
 
void maneuver(int speedLeft, int speedRight, int t)
{
  // speedLeft, speedRight ranges: Backward  Linear  Stop  Linear   Forward
  //                               -200      -100......0......100       200
  servoLeft.writeMicroseconds(1500 + speedLeft);                                 // set left servo speed
  servoRight.writeMicroseconds(1500 - speedRight);                               // set right servo speed
  if(t == -1)                                                                    // if time = -1
  {                                  
    servoLeft.detach();                                                          // stop servo signals
    servoRight.detach();   
  }
  delay(t);                                                                      // delay for t
}
