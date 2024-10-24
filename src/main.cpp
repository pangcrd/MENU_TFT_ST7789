/*TFT ST7789V MENU test with RP2040*/
/*Mục đích chia sẻ học hỏi là chủ yếu*/
/*Có thể có nhiều thiếu sót, các bạn có thể sửa lại và cho mình ý kiến :))*/

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Adafruit_Sensor.h>
#include "EncoderRead.h"
//#include "linesplit.h"
#include <SPI.h>
#include <DHT.h> 

#define LED 15
#define FAN 14


TFT_eSPI    tft = TFT_eSPI();    
EncoderRead encoder(27, 22, 26); // GPIO EC11 pinA,pinB,Button

#define DHTPIN 28
#define DHTTYPE    DHT22   
DHT dht(DHTPIN, DHTTYPE);

int ledState = 255; 
bool fanState = false; 
int lastValue = 0;


int menuIndex = 0;
bool inSubMenu = false; // Kiểm tra xem có đang ở trong submenu không
int submenuIndex = 0;
int submenuIndex1 = 0;
int submenuIndex2 = 0;
int submenuIndex3 = 0;

bool controlPWM = false;
int pwmValue = 0; 

unsigned long prevMillis = 0;
const long dhtTimer = 2000;
float last_temp, last_hum;  


const unsigned char PROGMEM folder_ico[] = {0x3c,0x00,0x00,0x42,0x00,0x00,
    0x81,0xff,0x00,0x80,0x00,0x80,0xbf,0xfe,0x80,0xc0,0x01,0x80,0x80,0x00,0x80,0x80,
    0x00,0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x80,0x00,
    0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x7f,0xff,0x00};

static const unsigned char PROGMEM DolphinFlip[] = {0x10,0x40,0x00,0x00,0x00,0x00,0x10,0x40,0x00,
    0x00,0x00,0x00,0x10,0x80,0x3f,0xf8,0x00,0x00,0x10,0x80,0xc0,0x06,0x00,0x00,0x11,0x03,0x00,0x01,
    0x80,0x00,0x11,0x04,0x00,0x00,0x40,0x00,0x00,0x08,0x00,0x00,0x20,0x00,0x12,0x10,0x00,0x00,0x10,
    0x00,0x00,0x10,0x00,0x00,0x08,0x00,0x00,0x20,0x00,0x00,0x08,0x00,0x3c,0x20,0x00,0x78,0x04,0x00,
    0x43,0x40,0x00,0x84,0x04,0x00,0x80,0xc0,0x01,0x7a,0x02,0x00,0x80,0x30,0x02,0xdd,0x02,0x00,0xf0,
    0x0c,0x02,0x9d,0x02,0x00,0x7c,0x03,0x02,0xfd,0x02,0x00,0x3e,0x00,0x02,0xfd,0x01,0x00,0x1f,0x80,
    0x01,0x7a,0x01,0x00,0x0f,0xc0,0x01,0xf4,0x01,0x00,0x07,0xe0,0x02,0x08,0x01,0x00,0x03,0xf0,0x00,
    0x04,0x01,0x00,0x01,0xf8,0x00,0x00,0x01,0x00,0x7f,0x86,0x00,0x00,0x01,0x00,0x9e,0x01,0x80,0x06,
    0x01,0x00,0x81,0xff,0xf8,0x09,0x01,0x00,0x80,0x00,0x00,0x09,0x01,0x80,0x40,0x00,0x00,0x09,0x01,
    0x80,0x30,0x00,0x00,0x09,0x01,0xc0,0x0e,0x00,0x00,0x08,0xf0,0xc0,0x01,0xff,0x00,0x09,0x08,0xe0,
    0x00,0xfc,0x00,0x0e,0x04,0xe0,0x00,0x80,0x00,0x12,0x04,0x70,0x00,0x80,0x00,0x12,0x14,0x70,0x00,
    0x80,0x00,0x12,0x14,0x78,0x00,0x80,0x00,0x11,0xe4,0x38,0x00,0x80,0x00,0x08,0xe2,0x38,0x00,0x80,
    0x00,0x04,0x02,0x38,0x01,0x00,0x00,0x02,0x01,0x18,0x01,0x00,0x00,0x02,0x01,0x18,0x01,0x00,0x00,
    0x02,0x00,0x98,0x01,0x00,0x00,0x02,0x00,0x88,0x01,0x00,0x00,0x02,0x00,0x48};

void drawMainMenu() { // Design UI on https://lopaka.app/ 
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(0xF95C);
    tft.setTextSize(2);
    tft.setFreeFont();
    tft.drawString("MENU TFT ST7789", 31, 27);

    tft.drawRect(34, 75, 194, 40, (menuIndex == 0) ? 0x57EA : 0xFFFF);
    tft.setTextColor(0xFAAA);
    tft.drawString("LED CONTROL", 68, 87);

    tft.drawRect(33, 143, 194, 40, (menuIndex == 1) ? 0x57EA : 0xFFFF);
    tft.setTextColor(0xFFEA);
    tft.drawString("FAN CONTROL", 69, 155);

    tft.drawRect(33, 208, 194, 40, (menuIndex == 2) ? 0x57EA : 0xFFFF);
    tft.setTextColor(0xFABF);
    tft.drawString("TEMPERATURE", 69, 220);

    tft.drawRect(33, 274, 194, 40, (menuIndex == 3) ? 0x57EA : 0xFFFF);
    tft.setTextColor(0xFFFF);
    tft.drawString("INFO", 100, 286);

    int icon = 77 + (menuIndex * 65);
    tft.drawBitmap(8, icon + 10, folder_ico, 18, 18, 0xFE05);
}

void DrawLedControl() {
    inSubMenu = true; // Kiểm tra xem đã vào submenu chưa
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.drawString("LED CONTROL", 31, 27);
   
    tft.fillRoundRect(36, 101, 70, 35, 10, (submenuIndex == 0) ? TFT_CYAN : 0xFFFF);
    tft.setTextSize(2);
    tft.setTextColor(0xFAAA);
    tft.drawString("ON", 61, 111);

    tft.fillRoundRect(142, 102, 70, 35, 10, (submenuIndex == 1)? TFT_CYAN : 0xFFFF);
    tft.setTextSize(2);
    tft.setTextColor(0xFAAA);
    tft.drawString("OFF", 163, 113);

    tft.drawRoundRect(76, 247, 92, 39, 5,(submenuIndex == 2)? TFT_GREEN : 0xFFFF);
    tft.setTextSize(2);
    tft.setTextColor(TFT_GREEN);
    tft.drawString("BACK", 99, 259);

}

void DrawFanControl() {
    inSubMenu = true;
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.drawString("FAN CONTROL", 31, 27);

    tft.fillRoundRect(36, 101, 70, 35, 10, (submenuIndex1 == 0) ? 0x57FF : 0xFFFF);
    tft.setTextSize(2);
    tft.setTextColor(0xFAAA);
    tft.drawString("ON", 61, 111);

    tft.fillRoundRect(142, 102, 70, 35, 10, (submenuIndex1 == 1)? 0x57FF : 0xFFFF);
    tft.setTextSize(2);
    tft.setTextColor(0xFAAA);
    tft.drawString("OFF", 163, 113);

    tft.drawRect(42, 190, 158, 24,(submenuIndex1 == 2)? 0xFFEA : 0xFFFF);
    tft.setTextSize(2);
    tft.setTextColor(0xFABF);
    tft.drawString("Speed:", 42, 168);

    tft.drawRoundRect(76, 247, 92, 39, 5,(submenuIndex1 == 3)? TFT_GREEN : 0xFFFF);
    tft.setTextSize(2);
    tft.setTextColor(TFT_GREEN);
    tft.drawString("BACK", 99, 259);
}



void DrawTempHumi() {
    inSubMenu = true;
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.drawString("TEMP&HUMI", 40, 27);
    tft.fillRoundRect(67, 107, 110, 37, 5, 0xAEBA);
    tft.fillRoundRect(67, 187, 110, 37, 5, 0xAEBA);


    tft.setTextColor((submenuIndex2 == 0) ? 0xFABF : 0xFFFF);
    tft.setTextSize(2);
    tft.drawString("Temperture", 65, 75);

    tft.setTextColor((submenuIndex2 == 1) ? 0xFABF : 0xFFFF);
    tft.setTextSize(2);
    tft.drawString("Humidity",  76, 160);

    tft.drawRoundRect(76, 247, 92, 39, 5,(submenuIndex2 == 2)? TFT_GREEN : 0xFFFF);
    tft.setTextSize(2);
    tft.setTextColor(TFT_GREEN);
    tft.drawString("BACK", 99, 259);

}

void DrawInfo() {
    inSubMenu = true;
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_SKYBLUE);
    tft.setTextSize(2);
    tft.drawString("INFO", 98, 27);

    tft.setTextColor((submenuIndex3 == 0) ? 0xF800 : 0xFFFF);
    tft.setTextSize(2);
    tft.drawString("Version",  78, 61);
    tft.drawString("0.1.10",  86, 90);

    tft.setTextColor((submenuIndex3 == 1) ? 0xF800 : 0xFFFF);
    tft.setTextSize(2);
    tft.drawString("Author",  80, 120);
    tft.drawString("HO VAN BANG",  50, 145);

    tft.drawRoundRect(76, 260, 92, 39, 5,(submenuIndex3 == 2)? TFT_GREEN : 0xFFFF);
    tft.setTextSize(2);
    tft.setTextColor(TFT_GREEN);
    tft.drawString("BACK", 101, 271);


}

void mainMenufunc(){
    /*Giải thích: (inSubMenu ? 3 : 4) */
    /*3 là số lượng mục trong submenu.*/
    /*4 là số lượng mục trong menu chính. Khi không ở trong submenu (tức là inSubMenu là false), menuIndex sẽ lặp qua 4 mục.*/
    /*Có thể thêm nhiều menu theo nhu cầu*/
 static int lastPos = 0;
    int pos = encoder.getCounter();
    if (pos != lastPos) {
        if (pos > lastPos) {
            menuIndex = (menuIndex + 1) % (inSubMenu ? 3 : 4); 
        } else {
            menuIndex = (menuIndex - 1 + (inSubMenu ? 3 : 4)) % (inSubMenu ? 3 : 4); 
        }
        drawMainMenu();
        lastPos = pos;
    }
    if (encoder.encBtn()) { 
        if (inSubMenu) {
            inSubMenu = false;
            drawMainMenu();
        } else {
             encoder.resetCounter();
            switch (menuIndex) {
                case 0:
                    DrawLedControl();
                    break;
                case 1:
                    DrawFanControl();
                    break;
                case 2:
                    DrawTempHumi();
                    break;
                case 3:
                    DrawInfo();
                    break;
            }
        }
        
    }


}
void subMenuLED(){
 static int lastSubPos = -1;
    int newSubPos = encoder.getCounter();

    if (newSubPos != lastSubPos) {
        submenuIndex = newSubPos % 3;  // 3 mục trong submenu: Turn ON, Turn OFF, Back
        DrawLedControl();           // Tạo theo nhu cầu
        lastSubPos = newSubPos;
    }
    if (encoder.encBtn()) {
        if (submenuIndex == 0) {
            digitalWrite(LED, HIGH);
            tft.setTextSize(2);
            tft.setTextColor(0xFABF);
            tft.drawString("LED is ON", 68, 193);
        } else if (submenuIndex == 1) {
            digitalWrite(LED, LOW);
            tft.setTextColor(0xFABF);
            tft.setTextSize(2);
            tft.drawString("LED is OFF", 68, 193);
        } else if (submenuIndex == 2) {
            inSubMenu = false;
            submenuIndex = 0; // Reset submenuIndex
            encoder.resetCounter();
            drawMainMenu();
        }
        delay(500);  
    }
}

void subMenuFan() {
    static int lastSubPos1 = -1;
    int newSubPos1 = encoder.getCounter();

    if (!controlPWM) {
        //Điều hướng qua menu nếu không ở chế độ điều khiển PWM
        if (newSubPos1 != lastSubPos1) {
            submenuIndex1 = newSubPos1 % 4;  //4 mục trong submenu: Turn ON, Turn OFF,PWM slide, Back
            DrawFanControl();
            lastSubPos1 = newSubPos1;
        }
    }
    //Điều khiển ON/OFF
    //ĐIều khiển theo PWM
    if (encoder.encBtn()) {
        if (submenuIndex1 == 0) {
            digitalWrite(FAN, HIGH);
            fanState = true;
            controlPWM = false;
            pwmValue = 255;
            analogWrite(FAN, pwmValue); 

        } else if (submenuIndex1 == 1) {
            digitalWrite(FAN, LOW);
            fanState = false;
            controlPWM = false;
            pwmValue = 0;
            analogWrite(FAN, pwmValue); 

        } else if (submenuIndex1 == 2) {
            controlPWM = !controlPWM;
            if (!controlPWM && fanState) {
                analogWrite(FAN, pwmValue);  
            } else if (!fanState) {
                analogWrite(FAN, 0);  
            }
        
        } else if (submenuIndex1 == 3) {
            inSubMenu = false;
            submenuIndex1 = 0;
            encoder.resetCounter();
            drawMainMenu();
        }
        delay(500);
    }

    if (controlPWM) {
        int encVal = newSubPos1; 
        pwmValue = map(encVal, 0, 100, 0, 255); 
        pwmValue = constrain(pwmValue, 0, 255);  
        int speedPer = (pwmValue *100) /255 ;
        analogWrite(FAN, pwmValue);  // Cập nhật PWM để điều khiển tốc độ

        // Vẽ thanh trượt
        int slide = map(pwmValue, 0, 255, 0, 156); 
        if (pwmValue > lastValue) {
            tft.fillRect(43 + map(lastValue, 0, 255, 0, 156), 191, slide - map(lastValue, 0, 255, 0, 156), 22, 0x57FF); 
        } else if (pwmValue < lastValue) {
            tft.fillRect(43 + slide, 191, map(lastValue, 0, 255, 0, 156) - slide, 22, TFT_BLACK); 
        }

        lastValue = pwmValue;    
        // In giá trị PWM lên lcd
        tft.setCursor(155, 170);
        tft.fillRoundRect(143, 168, 57, 21, 8, 0xFFFF);
        tft.setTextColor(0xF800);
        tft.println(speedPer);
       

    } else if (fanState) {

        analogWrite(FAN, pwmValue); 
        int slide = map(pwmValue, 0, 255, 0, 156); 
        tft.fillRect(43, 191, slide, 22, 0x57FF);  /*Giữ lại giá trị slide sau khi không sử dụng PWM*/

    } else {
        analogWrite(FAN, 0);
        tft.fillRect(43, 191, 156, 22, TFT_BLACK); /*Xóa slide khi OFF*/
    }
}

void subTempHumi(){

    static int lastSubPos2 = -1;
    int newSubPos2 = encoder.getCounter();


         if (newSubPos2 != lastSubPos2) {
            submenuIndex2 = newSubPos2 % 3; 
            DrawTempHumi();
            lastSubPos2 = newSubPos2;
        }

        if (encoder.encBtn()) {
            if(submenuIndex2 == 2){
                    inSubMenu = false;
                    submenuIndex2 = 0;
                    encoder.resetCounter();
                    drawMainMenu();
            }
            delay(500);  
        }

}

void readTempHumi(){
   float temperature = dht.readTemperature();
   float humidity = dht.readHumidity();


        if (temperature != last_temp)
        {
            tft.fillRect(87, 112, 80, 27, 0xAEBA);
            tft.setTextColor(0xFAAA);
            tft.setTextSize(2.5);
            tft.setCursor(87, 117);
            tft.print(temperature,1);         
            tft.print((char)247);     // Symbol '°' 
            tft.print("C");
             
        }
            if ( humidity != last_hum)
        {
            tft.fillRect(87, 192, 76, 27, 0xAEBA);
            tft.setTextColor(0xFABE);
            tft.setTextSize(2.5);
            tft.setCursor(91, 197);
            tft.print(humidity,1);
            tft.print("%"); 
            
        }

    last_temp = temperature;
    last_hum = humidity;
}

void subInfo(){

 static int lastSubPos3 = -1;
    int newSubPos3 = encoder.getCounter();

         if (newSubPos3 != lastSubPos3) {
            submenuIndex3 = newSubPos3 % 3; 
            DrawInfo();
            lastSubPos3 = newSubPos3;
        }

        if (encoder.encBtn()) {
            if(submenuIndex3 == 2){
                    inSubMenu = false;
                    submenuIndex3 = 0;
                    encoder.resetCounter();
                    drawMainMenu();
            }
            delay(500);  
        }
        tft.drawBitmap(95, 175, DolphinFlip, 45, 42, TFT_WHITE); 
    

}

/////////////////////////////////////////////////////////// setup ///////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  pinMode(FAN,OUTPUT); 
  pinMode(LED, OUTPUT);
  encoder.begin(); 
  drawMainMenu(); 
  dht.begin();
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
                  unsigned long currentMillis = millis();
                    if (currentMillis - prevMillis >= dhtTimer) {// Update sensor value after 2 seconds
                        prevMillis = currentMillis;
                        readTempHumi();
                    }
                break;
            }
            case 3:
                subInfo();
                break;

    }
    }
  
}
