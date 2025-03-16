//Global Variables
const float PPR = 12;  // Define the encoder resolution in PPR
const float GEAR_RATIO = 45;  // Define the gear ratio of the encoder
volatile int pulse = 0;  // Initialize the pulse count variable as volatile to ensure it can be accessed by the interrupt service routine
volatile float ti = 0; //setting initial times 
volatile float rmp_i = 0;
volatile float pulse_i = 0;  

//User created functions
void incrementCount() {
int direction = digitalRead(3);  // Read the current direction of rotation from pin 3
  if (direction == HIGH){
  pulse ++; // Increment the count when pin 3 is high
  }
  else{
  pulse --; // decrement the count when pin 3 is high
  }   
}
//Pin definitions and initial states
void setup() {
  pinMode(2, INPUT_PULLUP);  // Set pin 2 as an input with a pullup resistor
  attachInterrupt(digitalPinToInterrupt(2), incrementCount, RISING);  // Attach the interrupt to pin 2 and set it to trigger on a rising edge
  Serial.begin(9600);  // Start the serial communication

}
//iterations
void loop() {
  for(int pwm = 0; pwm <= 250; pwm++){ // incrementing the speed by 1 
    analogWrite(9, pwm); // the LOW, HIGH combo makes the device turn cw (positive direction)
    digitalWrite(10, LOW); 
    digitalWrite(11, HIGH); 

    noInterrupts();
    volatile float pulse_f = pulse; // total pulses is the final pulse
    volatile float t = micros(); // getting final time in micro seconds
    float rev = (pulse_f - pulse_i)/ (PPR* GEAR_RATIO);  // Convert PPR to revolutions where initial pulse was 0. 
    float rpm = rev/(t - ti)*(1.0e6)*60; // convertings to RPMs (rev/us)(us/s)(s/min)
    pulse_i = pulse_f; // updating pulses so that the old final is now the new initial
    ti = t;  // updating times so that the new initial time is the old time. 
    interrupts(); 
    Serial.println(rpm);  // Print the current position to the serial monitor
    delay(100);  // Delay for 100ms to avoid overwhelming the serial monitor with output
  }
}


