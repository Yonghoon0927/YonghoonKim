/* Smart Door System
 * MGMT-3074
 * Yonghoon Kim, Jigarkumar Patel, Gbenga Jones
 * Rev0: 02/23/22
 * Rev1: 03/11/22
 * Rev2: 03/17/22
 * Rev3: 03/24/22
 */
//Bluetooth
#include<SoftwareSerial.h>
//Bluetooth Communication
SoftwareSerial bt(2,3);

char Incoming_value = 0;
//Motor Control
int enable = 9; // PWM Control(Motor speed)
int motor_in1 = 10;
int motor_in2 = 11;
//Motion Sensors
int Motion_F = 4;
int Motion_B = 5;
//Limit Switches
int Lim_F = 6;
int Lim_B = 7;
//Solenoid lock control
int Sol = 8;
//Test Port
int G_LED =12;
int Test = 13;
// variants
int PX_F = 0;
int PX_B = 0;
int LS_F = 0;
int LS_B = 0;

void setup(){
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(G_LED,OUTPUT);
  //Set Motor Control pins to outputs
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
  pinMode(enable, OUTPUT);
  //Set Sensor pins to inputs
  pinMode(Motion_F, INPUT);
  pinMode(Motion_B, INPUT);
  pinMode(Lim_F, INPUT);
  pinMode(Lim_B, INPUT);
  //Set solenoid pin to output
  pinMode(Sol, OUTPUT);
  pinMode(Test, OUTPUT);
}

void Input_scan(){
   PX_F = digitalRead(Motion_F);
   PX_B = digitalRead(Motion_B);
   LS_F = digitalRead(Lim_F);
   LS_B = digitalRead(Lim_B);
}

void Motor_on_Backward(){ //Motor going Backward (Open door)
  Door_open(); //Solenoid retracts
  Input_scan();
  while(LS_B == 0){
    Input_scan();
    digitalWrite(motor_in1, HIGH);
    digitalWrite(motor_in2, LOW);
    Serial.println("Backward");
    Serial.println(LS_B);
    analogWrite(enable, 200); //PWM Control, Set speed to 50 out of possible range 0~255
  }
    Motor_off(); //Motor power off
    delay(1000);
    Motor_on_Forward();
}

void Motor_on_Forward(){ //Motor going forward (Closed door)
  Input_scan();
  while(LS_F == 0){
   Input_scan();
   digitalWrite(motor_in1, LOW);
   digitalWrite(motor_in2, HIGH);
   Serial.println("Forward");
   Serial.println(LS_F);
   analogWrite(enable, 200); //Set speed to 50 out of possible range 0~255
  }
   Motor_off();  //Motor power off
   Door_close(); //Solenoid advances
}

void Motor_off(){ //Motor Stopped
  digitalWrite(motor_in1, LOW);
  digitalWrite(motor_in2, LOW);
  Serial.println("Off");
  delay(1000);
}

void Door_open(){ //Solenoid is disabled
   digitalWrite(Sol, HIGH);
   Serial.println("Door Open");
   Serial.println("");
}

void Door_close(){ //Solenoid is activated
   digitalWrite(Sol, LOW);
   Serial.println("Door Closed");
   Serial.println("");
}

void loop(){
    if(bt.available()){
    digitalWrite(G_LED, HIGH);
      Input_scan();
      if (PX_F == HIGH || PX_B == HIGH){
        digitalWrite(Test, HIGH);
        Serial.println("Waiting");
        Motor_on_Backward();
      }
      else if (PX_F == LOW && PX_B == LOW){
        digitalWrite(Test, LOW);
        Motor_off();
      }
    }
    else if(!bt.available()){
    digitalWrite(G_LED, LOW);
    }
    Motor_off();
}
