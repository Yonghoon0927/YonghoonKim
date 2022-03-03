/* Smart Door System
 * MGMT-3074
 * Yonghoon Kim, Jigarkumar Patel, Gbenga Jones
 * Rev0: 02/23/22
 */

//Bluetooth Communication
int blue_signal = 0;
//Motor Control
int enable = 3; // PWM Control(Motor speed)
int motor_in1 = 4;
int motor_in2 = 5;
//Motion Sensors
int Motion_F = 6;
int Motion_B = 7;
//Limit Switches
int Lim_F = 8;
int Lim_B = 9;
//Solenoid lock control
int Sol = 10;

void setup(){
  Serial. begin(9600);
  
  //Set Motor Control pins to outputs
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(Motion_F, INPUT);
  pinMode(Motion_B, INPUT);
  pinMode(Lim_F, INPUT);
  pinMode(Lim_B, INPUT);
  pinMode(Sol, OUTPUT);
}

void Motor_on_Backward(){ //Motor going Backward (Open door)
  if (Lim_B == LOW){
    Door_open(); //Solenoid retracts
    delay(1000); //Delay time for solenoid retracting
    digitalWrite(motor_in1, HIGH);
    digitalWrite(motor_in2, LOW);
    analogWrite(enable, 50); //PWM Control, Set speed to 50 out of possible range 0~255
  }
  else if (Lim_B == HIGH)
    Motor_off(); //Motor power off
    delay(500);  //Delay time to prevent motor overload
}

void Motor_on_Forward(){ //Motor going forward (Closed door)
  if (Lim_F == LOW){
   digitalWrite(motor_in1, LOW);
   digitalWrite(motor_in2, HIGH);
   analogWrite(enable, 50); //Set speed to 50 out of possible range 0~255
  }
  else if (Lim_F == HIGH)
   Motor_off();  //Motor power off
   delay(500);   //Delay time to prevent motor overload
   Door_close(); //Solenoid advances
}

void Motor_off(){ //Motor Stopped
  digitalWrite(motor_in1, LOW);
  digitalWrite(motor_in2, LOW);
}

void Door_open(){ //Solenoid is disabled
   digitalWrite(Sol, LOW);
}

void Door_close(){ //Solenoid is activated
   digitalWrite(Sol, HIGH);
}

void loop(){
  if (Serial.available()>0){ //Send data only when you receive data:
    blue_signal = Serial.read();
    Serial.print(blue_signal); // Checking the bluetooth connection on Serial Monior screen
    Serial.print("|n");

   if (blue_signal == 1){
      if (Motion_F == HIGH || Motion_B == HIGH){
        Motor_on_Backward();
      }
     else if (Motion_F == LOW && Motion_B == LOW){
        delay(10000); //after 10seconds, door will be automatically closed
        Motor_on_Forward();
      }
   }
  else if (blue_signal == 0){
     delay(10000);  //after 10seconds, door will be automatically closed
     Motor_on_Forward();
  }
  }
}
