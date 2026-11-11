int sensorpin1=2;
int sensorpin2=3;
int sensorpin3=4;
int sensorpin4=5;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT); 
}


void forward()
{
      digitalWrite(2,HIGH);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
}
void backward()
{
  digitalWrite(2,LOW);
 digitalWrite(3,HIGH);
 digitalWrite(4,HIGH);
 digitalWrite(5,LOW);
}
void right()
{
   digitalWrite(2,LOW);
 digitalWrite(3,LOW);
 digitalWrite(4,LOW);
 digitalWrite(5,HIGH);
  }
void left()
{
  digitalWrite(2,HIGH);
 digitalWrite(3,LOW);
 digitalWrite(4,LOW);
 digitalWrite(5,LOW);
  }
  void Stop()
  {
 digitalWrite(2,LOW);
 digitalWrite(3,LOW);
 digitalWrite(4,LOW);
 digitalWrite(5,LOW);
    }

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>0)
  {
    char a=Serial.read();
      //   Serial.println(a);
    if(a=='8')
    {
      forward();
      Serial.println("forward");
      delay(3000);
    }
    if(a=='2')
 {
  backward();
  Serial.println("backward");
 delay(3000);
 }
 if(a=='6')
 {
 right();
  Serial.println("right");
 delay(3000);
 }
 if(a=='4')
 {
  left();
  Serial.println("left");
 delay(3000);
 }
 if(a=='0')
 {
  Stop();
  Serial.println("stop");
 delay(3000);
 
 }
  } 
  }
  
