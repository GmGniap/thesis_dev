#include<FlexiTimer2.h>

#define FORWARD 0x09
#define BACKWARD  0x06
#define LEFT_U  0x0A
#define RIGHT_U 0x05
#define LEFT    0x08
#define RIGHT   0x01
#define STOP    0x00

#define RPM     100
#define RPM_value ((unsigned int)(RPM * 1.3))

#define KP 0.25
#define KI 0.00025
#define KD 0.125

int Motor[6] = {22,23,24,25,4,5};

signed int Right_cnt=0,Left_cnt=0;
signed int pwm_value_R=0,pwm_value_L=0;
unsigned int ENCODER_CNT_L=0,ENCODER_CNT_R=0,Timer_flag=0;

void setup() {
  // put your setup code here, to run once:
  int z;
  Serial.begin(115200);
  for(z=0;z<6;z++)
  {
    pinMode(Motor[z],OUTPUT);
    digitalWrite(Motor[z],LOW);
  }
  attachInterrupt(6,Encoder_count_L,RISING);
  attachInterrupt(7,Encoder_count_R,RISING);
  FlexiTimer2::set(500,Timer2_ISR);
  FlexiTimer2::start();
  Motor_mode(FORWARD);
}

void loop() {
  // put your main code here, to run repeatedly:
  static signed int R_pre_err = 0;
  static signed int R_cur_err = 0;
  static signed int R_cur_cnt = 0;
  static signed int R_dt_err  = 0;
  static signed int R_err_sum = 0;

  static signed int L_pre_err = 0;
  static signed int L_cur_err = 0;
  static signed int L_cur_cnt = 0;
  static signed int L_dt_err  = 0;
  static signed int L_err_sum = 0;

  if(Timer_flag)
    {
      R_pre_err = R_cur_err;
      R_cur_cnt = Right_cnt;
      R_cur_err = RPM_value - R_cur_cnt;
      R_err_sum+= R_cur_err;
      R_dt_err  = R_cur_err - R_pre_err;

      pwm_value_R += (R_cur_err * KP) + (R_err_sum * KI) + (R_dt_err * KD);

      if(pwm_value_R > 255)
        pwm_value_R = 255;
      else if(pwm_value_R < 0)
        pwm_value_R = 0;

//L
      L_pre_err = L_cur_err;
      L_cur_cnt = Left_cnt;
      L_cur_err = RPM_value - L_cur_cnt;
      L_err_sum+= L_cur_err;
      L_dt_err  = L_cur_err - L_pre_err;

      pwm_value_L += (L_cur_err * KP) + (L_err_sum * KI) + (L_dt_err * KD);

      if(pwm_value_L > 255)
        pwm_value_L = 255;
      else if(pwm_value_L < 0)
        pwm_value_L = 0;

      Motor_Control('R',pwm_value_R);
      Motor_Control('L',pwm_value_L);

      Serial.print("PWM R/L : ");
      Serial.print(pwm_value_R);
      Serial.print(" / ");
      Serial.print(pwm_value_L);
      Serial.print("  Count R/L : ");
      Serial.print(Right_cnt);
      Serial.print(" / ");
      Serial.print(Left_cnt);
      Serial.print(" RPM_value: ");
      Serial.println(RPM_value);
      Timer_flag = 0 ;
    }      
}

void Motor_mode(int da)
{
  int z;
  for(z=0;z<4;z++)
  {
    digitalWrite(Motor[z],(da>>z)&0x01);
  }
}

void Motor_Control(char da,unsigned int OC_value)
{
  switch(da)
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

void Encoder_count_L()
{
  ENCODER_CNT_L++;
}

void Encoder_count_R()
{
  ENCODER_CNT_R++;
}

void Timer2_ISR()
{
  Right_cnt = ENCODER_CNT_R;
  Left_cnt  = ENCODER_CNT_L;
  ENCODER_CNT_R = 0;
  ENCODER_CNT_L = 0;
  Timer_flag = 1;
}

