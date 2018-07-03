//This program is about controlling LED/Buzzer by UART , included in Manual Book - Page 155 - Section(5-3)
#include<Arduino.h>
#include<MsTimer2.h>

#define FORWARD 0x09
#define BACK  0x06
#define STOP  0x00
#define IDLE  0x00
#define BLINK_OFF 0
#define BLINK_ON  1

int front = 10;
int back  = 9 ;
int buzzer= 45;
int led_state = 0 ;

unsigned char led_value=0,led_mode=0,blink_state=0,buzzer_state=0,led_status=STOP;
unsigned char Command = 0;

void setup() {
  // put your setup code here, to run once:
  //PORT_init();
  pinMode(front,OUTPUT);
  pinMode(back,OUTPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(front,LOW);
  digitalWrite(back,LOW);
  digitalWrite(buzzer,LOW);
  
  Serial.begin(115200);
  Serial.println("Smart Car >>");
  MsTimer2::set(500000,flash);
  MsTimer2::start();
  Serial.println("Timer start..");
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial.available()>0)
  {
    //Serial.println("hello");
    Command = Serial.read();
    Serial.write(Command);  //to check whether the received data is right or not
    switch(Command)
      {
        case 'y':
        led_status = FORWARD;
        //Serial.println("Y-input");
        break;

        case 's':
        led_status = STOP;
        break;

        case 'b':
        led_status = BACK;
        break;

        default:
        led_status = IDLE;
        break;
      }
  }
}

void flash(){
  digitalWrite(front ,led_state);
  if(led_state)
    led_state = 0;
  else
    led_state = 1;
}

void Timer2_ISR()
{
  Serial.println("Timer2 Function");
  switch(led_status)
  {
  case 'FORWARD':
    led_value = 1;
    blink_state = BLINK_OFF;
    buzzer    = 1;
    Serial.println("Forward");
    break;

  case BACK:
    led_value = 2;
    blink_state = BLINK_ON;
    buzzer    = 1;
    break;
    
  case IDLE:
  
  default:
    led_value = 0;
    blink_state = BLINK_OFF;
    buzzer    = 0;
    Serial.println("Default");
    break;
  }
  LED_control(led_value);
  if(led_mode == 0)
    {
      if(blink_state == BLINK_ON)
      {
        LED_control(0);
      }
      led_mode == 1;
    }
    else
      led_mode = 0;

    digitalWrite(buzzer,buzzer_state);
}

void LED_control(int da)
{
  digitalWrite(front,(da & 0x01));
  digitalWrite(back ,((da>>1) & 0x01));
}

/*
void PORT_init()
{
  Serial.println("Initializing...");
  pinMode(front , OUTPUT);
  pinMode(back  , OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(front , LOW);
  digitalWrite(back  , LOW);
  digitalWrite(buzzer,LOW);
}*/
