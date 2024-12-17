#include "component.h"

#define DHTPIN 28
#define DHTTYPE    DHT22   
DHT dht(DHTPIN, DHTTYPE);

unsigned long prevMillis = 0;
const long dhtTimer = 2000;
float last_temp, last_hum;  
void DTHbegin(){

    dht.begin();

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

void DTHupdate(){
      unsigned long currentMillis = millis();
        if (currentMillis - prevMillis >= dhtTimer) {// Update sensor value after 2 seconds
                prevMillis = currentMillis;
                readTempHumi();
            }
}