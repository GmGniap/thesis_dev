#define FORWARD 0x0A
#define BACKWARD 0x05
#define STOP 0x00

int Motor[6] = {22,23,24,25,4,5};

void setup() {
  // put your setup code here, to run once:
int z;
for(z=0;z<6;z++)
{
  pinMode(Motor[z],OUTPUT);
  digitalWrite(Motor[z],LOW);
}
}

void loop() {
  // put your main code here, to run repeatedly:
Motor_Control('L');
Motor_mode(FORWARD);
delay(2000);
Motor_mode(STOP);
delay(200);
Motor_mode(BACKWARD);
delay(2000);
Motor_mode(STOP);
delay(200);

Motor_Control('R');
Motor_mode(FORWARD);
delay(2000);
Motor_mode(STOP);
delay(200);
Motor_mode(BACKWARD);
delay(2000);
Motor_mode(STOP);
delay(200);

}

void Motor_mode(int da)
{
  int z;
  for(z=0;z<4;z++)
    digitalWrite(Motor[z],(da>>z) & 0x01);
}

void Motor_Control(char da)
{
  switch(da)
  {
    case 'L':
      digitalWrite(Motor[4],HIGH);
      digitalWrite(Motor[5],LOW);
    break;

    case 'R':
      digitalWrite(Motor[4],LOW);
      digitalWrite(Motor[5],HIGH);
    break;
    
    default:
      digitalWrite(Motor[4],LOW);
      digitalWrite(Motor[5],LOW);
    break;
  }
}

