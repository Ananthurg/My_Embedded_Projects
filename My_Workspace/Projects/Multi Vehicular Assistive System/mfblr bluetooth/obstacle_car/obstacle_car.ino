// defining the pins
#define c1 4
#define c2 5
#define c3 6
#define c4 7

const int trigPin = 2;
const int echoPin = 3;
long duration;
int distance;
int flag=0;
int flag1=0;
void setup()
{
  Serial.begin(115200);
  pinMode( c1, OUTPUT);
  pinMode( c2, OUTPUT);
  pinMode( c3, OUTPUT);
  pinMode( c4, OUTPUT);
  
  digitalWrite( c1, LOW);
  digitalWrite( c2, LOW);
  digitalWrite( c3, LOW);
  digitalWrite( c4, LOW);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

}

void forward()
{
  digitalWrite( c1, LOW);
  digitalWrite( c2, HIGH);
  digitalWrite( c3,LOW);
  digitalWrite( c4, HIGH);
  Serial.println("forward");
}
void backward()
{
  digitalWrite( c1, HIGH);
  digitalWrite( c2, LOW);
  digitalWrite( c3,HIGH);
  digitalWrite( c4, LOW);
  Serial.println("backward");
}
void left()
{
  digitalWrite( c1, LOW);
  digitalWrite( c2, HIGH);
  digitalWrite( c3,LOW);
  digitalWrite( c4, LOW);
  Serial.println("left");
}
void right()
{
  digitalWrite( c1, LOW);
  digitalWrite( c2,LOW);
  digitalWrite( c3,LOW);
  digitalWrite( c4, HIGH);
  Serial.println("right");
}
void Stop()
{
  digitalWrite( c1, LOW);
  digitalWrite( c2, LOW);
  digitalWrite( c3,LOW);
  digitalWrite( c4, LOW);
  Serial.println("Stop");
}


void loop() 
{  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.println(distance);


  if (distance>20)
  {
    flag=0;
    Serial.print(flag);
    Serial.print("--clear---");
    Serial.println(distance);
    delay(1000);
  }
  else if (distance<20 && flag1== 1)
  {
    flag=1;
    Serial.print(flag);
    Serial.print("--obstacle detected--");
    Serial.println(distance);
    delay(1000);
    Stop();
    flag1=0;
//    delay(1000);
//    backward();
//    delay(3000);
//    left();
//    delay(2000);
//    Stop();
  }
  
  while (Serial.available()>0)
  {
    char data=Serial.read();
    Serial.print(data);
    if (data=='8' && flag==0)
    {
      forward();
    }

    if (data=='2' && flag==0)
    {
      backward();
      flag1=1;
    }
    
    if (data=='4'&& flag==0)
    {
      left();
    }

    if (data=='6'&& flag==0)
    {
      right();
    }
    
    if (data=='0'&& flag==0)
    {
      Stop();
    }
    
  }

}
