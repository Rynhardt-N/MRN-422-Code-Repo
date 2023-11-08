#include <Servo.h> //includes Servo library
Servo servo;                  //this creates a servo object for the servo library


//set Arduino pins for ultrasonic sensor and servo
const int PinServo = 11;
const int PinEcho = 9;
const int PinTrig = 10;
const int numRead = 2;
int index = 0;
int val = 0;
int sum = 0;
int readings[numRead];      //Creates array for readings to be stored in
int ave = 0;

//Define Variables
//Ultrasonic
long duration;                //the amount of time it took for the sound to return to the sensor
int distance;                 //x location of the ball

//Servo
int pos = 0;                  //initial value for current servo position variable
double StartAngle = 87;       //the start angle of the servo is the angle at which the beam is horizontal
double Perror,Ierror,Derror;  //the PID error values
double output;                //combined error values which have been multiplied by PID gains
double last_error;            //the error from the last iteration of the code

//Time
unsigned long last_time;      //time from last iteration of the code
unsigned long now;            //Current time
double dt;                    //Change in time

//Setpoint
int setpoint = 15;             //the point on the beam that the ball should settle at

//PID Coefficient
float Kp = 6;                 //Proportional coefficient
float Ki = 0.0005;            //Integral coefficient
float Kd = 7;                 //Derivative coefficient

void setup() {
  //for ultrasonic sensor
  pinMode(PinTrig, OUTPUT);   //designates the trigger pin as an output pin
  pinMode(PinEcho, INPUT);    //designates the echo pin as an input pin

  //for servo
  servo.attach(PinServo);     //attaches servo object to the correct pin
  servo.write(StartAngle);    //sets the starting angle for the servo
  Serial.begin(74880);         //initiates the serial monitor
  delay(5000);
  Serial.print("time,distance,angle");
}

void loop() {
  //Ultrasonic Sensor
  //Setup
  digitalWrite(PinTrig,LOW);  //Clear pin
  delay(2);
  //Output signal for 10 microseconds
  digitalWrite(PinTrig, HIGH);
  delay(10);
  digitalWrite(PinTrig,LOW);
  //Input from echo pin
  duration = pulseIn(PinEcho,HIGH);
  //Calculate Distance
  float dis = duration*0.034/2 ;

  sum = sum-readings[index];    //The sum minuses the oldest value from the readings array
  val = dis;               //The val integer is assigned the value of the current distance reading
  readings[index] = val;        //The current index of the readings array is updated
  sum = sum + val;              //The sum is updated to include the current reading
  index = (index+1)%numRead;    //The index is updated to the next value between 0 and the number of readings

  distance = sum/numRead;
  Serial.println(distance);    //12 is subtracted so x goes from -12 to 12 with 0 the middle
  if (distance > 35 or distance < 3) {     //This ensures that the sensor only reads distances -
    distance = -30;                         //within the range that the ball is able to go.
  }

  //Applying the PID Algorithm
  now = millis();              //time
  dt = (now - last_time);
  Perror = setpoint - distance;

  Derror = (Perror - last_error)/dt;
  Ierror = Ierror + (Perror*dt);
  if (Ierror>10){
    Ierror = 10;
  }
  if (Ierror<-10){
    Ierror = -10;
  }
  //Combining the terms of the ouput
  int output = Kp*Perror + Ki*Ierror + Kd*Derror;

  //Upper and Lower PID Bounds to Prevent Saturation and Overreaction of the system
  if (output>35) {
    output = 35;
  }
  if (output < -35){
    output = -35;
  }
  //Write the output to the servo
  servo.write(StartAngle+output);

  //Serial outputs to be converted to CSV
  Serial.print(now);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(",");
  Serial.print(StartAngle+output);
  //update variables
  last_error = Perror;
  last_time = now;
  delay(2);
}