void setup() 
{
  Serial.begin(9600);
  Serial.println("AT+CMGF=0");
  delay(1000);
  Serial.println("ATD9567104487;");
  delay(50000);
  Serial.println("ATH");
  delay(1000);
  Serial.println("AT+CMGF=1");
  delay(1000);
    Serial.println("AT+CMGS=\"+919567104487\"\r");
    delay(1000);
      Serial.println("heloo");
      delay(1000);
      Serial.println(char(26));
      delay(1000);
      


}

void loop() {
  // put your main code here, to run repeatedly:

}
