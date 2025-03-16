//Global Variables
const float PPR = 12;  // Define the encoder resolution in PPR
const float GEAR_RATIO = 45;  // Define the gear ratio of the encoder
volatile int pulse = 0;  // Initialize the pulse count variable as volatile to ensure it can be accessed by the interrupt service routine

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

//interations
void loop() {
  float revolutions = pulse / (PPR* GEAR_RATIO);  // Convert PPR to revolutions
  float radians = revolutions*2*PI; // Convert revolutions to radians
  Serial.println(radians);  // Print the current position to the serial monitor
  delay(100);  // Delay for 100ms to avoid overwhelming the serial monitor with output
}


