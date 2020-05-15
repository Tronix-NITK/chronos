#include <Wire.h>
#include <RealTimeClockDS1307.h>
#define a 52
#define b 50
#define c 48
#define d 46
#define e 44
#define f 42
#define g 40
#define red   11
#define green 12
#define blue  13
int rval=0,gval=255,bval=255;


int engdat=10,enghour=0,engmin=0,engsec=0;    
int curdat=8,curhours=23,curminutes=59,curseconds=45;
int edat=engdat-1,ehour,emin,esec;


int en[6]={22,24,26,28,30,32};
void setup() 
{
  int i=0;
  RTC.setDate(curdat);
  RTC.setHours(curhours);
  RTC.setMinutes(curminutes);
  RTC.setSeconds(curseconds);
  RTC.setClock();

  
  for(i=40;i<=52;i+=2)
    pinMode(i,OUTPUT);
  for(i=22;i<=32;i+=2)
    pinMode(i,OUTPUT);
  pinMode(red,OUTPUT);
  analogWrite(red,rval);
  pinMode(green,OUTPUT);
  analogWrite(green,gval);
  pinMode(blue,OUTPUT);
  analogWrite(blue,bval);

  
  (enghour==0)?(ehour=23):(ehour=enghour);
  (engmin==0)?(emin=59):(emin=engmin);
  (engsec==0)?(esec=59):(esec=engsec);
  Serial.begin(9600); 
}
int dat59=RTC.getDate()-1;
int secprev=RTC.getSeconds();
void loop() 
{
  out(LOW,LOW,LOW,LOW,LOW,LOW,LOW);
  RTC.readClock();
  if(RTC.getSeconds()!=secprev)
    rgb(rval,gval,bval);
  int hours=RTC.getHours();
  int minutes=RTC.getMinutes();
  int seconds=RTC.getSeconds();
  Serial.print(hours);
  Serial.print(minutes);
  Serial.println(seconds);
  if(hours==23 && minutes==59 && seconds==59)
    dat59=RTC.getDate();
  if(hours==0 && minutes==0 && seconds==0)
  {
    RTC.setDate(++dat59);
    RTC.setClock();
  }
  int dat=RTC.getDate();
  if(engdat==dat && enghour==hours && engmin==minutes && engsec==seconds)
  {
    RTC.setDate(engdat);
    RTC.setHours(enghour);
    RTC.setMinutes(engmin);
    RTC.setSeconds(engsec);
    RTC.setClock();
    dat=RTC.getDate();
    hours=RTC.getHours();
    minutes=RTC.getMinutes();
    seconds=RTC.getSeconds();
    edat=engdat;
    ehour=enghour;
    emin=engmin;
    esec=engsec;
  }
  if(edat-dat>=1)
  {
    Serial.print(toPos(edat,dat,0));
    Serial.print(":");
    Serial.print(toPos(ehour,hours,2));
    Serial.print(":");
    Serial.println(toPos(emin,minutes,4));
    toPos(edat,dat,0);
    delay(1);
    toPos(ehour,hours,2);
    delay(1);
    toPos(emin,minutes,4);
    delay(1);
  }
  else
  {
    Serial.print(toPos(ehour,hours,0));
    Serial.print(":");
    Serial.print(toPos(emin,minutes,2));
    Serial.print(":");
    Serial.println(toPos(esec,seconds,4));
    toPos(ehour,hours,0);
    delay(1);
    toPos(emin,minutes,2);
    delay(1);
    toPos(esec,seconds,4);
    delay(1);
  }
  secprev=RTC.getSeconds();
  delay(1000);
}
int toPos(int a1,int b1,int k)   //separates the digits and sends to rgbDigit()
{
  int c3=(a1>b1)?(a1-b1):(b1-a1);
  int c0=c3%10;
  int c1=c3/10; //k is the digit's index
  digitalWrite(en[k],HIGH);
  rgbDigit(c1,k);
  delay(1);
  digitalWrite(en[k],LOW);
  digitalWrite(en[k+1],HIGH);
  rgbDigit(c0,k+1);
  digitalWrite(en[k+1],LOW);
  return (a>b)?(a-b):(b-a);
}
void rgbDigit(int dig,int k)
{
  if(en[k]==HIGH)
  {
    if(dig==0)
      out(HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,LOW);
    if(dig==1)
      out(LOW,HIGH,HIGH,LOW,LOW,LOW,LOW);
    if(dig==2)
      out(HIGH,HIGH,LOW,HIGH,HIGH,LOW,HIGH);
    if(dig==3)
      out(HIGH,HIGH,HIGH,HIGH,LOW,LOW,HIGH);
    if(dig==4)
      out(LOW,HIGH,HIGH,LOW,LOW,HIGH,HIGH);
    if(dig==5)
      out(HIGH,HIGH,HIGH,LOW,LOW,HIGH,HIGH);
    if(dig==6)
      out(HIGH,LOW,HIGH,HIGH,HIGH,HIGH,HIGH);
    if(dig==7)
      out(HIGH,HIGH,HIGH,LOW,LOW,LOW,LOW);
    if(dig==8)
      out(HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH);
    if(dig==9)
      out(HIGH,HIGH,HIGH,HIGH,LOW,HIGH,HIGH);
  }
}
void out(boolean aout,boolean bout,boolean cout,boolean dout,boolean eout,boolean fout,boolean gout)
{
  digitalWrite(a,aout);
  digitalWrite(b,bout);
  digitalWrite(c,cout);
  digitalWrite(d,dout);
  digitalWrite(e,eout);
  digitalWrite(f,fout);
  digitalWrite(g,gout);
}
void rgb(int rval,int gval,int bval)
{
  if(rval<255 && gval>0 && bval==255)
  {
    rval=rval+5;
    gval=gval-5;
  }
  if(rval==255 && gval<255 && bval>0)
  {
    gval=gval+5;
    bval=bval-5;
  }
  if(rval>0 && gval==255 && bval<255)
  {
    bval=bval+5;
    rval=rval-5;
  }
  analogWrite(red,rval);
  analogWrite(green,gval);
  analogWrite(blue,bval);
}
