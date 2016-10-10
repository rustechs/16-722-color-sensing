/*
 * color-sensor.ino
 * Samples four amplified photodiodes (IR, R, Y, G) using
 * ADCs, averages samples, and transmits data over USB. Also
 * controls light source -- white LED or RGB.
 * 
 * Alexander Volkov Jr.
 * 
 * 16-722
 * Assignment 3: Color Sensing
 */

/*
 * Sensor Pin Mappings:
 * Yellow --> A1
 * IR --> A0
 * Green --> A2
 * Red --> A3
 * 
 * Illuminator Pin Mappings: 
 * White --> 6 (PWM)
 * 
 * Red (RGB) --> 9 (PWM)
 * Green (RGB) --> 10 (PWM)
 * Blue (RGB) --> 11 (PWM)
 */

// Number of measurements to average
const int N = 5;

// Photodiode ADC pin mappings
const int IR = A0;
const int Red = A3;
const int Yellow = A1;
const int Green = A2;

// Sensor measurement storage variables
int IRVal = 0;
int RedVal = 0;
int YellowVal = 0;
int GreenVal = 0;

// Illumination source pin and brightness
const int ledPin = 6;
const byte brightness = 150;

// Serial command char
const char start = 'm';

void setup() {
  //Initialize serial
  Serial.begin(9600);

  //Setup led pin as output and initialize to off
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 255);
}

void loop() {
  // Wait for start command over serial from PC-side program
  while (Serial.available() > 0) {
    
    // Check if incoming char is expected start command
    if (Serial.read() == start) {

      //Turn on illumination source
      analogWrite(ledPin, 255-brightness);

      //Wait a touch
      delay(200);
      
      //Begin measurements, repeat N times
      for (int i=0; i<N; i++){

        IRVal += analogRead(IR);
        RedVal += analogRead(Red);
        YellowVal += analogRead(Yellow);
        GreenVal += analogRead(Green);
        
      }

      // Average over N measurements
      IRVal /= N;
      RedVal /= N;
      YellowVal /= N;
      GreenVal /= N;

      //Transmit data
      Serial.print(IRVal);
      Serial.print(",");
      Serial.print(RedVal);
      Serial.print(",");
      Serial.print(YellowVal);
      Serial.print(",");
      Serial.println(GreenVal);

      Serial.flush();

      //Turn off illumination source
      analogWrite(ledPin, 255);
    }
  }

}
