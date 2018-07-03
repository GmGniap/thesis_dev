#include<MsTimer2.h>

int front = 10;
int back  = 9 ;
int led_state = 0 ;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(front,OUTPUT);
  pinMode(back,OUTPUT);
  MsTimer2::set(500000,Timer2_ISR);
  MsTimer2::start();
}
void loop() {
  // put your main code here, to run repeatedly:

}

void Timer2_ISR()
{
  Serial.println("Function");
  digitalWrite(front,led_state);
  if(led_state)
    led_state=0;
  else
    led_state=1;
}

