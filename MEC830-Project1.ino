#include <Stepper.h>
#include <Servo.h>

const int ldr1 = A0;
const int ldr2 = A1;

const int step = 8;

int angle =0;
int event = 0;
const int tolerance = 12;
bool tracking = true;
int event_wait_time = 500;  //<------change to adjust the servo more

Servo servo;
Stepper stepper = Stepper(2048,11,9,10,8);  //(IN1,IN3,IN2,IN4)

void setup() {
  Serial.begin(9600);
  stepper.setSpeed(8); //speed control
  servo.attach(7);
  servo.write(angle);

  pinMode(ldr1, INPUT);
  pinMode(ldr2, INPUT);
  
  delay(2000);
}

void loop() {
  Serial.print("time: ");
  Serial.print(event);
  event++;
  Serial.print(", S_Angle: "); 
  Serial.print(angle);

  if (tracking = true) {
    if (analogRead(ldr1) - analogRead(ldr2) > tolerance){
     // stepper.step(-step);
      Serial.println("  CCW");
    } else if (analogRead(ldr2) - analogRead(ldr1) > tolerance){
    // stepper.step(step);
      Serial.println("  CW");
    } else {
      event +=1;
        
    }

  if (event > event_wait_time) {
    angle = returnPitch(angle);
    servo.write(angle);
    tracking = false;
    Serial.println("hold");
    event=0; 
  }

    
  } else if (tracking = false) {
     if (abs(analogRead(ldr1) - analogRead(ldr2)) > tolerance){
      event +=1;
     }
     if (event > event_wait_time) {
      tracking = true;
      Serial.println("tracking");
      event =0;
     } 
  }
}

int returnPitch(int angle){
  int maxLi = analogRead(ldr1) + analogRead(ldr2);
  int maxPitch = angle;
  
   //test response with smaller stepps (5 and 10)
  for (int i = 45; i <= 135; i += 5){

    Serial.println("SERVO----------------------");
    servo.write(i);
    
    if ((analogRead(ldr1) + analogRead(ldr2)) > maxLi){
      maxLi = analogRead(ldr1) + analogRead(ldr2);
      maxPitch = i; 
      
    }
    delay(50);
  }

  if (maxPitch> 15){
    maxPitch=15;
  }else if (maxPitch <-90){
    maxPitch=-90;
  }

  return maxPitch;
  
}
