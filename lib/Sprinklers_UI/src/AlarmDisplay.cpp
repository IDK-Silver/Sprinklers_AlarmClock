#include "AlarmDisplay.h"

AlarmDisplay::AlarmDisplay(LiquidCrystal_I2C* lcd_address)
{
	lcd = lcd_address;
}

AlarmDisplay::~AlarmDisplay()
{
}

void AlarmDisplay::displayNowTime(TimeSystemData& data)
{
    /*
	lcd->clear();
	lcd->backlight();
	lcd->setCursor(0, 0);
	lcd->print("Loli");
	delay(1000);
	lcd->clear();
	lcd->noBacklight();
    */



    
    lcd->clear();
	lcd->setCursor(0, 0);
	lcd->print("Data:");
	lcd->print(data.getYear());   lcd->print("-");
  	lcd->print(data.getMonth());  lcd->print("-");
  	lcd->print(data.getDay());    lcd->print(" ");
	lcd->setCursor(0, 1);
	lcd->print("Time:");
  	lcd->print(data.getHour());   lcd->print(":");
  	lcd->print(data.getMinute()); lcd->print(":");
  	lcd->print(data.getSecond()); lcd->print("");
    
    
}

