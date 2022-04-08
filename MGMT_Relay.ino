/* Smart Door System
 * MGMT-3074
 * Yonghoon Kim, Jigarkumar Patel, Gbenga Jones
 * Rev0: 02/23/22
 * Rev1: 03/11/22
 * Rev2: 03/17/22
 * Rev3: 03/24/22
 * Rev4: 04/01/22
 */
//Bluetooth
#include<SoftwareSerial.h>
//Bluetooth Communication
SoftwareSerial bt(2,3);
char Incoming_value = 0;

//Motion Sensors
int Motion_F = 4;
int Motion_B = 5;
//Limit Switches
int Lim_F = 6;
int Lim_B = 7;
//Solenoid for control direction of motor
int SolA = 8;
int SolB = 9;
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

  //Set Sensor pins to inputs
  pinMode(Motion_F, INPUT);
  pinMode(Motion_B, INPUT);
  pinMode(Lim_F, INPUT);
  pinMode(Lim_B, INPUT);
  //Set solenoid pin to output
  pinMode(SolA, OUTPUT);
  pinMode(SolB, OUTPUT);
  //Set test LED (Bluetooth communication, Motion Sensor)
  pinMode(Test, OUTPUT);
  pinMode(G_LED,OUTPUT);
  Serial.println("========== MGMT-3074 =========");
  Serial.println("====== Smart Door System ======");
}

void Input_scan(){
   PX_F = digitalRead(Motion_F);
   PX_B = digitalRead(Motion_B);
   LS_F = digitalRead(Lim_F);
   LS_B = digitalRead(Lim_B);
}

void Motor_on_Backward(){ //Motor going Backward (Open door)
  Input_scan();
  while(LS_B == 1){
    Input_scan();
    digitalWrite(SolA, LOW);
    digitalWrite(SolB, HIGH);
    Serial.println("===== Motor Running - Backward =====");
    Serial.println("Motor runs until Limit switch is reached");
  }
    Motor_off(); //Motor power off
    Serial.println("Back Limit Switch was connected");
    Serial.println("===========Door is open===========");
    delay(500);
    Motor_on_Forward();
}

void Motor_on_Forward(){ //Motor going forward (Closed door)
  Input_scan();
  while(LS_F == 1){
   Input_scan();
   digitalWrite(SolA, HIGH);
   digitalWrite(SolB, LOW);
   Serial.println("===== Motor Running - Forward =====");
   Serial.println("Motor runs until the Limit switch is reached");
   Serial.println(LS_F);
  }
   Motor_off();  //Motor power off
   Serial.println("===========Door is closed===========");
   delay(500);
}

void Motor_off(){ //Motor Stopped
  digitalWrite(SolA, LOW);
  digitalWrite(SolB, LOW);
  Serial.println("Motor Stopped");
  delay(3000);
}

void loop(){
    digitalWrite(G_LED, LOW);
    if(!bt.available()){
    digitalWrite(G_LED, HIGH);
      Input_scan();
      if (PX_F == HIGH || PX_B == HIGH){
        digitalWrite(Test, HIGH);
        delay(500);
        Motor_on_Backward();
      }
      else if (PX_F == LOW && PX_B == LOW){
        digitalWrite(Test, LOW);
        Motor_off();
      }
    }
    else if(bt.available()){
    digitalWrite(G_LED, LOW);
    Serial.println("//////////Bluetooth is disconnected//////////");
    }
    Motor_off();
}
