/* Demonstration sketch for PCF8574T I2C LCD Backpack 
Uses library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads GNU General Public License, version 3 (GPL-3.0) */

#include <dht.h>
#include <LCD5110_Basic.h>
#include "MQ135.h"

//空气质量
MQ135 gasSensor = MQ135(1);

//LCD5110
LCD5110 myGLCD(8,9,10,11,12);
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

//温湿度
dht DHT;
#define DHT11_PIN 5

//PM2.5
int dustPin=0;
float dustVal=0;
int ledPower=2;
int delayTime=280;
int delayTime2=40;
float offTime=9680;
float a=0;
String s0 = " ";
String s1 = " ";
String s2 = " ";
String s3 = " ";


void setup()
{
  Serial.begin(115200);
  pinMode(ledPower,OUTPUT);
  pinMode(dustPin, INPUT);
  myGLCD.InitLCD(); //Intializing LCD
  myGLCD.setContrast(55);
}

void pm25(){
  digitalWrite(ledPower,LOW); 
  delayMicroseconds(delayTime);
  dustVal=analogRead(dustPin); 
  delayMicroseconds(delayTime2);
  digitalWrite(ledPower,HIGH); 
  delayMicroseconds(offTime);
  //delay(1000);
  if (dustVal>36.455)
  a = (float(dustVal/1024)-0.0356)*120000*0.035;
  s0 = String("PM2.5: "+String(a));
  myGLCD.setFont(SmallFont);
  myGLCD.print(s0,0,8);
}

void ht(){
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
                Serial.print("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    case DHTLIB_ERROR_CONNECT:
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.print("Ack High error,\t");
        break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);

  myGLCD.setFont(SmallFont);
//  myGLCD.printNumI("hum",0,16);
  s1 = String("H : " + String(int(DHT.humidity)) + " %");
  myGLCD.print(s1,0,24);
  myGLCD.setFont(SmallFont);
  s2 = String("T : " + String(int(DHT.temperature))+" C");
  myGLCD.print(s2,0,32);
  
  }

void ppm(){
  float ppm = gasSensor.getPPM();
  s3 = String("Air: " + String(ppm)+"ppm");
  myGLCD.print(s3,0,16);
  }

void loop()
{ 
  
  myGLCD.setFont(SmallFont);
  myGLCD.print("Hello World",CENTER,0);



  pm25();
  ht();
  ppm();
  
  delay(2000);
  myGLCD.clrScr();





}
