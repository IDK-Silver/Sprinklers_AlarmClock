#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <AlarmDisplay.h>
#include <DS3231.h>
#include <TimeSystem.h>
#include <Wire.h>
#include <pt.h>


LiquidCrystal_I2C lcd(0x27,16,2);
DS3231 time_clock;
AlarmDisplay ui(&lcd);
TimeSystemData data(&time_clock);


void setup() 
{
    time_clock.begin();
    lcd.init();
    lcd.setCursor(0, 0);
    lcd.clear();

}

void loop() 
{
    data.updatedData();
    ui.displayNowTime(data);
    delay(1000);
}