#define FORWARD 0x09
#define BACKWARD 0x06
#define calc_PWM(_per)((unsigned int)(_per*2.55))

int Motor[6]={22,23,24,25,4,5};

void setup() {
  // put your setup code here, to run once:
  int z;
  for(z=0;z<6;z++)
  {
    pinMode(Motor[z],OUTPUT);
    pinMode(Motor[z],LOW);
  }
  Motor_mode(FORWARD);
}

void loop() {
  // put your main code here, to run repeatedly:
  int per;
  for(per=0;per<100;per++)
    {
      Motor_Control('A',per);
      delay(50);
    }
   for(per=100;per>0;per--)
    {
      Motor_Control('A',per);
      delay(50);
    }
}

void Motor_mode(int da)
  {
    int z;
    for(z=0;z<4;z++)
      {
        digitalWrite(Motor[z],(da>>z) & 0x01);
      }
  }

void Motor_Control(char sel,unsigned int _percent)
{
  unsigned int OC_value=calc_PWM(_percent);
  switch(sel)
  {
    case 'L':
      analogWrite(Motor[4],OC_value);
      break;
    case 'R':
      analogWrite(Motor[5],OC_value);
      break;
    case 'A':
      analogWrite(Motor[4],OC_value);
      analogWrite(Motor[5],OC_value);
      break;
  }
}

