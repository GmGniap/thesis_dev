int front=10;
int back = 9;
int state=0;

void setup() {
  // put your setup code here, to run once:
pinMode(front,OUTPUT);
pinMode(back,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(back,state);
if(state)
  state=0;
else
  state=1;
  delay(1000);
}
