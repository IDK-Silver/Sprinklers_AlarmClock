#include "SprinklersAlarmDisplay.h"

AlarmDisplay::AlarmDisplay(LiquidCrystal_I2C* lcd_address)
{
	lcd = lcd_address;
}

AlarmDisplay::~AlarmDisplay()
{
}

void AlarmDisplay::displayNowTime(TimeSystemData& data)
{
    lcd->clear();
	lcd->setCursor(0, 0);
	lcd->print("Date:");
	lcd->print(data.getYear());   lcd->print("-");
  	lcd->print(data.getMonth());  lcd->print("-");
  	lcd->print(data.getDay());    lcd->print(" ");
	lcd->setCursor(0, 1);
	lcd->print("Time:");
  	lcd->print(data.getHour());   lcd->print(":");
  	lcd->print(data.getMinute()); lcd->print(":");
  	lcd->print(data.getSecond()); lcd->print("");  
}


void AlarmDisplay::displayEditModeUI(TimeSystemData& data)
{


	lcd->clear();
	lcd->setCursor(0, 0);
	lcd->print("Edit:");
	lcd->print(data.getYear());	    lcd->print(":");
	lcd->print(data.getMonth());	lcd->print(":");
  	lcd->print(data.getDay());	    lcd->print("");
	lcd->setCursor(0, 1);
	lcd->print("Mode:");
  	lcd->print(data.getHour());     lcd->print(":");
  	lcd->print(data.getMinute());   lcd->print(":");
  	lcd->print(data.getSecond());   lcd->print("");  
}

void AlarmDisplay::displaySuccessConfirm(TimeSystemData& data)
{
	
}
