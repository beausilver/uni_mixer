//////////////////////// 
//
// Beau Silver
// 10/24/14
// Please re-use code for ANY purpose
// (commercial, etc.)
//
////////////////////////

#include <CapacitiveSensor.h>

int faderSpeed = 3;  //PWM control for motor 1
int faderDirection = 2;  //direction control for motor 1
int softFader = 0;
int lastValue = 0;
int faderMax;
int faderMin;

int FAST = 255;
int STOP = 0;
int WIGGLE = 10;
int TOUCH_SENSE_THRESH = 2000;

CapacitiveSensor cs = CapacitiveSensor(6,7);

void setup() {
  Serial.begin(9600);
  pinMode(faderSpeed, OUTPUT);
  pinMode(faderDirection, OUTPUT);
  calibrateFader();
  analogWrite(faderSpeed, STOP); // init fader to not move 
}

void loop(){  

  if (Serial.available() > 0)
  {  
     // Serial fader messages coming in from middleware (Python, etc.)
     // Read from Serial
     char t[4];
     Serial.readBytesUntil('\n', t, 4);
     softFader = atoi(t);
    
     if (cs.capacitiveSensor(30) < TOUCH_SENSE_THRESH) // Fader is NOT touched
     {
    
       // Clip on both ends
       if (softFader > faderMax)
       {
         softFader = faderMax; 
       }
       if (softFader < faderMin)
       {
         softFader = faderMin;  
       }
       // print val
       Serial.println(softFader);

       if (analogRead(0) < softFader - WIGGLE)
       {
         while( analogRead(0) < softFader - WIGGLE)
         {
           digitalWrite(faderDirection,  HIGH); 
           analogWrite(faderSpeed, FAST);
         } 
         analogWrite(faderSpeed, STOP); 
       }
       else
       {
         while( analogRead(0) > softFader + WIGGLE)
         {
           digitalWrite(faderDirection, LOW); 
           analogWrite(faderSpeed, FAST); 
         } 
         analogWrite(faderSpeed, STOP); 
       }
   
    } // end cap sense
  } // end serial
  else
  {
    //Serial.println("No msgs on Serial");
  }
  
  int faderValue = analogRead(A0);
  if ( (faderValue > lastValue + 1) || (faderValue < lastValue - 1) )
  {
    Serial.println(faderValue);
    lastValue = faderValue;
  }
  
  delay(10);   
}

//Calibrates the min and max position of the fader
void calibrateFader() {
    analogWrite(faderSpeed, FAST); // Set speed to Fast
    //Send fader to the top and read max position
    digitalWrite(faderDirection, HIGH);
    delay(250);  
    faderMax = analogRead(0);
    Serial.println("Fader Max:");
    Serial.println(faderMax);
    
    //Send fader to the bottom and read max position
    digitalWrite(faderDirection, LOW);
    delay(250);
    faderMin = analogRead(0);
    Serial.println("Fader Min:");
    Serial.println(faderMin);
    
    analogWrite(faderSpeed, STOP); // Set speed to stop 
}
