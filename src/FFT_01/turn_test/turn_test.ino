#include <arduinoFFT.h>
#define PIN_IN1 2
#define PIN_IN2 4
#define PIN_VREF 3   //pwm
#define PIN_VREF2 11 //pwm2
#define PIN_IN3 13
#define PIN_IN4 12
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
const uint16_t samples = 128;             //power of 2
const double samplingFrequency = 10000;   //Maximum is a  half of this status
double vReal[samples];
double vImag[samples];
unsigned int sampling_period_us;
unsigned long microseconds;
int val = 0;
int rup=0;
int mode=0;
int delayal=0;
double rc = 0;
boolean move = false;
void setup()
{
  Serial.begin(9600);
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  pinMode(PIN_IN1,OUTPUT);
  pinMode(PIN_IN2,OUTPUT);
  pinMode(PIN_IN3,OUTPUT);
  pinMode(PIN_IN4,OUTPUT);
}

void loop()
{
  analogWrite(PIN_VREF, 200);  //change pwm
  analogWrite(PIN_VREF2,200);
  double peak=0.0;
    for(int i=0;i<samples;i++){
      microseconds = micros();
      vReal[i] = analogRead(0);
      vImag[i] = 0;
      while(micros()<(microseconds+sampling_period_us)){ 
      }
    }
    FFT.Windowing(vReal,samples,FFT_WIN_TYP_HAMMING,FFT_FORWARD);
    FFT.Compute(vReal,vImag,samples,FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal,vImag,samples);
    peak = FFT.MajorPeak(vReal,samples,samplingFrequency); //peak has a major frequency(the most frequency)
    rc=peak;                    //no needness
    if(!move){
      if(rc<2500){                 //stop mode
        digitalWrite(PIN_IN1, LOW);
        digitalWrite(PIN_IN2, LOW);
        digitalWrite(PIN_IN3, LOW);
        digitalWrite(PIN_IN4, LOW);
        delayal=1;
      }
      else{
        move=true;
      }
    }
    else{
      if(rc<2250){                //this stop mode has lower limit than the upper one
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delayal=1;
          move=false;
      }
      else if(rc<3000){
        if(mode!=1){ 
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delay(200);
        }
        if(mode==1){               //back mode
          analogWrite(PIN_VREF, 200);
          analogWrite(PIN_VREF2,200);
          digitalWrite(PIN_IN1,HIGH);
          digitalWrite(PIN_IN2,LOW);
          digitalWrite(PIN_IN3,HIGH);
          digitalWrite(PIN_IN4,LOW);
          delayal=1000;}
          mode=1;
      }
      else if(rc<3600){
        if(mode==1){     
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delay(200);
        }
        if(mode==2){                  //front mode
          analogWrite(PIN_VREF, 200);
          analogWrite(PIN_VREF2,200);
          digitalWrite(PIN_IN1,LOW);
          digitalWrite(PIN_IN2,HIGH);
          digitalWrite(PIN_IN3,LOW);
          digitalWrite(PIN_IN4,HIGH);
          delayal=1000;}
          mode=2;
      }
      else if(rc<4250){
        if(mode==1){
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delay(200);
        }
        if(mode==3){                 //turn left
          analogWrite(PIN_VREF2,220);
          analogWrite(PIN_VREF,100);
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2,HIGH);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4,HIGH);
         delayal=3000;}
          mode=3;
      }
      else if(rc<5000){
        if(mode==1){ 
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delay(200);
        }
        if(mode==4){               //turn right
          analogWrite(PIN_VREF, 220);
          analogWrite(PIN_VREF2,100);
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2,HIGH);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4,HIGH);
          delayal=3000;}
          mode=4;
      }
      else{                      //exclusion
          digitalWrite(PIN_IN1,LOW);
          digitalWrite(PIN_IN2,LOW);
          digitalWrite(PIN_IN3,LOW);
          digitalWrite(PIN_IN4,LOW);
          delayal=100;
      }
    }
     delay(delayal);   //delay means the length of each mode
}
//mode=0 -> stop
//mode=1 -> back
//mode=2 -> foward
//mode=3||4 -> turn
//back->foward || foward->back is forbidden
