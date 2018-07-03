//This Program is about measuring rotating displacement of wheel with encoder , from Manual book - P193 - section(6-5)

#define Encoder_count_H_
#define FORWARD 0x09
#define BACKWARD 0x06

#include <FlexiTimer2.h>

void flash() {
  static boolean output = HIGH;

  digitalWrite(13,output);
  output = !output;
}

int Motor[6] = {22,23,24,25,4,5};

unsigned int Encoder_L=0,Encoder_R=0;
unsigned int Encoder_value_L=0,Encoder_value_R=0,Timer_flag=0;

void setup() {
  // put your setup code here, to run once:
int z;
for(z=0;z<6;z++)
 {
  pinMode(Motor[z],OUTPUT);
  digitalWrite(Motor[z],LOW);
 }

Serial.begin(115200);
Serial.print("Hello");

attachInterrupt(6,Encoder_count_L,FALLING);
attachInterrupt(7,Encoder_count_R,FALLING);

FlexiTimer2::set(500,Timer_ISR);
FlexiTimer2::start();
Motor_mode(FORWARD);
Motor_Control('A');
}

void loop() {
  // put your main code here, to run repeatedly:
if(Timer_flag)
{
  Encoder_value_L=Encoder_L;
  Encoder_value_R=Encoder_R;

  Encoder_L = 0;
  Encoder_R = 0;

  Serial.print("Counter L/R: ");
  Serial.print(Encoder_value_L);
  Serial.print(" / ");
  Serial.println(Encoder_value_R);
  Timer_flag = 0;
}
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
    case 'A':
      digitalWrite(Motor[4],HIGH);
      digitalWrite(Motor[5],HIGH);
      break;

    default:
      digitalWrite(Motor[4],LOW);
      digitalWrite(Motor[5],LOW);
      break;
  }
}

void Encoder_count_L()
{
  Encoder_L++;
}

void Encoder_count_R()
{
  Encoder_R++;
}

void Timer_ISR()
{
  Timer_flag = 1;
}

