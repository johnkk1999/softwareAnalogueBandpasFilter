#include <Arduino.h>
typedef struct{
  float coeff[2];
  float out[2];
}RcLowPass;

typedef struct{
  float coeff;
  float out[2];
}RcHighPass;

void RcFilterInit(RcHighPass*, RcLowPass*, float, float,float);
void RcFilterUpdate(RcHighPass*,RcLowPass* , float);

RcLowPass filtLow;
RcHighPass filtHigh;


void setup()
{
  Serial.begin(9600);
  RcFilterInit(&filtHigh,&filtLow,220.0e-6,40,60);
analogReadResolution(16);
}
void loop() {
 float voltage = analogRead(A0);
 voltage = 3.3 * voltage/65536.0;
 RcFilterUpdate(&filtHigh,&filtLow,voltage);
 Serial.print(">not filtered:");
 Serial.println(voltage);
 Serial.print(">filtered:");
 Serial.println(filtHigh.out[0]);

 
}
void RcFilterInit(RcHighPass *filtH, RcLowPass* filtL ,float sampTime, float lowFreq, float highFreq)
{
  //low pass filter
  float RC = 1/(6.2831853072f*lowFreq);
  filtL->coeff[0] = sampTime/(sampTime+RC);
  filtL->coeff[1] = RC /(sampTime + RC);
  
  //high pass filter
  float RC2 = 1/(6.2831853072f*highFreq);
  filtH->coeff = RC2/(sampTime+RC2);
  //initial outputs to zero
  filtL->out[0] = 0.0f;
  filtL->out[1] = 0.0f;
  filtH->out[0] = 0.0f;
  filtH->out[1] = 0.0f;

}
void RcFilterUpdate(RcHighPass* filtH,RcLowPass* filtL, float input)
{
  //low pass filter
  filtL->out[1] = filtL->out[0];
  filtL->out[0] = filtL->coeff[0] * input + filtL->coeff[1] * filtL->out[1];

  //high pass filter

  filtH->out[1] = filtH->out[0];
  filtH->out[0] = filtH->coeff* (filtL->out[0] - filtL->out[1]) + filtH->coeff*filtH->out[1];

  }