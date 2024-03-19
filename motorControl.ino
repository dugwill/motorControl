//  Motor control testing form the Ali Express 24GP-2430 motor
//  My Model number is DC24V169RPM-24v 169 Max RPM
// 
// The speed controll is inverted. A 0% duty cycle is full speed
// 100% duty Cycle stops the motor
// 
// 

#include <TimerOne.h>

// Constants
const int CW=HIGH;
const int CCW=LOW;
const int maxSpeed=930;
const int stop=1023;

// Motor Drive settings
int speed=1023;
bool direction = CW;
int pwmFreq = 40; // In uS.  Take the inverse for Freq i.e. 1/40uS=25KHz range is 20KHz-30KHz

// Pin Definitions
const int MOT_ENC_PIN = 2;  
const int MOT_DIR_PIN = 8;
const int MOT_PWM_PIN = 9;
const int OUT_ENC_PIN = 3;

// Motor Encoder Variables
long mCnt,mPrevCnt,mCurrentCnt, mDiff;
float mRPM, calcOutRPM;

// Ouptut Encoder Variables
long oCnt, oPrevCnt, oCurrentCnt, oDiff;
float oRPM;

void setup(void)
{
  // Initialize Pins as input
  pinMode(MOT_ENC_PIN, INPUT_PULLUP);
  pinMode(OUT_ENC_PIN, INPUT); 
  pinMode(MOT_DIR_PIN, OUTPUT);  //direction control 
  pinMode(MOT_PWM_PIN, OUTPUT);  // PWM drive

  // Calls to enable Timer1 for PWM
  Timer1.initialize(pwmFreq);
  Timer1.pwm(MOT_PWM_PIN, 1023);  
  
  // Set Initial Direction
  digitalWrite(MOT_DIR_PIN, direction);

  // Attach interrupt to motor encoder
  attachInterrupt(digitalPinToInterrupt(MOT_ENC_PIN), encoderInterrupt, RISING);

  // Attach interrupt to output encoder
  attachInterrupt(digitalPinToInterrupt(OUT_ENC_PIN), outputInterrupt, RISING);

  // Initialize Serial communication
  Serial.begin(115200);

}
 
void loop(void)
{
  for (int i = stop; i > maxSpeed; i--)
  {
    Timer1.setPwmDuty(MOT_PWM_PIN, i);
    delay(10);
  }
  delay(3000);
  for (int i = maxSpeed; i < stop; i++)
  {
    Timer1.setPwmDuty(MOT_PWM_PIN, i);
    delay(10);
  }
  direction=!direction;
  digitalWrite(MOT_DIR_PIN, direction);

}


// Interrupt service routine for KY-10 pin
void encoderInterrupt() { 
    mCnt++;
}

// Interrupt service routine for OUTPUT pin
void outputInterrupt() { 
    oCnt++;
}
 