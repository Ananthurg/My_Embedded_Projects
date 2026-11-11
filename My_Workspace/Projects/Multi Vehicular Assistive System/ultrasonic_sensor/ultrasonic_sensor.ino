
const int pingPin = 2; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 4; // Echo Pin of Ultrasonic Sensor
 int buzzer = 7;
void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   pinMode(7,OUTPUT);
}

void loop() {
   long duration, inches,microsecondsToInches(duration), cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   //inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
  // Serial.print(inches);
  // Serial.print("in, ");
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   delay(100);
   if (cm<=100)
   {
    digitalWrite(7,HIGH);
    delay(2000);
    digitalWrite(7, LOW);
   }

}
 
long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds){
   return microseconds / 29/ 2;
}
