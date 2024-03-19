//  Motor control testing form the Ali Express 24GP-2430 motor
//  My Model number is DC24V169RPM - 24v 169 Max RPM
//  
// The speed controll for this line of motors is inverted. 
// A 0% duty cycle is full speed. 100% duty cycle stops the motor
// For the TimerOne Library, 1023 is 100% duty cycle - full stop


#include <TimerOne.h>

// Constants
const int CW=HIGH; // Used to set the direction of motor to clockwise, looking at the front of the motor
const int CCW=LOW; // Set direction to counter clockwise
const int maxSpeed=0;
const int stop=1023;

// Motor Drive settings
int speed=1023;  // Start with the motor stopped
bool direction = CW;
int pwmFreq = 40; // Cycle duration in uSec.  Take the inverse for Freq i.e. 1/40uS=25KHz. Range is 20KHz-30KHz

// Pin Definitions
const int MOT_ENC_PIN = 2;  
const int MOT_DIR_PIN = 8;
const int MOT_PWM_PIN = 9;
const int OUT_ENC_PIN = 3;
const int SPEED_PIN = A0;

// Motor Encoder Variables
long mCnt,mPrevCnt,mCurrentCnt, mDiff;
float mRPM, calcOutRPM;

// Ouptut Encoder Variables
long oCnt, oPrevCnt, oCurrentCnt, oDiff;
float oRPM;

//int speed;

void setup(void)
{
  // Initialize Pins as input
  pinMode(MOT_ENC_PIN, INPUT_PULLUP);  // Motor encoder output needs a pull-up
  pinMode(OUT_ENC_PIN, INPUT); 
  pinMode(MOT_DIR_PIN, OUTPUT);  // Motor direction control 
  pinMode(MOT_PWM_PIN, OUTPUT);  // PWM drive
  //pinMode(SPEED_PIN, INPUT);

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
  // Read speed pin, invert for 2430 motor
  speed=1023-analogRead(SPEED_PIN);
    Serial.print("Speed: ");
    Serial.println(speed);

  for (int i = stop; i > speed; i--)
  {
    Timer1.setPwmDuty(MOT_PWM_PIN, i);
    delay(1);
  }
  delay(3000);
  for (int i = speed; i < stop; i++)
  {
    Timer1.setPwmDuty(MOT_PWM_PIN, i);
    delay(1);
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
 