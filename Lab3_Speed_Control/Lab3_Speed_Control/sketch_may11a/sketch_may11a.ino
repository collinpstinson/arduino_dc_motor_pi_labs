/*/////////////////////////////////////////////////////////////////////////////////////
GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////////////*/

// motor specifications 
const float gr = 45; // gear ratio
const float ppr = 12; // pulses per revolution

// motor pins
const int pwm = 9; // voltage pin that determines speed 
const int in1 = 10; // these last two determine direction
const int in2 = 11;

// encoder pins
const int enca = 2; // as the motor spins, the encoder signals 
const int encb = 3; // high and low values to help determine position/direction

// initial states
float ti = 0; // initial time
volatile int ctf, cti = 0;  // initial pulse counts are zero 

// PID values
float kp = 1; 
float ki = 0; 


/*////////////////////////////////////////////////////////////////////////////////////
USER CREATED FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////*/

// // count pulses 
void pulsecount(){
  int b = digitalRead(encb); // reading from encb called from ISR
  if(b == HIGH){ 
    ctf ++; // clockwise direction
  }
  else{
    ctf --; // counterclockwise direction
  }
}

/*////////////////////////////////////////////////////////////////////////////////////
Setup function runs only once and cannot be read from the loop() function
////////////////////////////////////////////////////////////////////////////////////*/
void setup() {
  Serial.begin(9600); // communication between the computer and board

  pinMode(enca, INPUT_PULLUP); // the pins have to be defined
  pinMode(encb, INPUT_PULLUP);
  pinMode(pwm, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(enca),pulsecount,RISING); // the ISR used to count pulses
}
/*////////////////////////////////////////////////////////////////////////////////////
loop() is how our program gets repeatedly reiterated.
////////////////////////////////////////////////////////////////////////////////////*/
void loop() {
 
  // Position and speed
  noInterrupts(); // this ensure that the ISR does not affect our position calculation
  volatile int ct = ctf - cti; // calculate the change in position in terms of pulse counts per revolution
  volatile float rev = ct/(ppr*gr); // rpms calculated in relation to counts, PPR, gear ratio, and time
  interrupts(); 

// time variables 
  long tf = micros(); // getting time and converting from microseconds to minutes
  long dt = (tf - ti)/(60*1.0e6); // time change
 

// PID control
 float rpmt = 50.0; //setting velocity to 50 rpms
 float rpm = rev/dt;
 float error = rpmt - rpm; 
 float u = error*(kp + ki*dt); 

 // PID sets the pmw
 int pwm_v = constrain(u, 0, 255); 

// Initiate the motor
analogWrite(pwm, pwm_v); 

if (error > 0){
  digitalWrite(in1, LOW); // if the speed is under the target, give it a positive direction
  digitalWrite(in2, HIGH); 
}
else if(error < 0){
  digitalWrite(in1, HIGH); // if the speed overshoots target, give it a negative direction
  digitalWrite(in2, LOW);  
}
else{
  digitalWrite(in1, LOW); // if there is no error, do nothing
  digitalWrite(in2, LOW);  
}

// graph the set rpm and output 
Serial.println(rpmt); // graph the 
Serial.print(" ");
Serial.print(rpm);
Serial.println();

// update time and position for next iteration
ti = tf;  
cti = ctf;
delay(100); 
}
