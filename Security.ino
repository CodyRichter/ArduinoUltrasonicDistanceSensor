  /*
  * Arduino Security System
  * Created by: Cody R.
  * 
  * 
  * Implements an ultrasonic distance sensor, 3 LEDs, and a switch to track changes to the physical world after configured.
  */
 
 //
 // Basic Software Setup
 //
 double softwareVersion = 1.0;
 
 //
 // Port Setup
 //
 int INPUT_PIN_SENSOR = 13; //This is the pin # of the input pin that contains the input ping sensor
 int INPUT_PIN_MODE_TOGGLE = 12; //Input for control mode toggle
 int OUTPUT_PIN_LED_RED = 4; //Output pin for red LED
 int OUTPUT_PIN_POWER_SWITCH = 10; //Output pin for red LED
 int OUTPUT_PIN_LED_BLUE = 5; //Output pin for blue LED
 int OUTPUT_PIN_LED_GREEN = 6; //Output pin for green LED
 
 //
 //Software Control Variables
 //
 int defaultRange = -10;
 int proximityRange = defaultRange; //How close a person must be to the sensor (in inches) for the security to activate
 int delta = 3; //Tolerance allowed in sensor to account for minor variations (in inches)
 boolean scanningMode = true;
 boolean targetDetected = false;
 boolean setupMode = false;
 boolean setupComplete = false;
 int sampleTotal = 0, sampleWithinRange = 0;
   long duration, inches;
 
 void setup() {
   //Input Pins
   pinMode (INPUT_PIN_SENSOR, INPUT);
   pinMode (INPUT_PIN_MODE_TOGGLE, INPUT);
   //Output LED Pins
   pinMode (OUTPUT_PIN_LED_RED, OUTPUT);
   pinMode (OUTPUT_PIN_LED_GREEN, OUTPUT);
   pinMode (OUTPUT_PIN_LED_BLUE, OUTPUT);
   //Output Voltage Pins
   pinMode (OUTPUT_PIN_POWER_SWITCH, OUTPUT);
     
   //5v Output Pins For Simplicity in Soldering
   digitalWrite(OUTPUT_PIN_POWER_SWITCH, HIGH);
 
   Serial.begin(9600);
 }
 
 void loop() {
   //Motion Detection
 
   if (proximityRange == defaultRange)
   {
     digitalWrite(OUTPUT_PIN_LED_RED, HIGH);
     digitalWrite(OUTPUT_PIN_LED_GREEN, HIGH);
     digitalWrite(OUTPUT_PIN_LED_BLUE, HIGH);
   }
 
   if (targetDetected && scanningMode && proximityRange != defaultRange) //Turns On Red Light If Target Detected
   {
     digitalWrite(OUTPUT_PIN_LED_RED, HIGH);
     digitalWrite(OUTPUT_PIN_LED_GREEN, LOW);
     digitalWrite(OUTPUT_PIN_LED_BLUE, LOW);
   }
   else if (scanningMode && !targetDetected && proximityRange != defaultRange)
   {
     digitalWrite(OUTPUT_PIN_LED_GREEN, HIGH);
     digitalWrite(OUTPUT_PIN_LED_RED, LOW);
     digitalWrite(OUTPUT_PIN_LED_BLUE, LOW);
   }
 
 
   if (digitalRead(INPUT_PIN_MODE_TOGGLE) == HIGH)
   {
     setupMode = true;
     scanningMode = false;
   }
   else
   {
     setupMode = false;
     scanningMode = true;
     setupComplete = false;
   }
 
   if (scanningMode && !targetDetected && proximityRange != defaultRange)
   {
     //Distance Sensor Scans Area Ahead Of It
     pinMode(INPUT_PIN_SENSOR, OUTPUT);
     digitalWrite(INPUT_PIN_SENSOR, LOW);
     delayMicroseconds(2);
     digitalWrite(INPUT_PIN_SENSOR, HIGH);
     delayMicroseconds(5);
     digitalWrite(INPUT_PIN_SENSOR, LOW);
 
     //Calculate Distance To Target
     pinMode(INPUT_PIN_SENSOR, INPUT);
     duration = pulseIn(INPUT_PIN_SENSOR, HIGH);
     inches = microsecondsToInches(duration);
 
     delayMicroseconds(10); //MANDATORY Delay to allow time between cycles
 
       Serial.println("Distance From Ideal: ");
       Serial.println(abs(proximityRange-inches));
     if (abs(proximityRange-inches) > delta)
     {
       Serial.println("Outlier Found: Distance From Ideal: ");
       Serial.println(abs(proximityRange-inches));
       sampleWithinRange++;
       sampleTotal++;
       
     }
     else
     {
       sampleTotal++;
     }
     
     if (sampleTotal >= 10)
     {
       Serial.println("30 Samples. Checking:");
       if (sampleWithinRange*sampleTotal >= 50)
       {
         Serial.println("TARGET DETECTED");
         targetDetected = true;
       }
       else
       {
         Serial.println("No Target");
       }
       sampleTotal = 0;
       sampleWithinRange = 0;
     }
 
   }
   //Mode 3: Setup
   if (setupMode && !setupComplete)
   {
     Serial.println("In Setup Mode");
     int numberSamples = 30;
     int distance[numberSamples];
     int minimum = 1000000, maximum = 0;
     digitalWrite(OUTPUT_PIN_LED_RED, LOW);
     digitalWrite(OUTPUT_PIN_LED_GREEN, LOW);
     digitalWrite(OUTPUT_PIN_LED_BLUE, HIGH);
 
     for (int k = 0; k < numberSamples; k++)
     {
       Serial.println("Starting Iteration:");
       Serial.print(k);
       //Distance Sensor Scans Area Ahead Of It
       pinMode(INPUT_PIN_SENSOR, OUTPUT);
       digitalWrite(INPUT_PIN_SENSOR, LOW);
       delayMicroseconds(2);
       digitalWrite(INPUT_PIN_SENSOR, HIGH);
       delayMicroseconds(5);
       digitalWrite(INPUT_PIN_SENSOR, LOW);
 
       //Calculate Distance To Target
       pinMode(INPUT_PIN_SENSOR, INPUT);
       duration = pulseIn(INPUT_PIN_SENSOR, HIGH);
       inches = microsecondsToInches(duration);
       distance[k] = inches;
       if (inches > maximum) maximum = inches;
       if (inches < minimum) minimum = inches;
       delayMicroseconds(15); //MANDATORY Delay to allow time between cycles
     }
     if ((maximum - minimum) > delta)
     {
       digitalWrite(OUTPUT_PIN_LED_RED, HIGH);
       Serial.println("Setup Failed!");
       setupComplete = true;
     }
     else
     {
       digitalWrite(OUTPUT_PIN_LED_GREEN, HIGH);
       digitalWrite(OUTPUT_PIN_LED_BLUE, HIGH);
       proximityRange = (maximum + minimum) / 2;
       targetDetected = false;
       Serial.println("Setup Successful! Distance to Target: ");
       Serial.print(proximityRange);
       setupComplete = true;
     }
   }
 }
 
 long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
 }
