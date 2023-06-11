#include <arduinoFFT.h>
#define PIN_IN1 2
#define PIN_IN2 4
#define PIN_VREF 3   //pwm
#define PIN_VREF2 11
#define PIN_IN3 13
#define PIN_IN4 12
#define a 0.8
int keep;
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
const uint16_t samples = 128; 
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
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
}

void loop()
{
  analogWrite(PIN_VREF, 255);
  analogWrite(PIN_VREF2, 255);
  double peak=0.0;
  while(peak<1000){
    rup++;
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
    peak = FFT.MajorPeak(vReal,samples,samplingFrequency);
    if(rup>10)break;
  }
  rup=0;
    rc=peak;
    if(!move){
      if(rc<2500){
        digitalWrite(PIN_IN1, LOW);
        digitalWrite(PIN_IN2, LOW);
        digitalWrite(PIN_IN3, LOW);
        digitalWrite(PIN_IN4, LOW);
        delayal=1;
        mode=0;
      }
      else{
        move=true;
      }
    }
    else{
      if(rc<2250){
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delayal=1;
          move=false;
          mode=0;
      }
      else if(rc<2800){
        if(mode!=1){
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delay(100);
        }
          digitalWrite(PIN_IN1,HIGH);
          digitalWrite(PIN_IN2,LOW);
          digitalWrite(PIN_IN3,HIGH);
          digitalWrite(PIN_IN4,LOW);
          delayal=100;
          mode=1;
      }
      else if(rc<3200){
        if(mode!=2){
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delay(100);
        }
          digitalWrite(PIN_IN1,LOW);
          digitalWrite(PIN_IN2,HIGH);
          digitalWrite(PIN_IN3,LOW);
          digitalWrite(PIN_IN4,HIGH);
          delayal=100;
          mode=2;
      }
      else if(rc<3600){
        if(mode!=3){
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delay(100);
        }
          analogWrite(PIN_VREF,127);
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2,HIGH);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4,HIGH);
          delayal=100;
          mode=3;
      }
      else if(rc<4000){
        if(mode!=4){
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2, LOW);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4, LOW);
          delay(100);
        }
          analogWrite(PIN_VREF2, 127);
          digitalWrite(PIN_IN1, LOW);
          digitalWrite(PIN_IN2,HIGH);
          digitalWrite(PIN_IN3, LOW);
          digitalWrite(PIN_IN4,HIGH);
          delayal=100;
          mode=4;
      }
      else{
          digitalWrite(PIN_IN1,LOW);
          digitalWrite(PIN_IN2,LOW);
          digitalWrite(PIN_IN3,LOW);
          digitalWrite(PIN_IN4,LOW);
          delayal=100;
          mode=0;
      }
    }
     delay(delayal);
}





//mode=0 -> stop
//mode=1 -> back
//mode=2 -> foward
//mode=3||4 -> turn
//back->foward || foward->back is forbidden


















/*
void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    // Print abscissa value 
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
	break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
	break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
	break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}
*/


/* Build raw data 
  double cycles = (((samples-1) * signalFrequency) / samplingFrequency); //Number of signal cycles that the sampling will read
  for (uint16_t i = 0; i < samples; i++)
  {
    vReal[i] = int8_t((amplitude * (sin((i * (twoPi * cycles)) / samples))) / 2.0);/* Build data with positive and negative values
    //vReal[i] = uint8_t((amplitude * (sin((i * (twoPi * cycles)) / samples) + 1.0)) / 2.0);/* Build data displaced on the Y axis to include only positive values
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }
  /* Print the results of the simulated sampling according to time 
  //Serial.println("Data:");
  //PrintVector(vReal, samples, SCL_TIME);
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data 
  //Serial.println("Weighed data:");
  //PrintVector(vReal, samples, SCL_TIME);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT 
  //Serial.println("Computed Real values:");
  //PrintVector(vReal, samples, SCL_INDEX);
  //Serial.println("Computed Imaginary values:");
  //PrintVector(vImag, samples, SCL_INDEX);
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes 
  //Serial.println("Computed magnitudes:");
  //PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
  double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
  Serial.println(x, 6);
  while(1); /* Run Once 
   delay(2000); /* Repeat after delay */
