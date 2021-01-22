#ifndef ALARM_DISPLAY_WITH_DS3231
#define ALARM_DISPLAY_WITH_DS3231

#include <LiquidCrystal_I2C.h>
#include <timeSystem.h>
#include <Wire.h>



class AlarmDisplay 
{
	
private:
    LiquidCrystal_I2C* lcd;
public:
    
    AlarmDisplay(LiquidCrystal_I2C*);
    ~AlarmDisplay();
    void displayNowTime(TimeSystemData&);
};


#endif