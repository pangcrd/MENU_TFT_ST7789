
#include "component.h"

#define UsrBTN1 35
#define UsrBTN2 36

/////////////////////////////////////////////////////////// setup ///////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  pinAtcv();
  encoder.begin(); 
  drawMainMenu(); 
  DTHbegin();
  pinMode(UsrBTN1, INPUT);
  pinMode(UsrBTN2, INPUT);

}

/////////////////////////////////////////////////////////// loop ///////////////////////////////////////////////////
void loop() {

  if (!inSubMenu) {
        mainMenufunc();
    } else {
        switch (menuIndex) {
            case 0:
                subMenuLED(); 
                break;
            case 1:
                subMenuFan(); 
                break;
            case 2:{
                subTempHumi();
                DTHupdate();
                break;
            }
            case 3:
                subInfo();
                break;

    }
    }
  
int buttonState1 = digitalRead(UsrBTN1);  
int buttonState2 = digitalRead(UsrBTN2); 

    if (buttonState1 == HIGH) {                 
        Serial.println("User Button 1 Pressed!");
        }

    if (buttonState2 == HIGH) {                 
        Serial.println("User Button 2 Pressed!");
   
    }


}