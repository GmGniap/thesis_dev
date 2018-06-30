#define InA1            5                      // INA motor pin
#define InB1            6                      // INB motor pin
#define PWM1            7                       // PWM motor pin

#define encodPinA1      2                       // encoder A pin
#define encodPinB1      3                       // encoder B pin

//#define Vpin            0                       // battery monitoring analog pin
//#define Apin            1                       // motor current monitoring analog pin

#define CURRENT_LIMIT   1000                     // high current warning
#define LOW_BAT         10000                   // low bat warning
#define LOOPTIME        100                     // PID loop time
#define NUMREADINGS     10                      // samples for Amp average

int readings[NUMREADINGS];
unsigned long lastMilli = 0;                    // loop timing
unsigned long lastMilliPrint = 0;               // loop timing
int speed_req = 300;                            // speed (Set Point)
int speed_act = 0;                              // speed (actual value)
int PWM_val = 0;                                // (25% = 64; 50% = 127; 75% = 191; 100% = 255)
int voltage = 0;                                // in mV
int current = 0;                                // in mA
volatile long count = 0;                        // rev counter
float Kp =   .4;                                // PID proportional control Gain
float Kd =    1;                                // PID Derivitave control gain


void setup() {
 analogReference(EXTERNAL);                            // Current external ref is 3.3V
 Serial.begin(9600);
 pinMode(InA1, OUTPUT);
 pinMode(InB1, OUTPUT);
 pinMode(PWM1, OUTPUT);
 
 pinMode(encodPinA1, INPUT);
 pinMode(encodPinB1, INPUT);
 digitalWrite(encodPinA1, HIGH);                      // turn on pullup resistor
 digitalWrite(encodPinB1, HIGH);
 attachInterrupt(1, rencoder, FALLING);
 for(int i=0; i<NUMREADINGS; i++)   readings[i] = 0;  // initialize readings to 0

 analogWrite(PWM1, PWM_val);
 digitalWrite(InA1, LOW);
 digitalWrite(InB1, HIGH);
}

void loop() {
 getParam();                                                                 // check keyboard
 if((millis()-lastMilli) >= LOOPTIME)   {                                    // enter tmed loop
   lastMilli = millis();
   getMotorData();                                                           // calculate speed, volts and Amps
   PWM_val= updatePid(PWM_val, speed_req, speed_act);                        // compute PWM value
   analogWrite(PWM1, PWM_val);                                               // send PWM to motor
 }
 printMotorInfo();                                                           // display data
}

void getMotorData()  {                                                        // calculate speed, volts and Amps
static long countAnt = 0;                                                   // last count
 speed_act = ((count - countAnt)*(60*(1000/LOOPTIME)))/(64*19);          // 64 pulses X 19 gear ratio = 1216 counts per output shaft rev
 countAnt = count;                  
}

int updatePid(int command, int targetValue, int currentValue)   {             // compute PWM value
float pidTerm = 0;                                                            // PID correction
int error=0;                                  
static int last_error=0;                            
 error = abs(targetValue) - abs(currentValue);
 pidTerm = (Kp * error) + (Kd * (error - last_error));                            
 last_error = error;
 return constrain(command + int(pidTerm), 0, 255);
}

void printMotorInfo()  {                                                      // display data
 if((millis()-lastMilliPrint) >= 500)   {                    
   lastMilliPrint = millis();
   Serial.print("SP:");             Serial.print(speed_req);  
   Serial.print("  RPM:");          Serial.print(speed_act);
   Serial.print("  PWM:");          Serial.print(PWM_val); 
   Serial.println(" "); 
   //Serial.print("  V:");            Serial.print(float(voltage)/1000,1);
   //Serial.print("  mA:");           Serial.println(current);

   if (current > CURRENT_LIMIT)               Serial.println("*** CURRENT_LIMIT ***");                
   if (voltage > 1000 && voltage < LOW_BAT)   Serial.println("*** LOW_BAT ***");                
 }
}

/*void rencoder()  {                                    // pulse and direction, direct port reading to save cycles
 if (PINB & 0b00000001)    count++;                // if(digitalRead(encodPinB1)==HIGH)   count ++;
 else                      count--;                // if (digitalRead(encodPinB1)==LOW)   count --;
} */
void rencoder()
{
  if (digitalRead(encodPinB1) == digitalRead(encodPinA1))
  {
    count = count - 1;
  }
  else
  {
    count = count + 1;
  }
}

int getParam()  {
char param, cmd;
 if(!Serial.available())    return 0;
 delay(10);                  
 param = Serial.read();                              // get parameter byte
 if(!Serial.available())    return 0;
 cmd = Serial.read();                                // get command byte
 Serial.flush();
 switch (param) {
   case 'v':                                         // adjust speed
     if(cmd=='+')  {
       speed_req += 20;
       if(speed_req>400)   speed_req=400;
     }
     if(cmd=='-')    {
       speed_req -= 20;
       if(speed_req<0)   speed_req=0;
     }
     break;
   case 's':                                        // adjust direction
     if(cmd=='+'){
       digitalWrite(InA1, LOW);
       digitalWrite(InB1, HIGH);
     }
     if(cmd=='-')   {
       digitalWrite(InA1, HIGH);
       digitalWrite(InB1, LOW);
     }
     break;
   case 'o':                                        // user should type "oo"
     digitalWrite(InA1, LOW);
     digitalWrite(InB1, LOW);
     speed_req = 0;
     break;
   default:
     Serial.println("???");
   }
}
