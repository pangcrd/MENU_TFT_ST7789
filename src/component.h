#ifndef COMPONENT_H
#define COMPONENT_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Adafruit_Sensor.h>
#include "EncoderRead.h"
//#include "linesplit.h"
#include <SPI.h>
#include <DHT.h> 

extern TFT_eSPI tft;  
extern EncoderRead encoder;

extern int menuIndex; 
extern bool inSubMenu; 

//Main menu
void drawMainMenu();
void DrawLedControl();
void DrawFanControl();
void DrawTempHumi();
void DrawInfo();
void mainMenufunc();
//sub Menu
void subMenuLED();
void subMenuFan();
void subTempHumi();
void subInfo();

void pinAtcv();
void count();
//DTH22
void DTHbegin();
void readTempHumi();
void DTHupdate();

void Buzzbeep();
void updateBuzz();



#endif