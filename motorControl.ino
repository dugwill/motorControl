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
const int PID_UPDATE_TIME=100;  // mSec

// Motor Drive settings
int speed=1023;  // Start with the motor stopped
int speedCmd;
bool direction = CW;
int pwmFreq = 40; // Cycle duration in uSec.  Take the inverse for Freq i.e. 1/40uS=25KHz. Range is 20KHz-30KHz

// Pin Definitions
const int MOT_ENC_PIN = 2;  
const int MOT_DIR_PIN = 8;
const int MOT_PWM_PIN = 9;
const int OUT_ENC_PIN = 3;
const int SPEED_PIN = A0;

// Motor Encoder Variables
long mCnt, mPrevCnt,mCurrentCnt, mDiff;
float mRPM, calcOutRPM; 

// Ouptut Encoder Variables
long oCnt, oPrevCnt, oCurrentCnt, oDiff;
float oRPM; 

// Misc variables
long printTime, printTimer=1000;
long pidTime, pidTimer=PID_UPDATE_TIME;

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
  Timer1.pwm(MOT_PWM_PIN, 0);  
  
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
    speedCmd=1023-analogRead(SPEED_PIN);
    
    if (millis()-printTime>printTimer){
        print();
        printTime=millis();
    }

    if (millis()-pidTime>pidTimer){
        updatePid();
        pidTime=millis();
    }

    if (speed>speedCmd){
        decel();
        Timer1.setPwmDuty(MOT_PWM_PIN, 1023-speed);
    }
    if (speed<speedCmd){
        accel();
    }
}

void updatePid(){

    // Motor Encoder Variables
    long mPrevCnt,mCurrentCnt, mDiff;
    float mRPM, calcOutRPM;

    
}

void accel(){
if(speed<1023){
    speed++;
  }
  Timer1.setPwmDuty(MOT_PWM_PIN, 1023-speed);
}

void decel(){
if(speed>0){
    speed--;
  }
  Timer1.setPwmDuty(MOT_PWM_PIN, 1023-speed);
}


// Interrupt service routine for KY-10 pin
void encoderInterrupt() { 
    mCnt++;
}

// Interrupt service routine for OUTPUT pin
void outputInterrupt() { 
    oCnt++;
}
 
void print(){
    
    // Read the counters
    mCurrentCnt=mCnt;
    oCurrentCnt=oCnt;

    // Calculate the motor data
    mDiff=mCurrentCnt-mPrevCnt;  // Calculate the number of encoder pulses
    mRPM=mDiff*60/9; // Convert to pl/min and correct for 6 pl/rev
    calcOutRPM=mRPM/35.5;  // Calculate output RPM
    mPrevCnt=mCurrentCnt;  // Update previous count

    // Calculat the output encoder data
    oDiff=oCurrentCnt-oPrevCnt; // Calculate the number of encoder pulses
    oRPM=oDiff*60/12; // Correct for 12 pl/rev
    oPrevCnt=oCurrentCnt; // Update previous count

    Serial.print("Output: diff: ");
    Serial.print(oDiff);
    Serial.print(" out RPM: ");
    Serial.print(oRPM);

    Serial.print("     Motor: diff: ");
    Serial.print(mDiff);
    Serial.print(" motor RPM: ");
    Serial.print(mRPM);
    Serial.print(" Calc Output RPM: ");
    Serial.println(calcOutRPM);
}