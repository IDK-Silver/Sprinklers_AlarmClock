#include "timeSystem.h"

TimeSystemData::TimeSystemData(DS3231* ds3231_address)
{
    clock = ds3231_address;
}

TimeSystemData::~TimeSystemData()
{
}

void TimeSystemData::updatedData()
{
    data = clock->getDateTime(); 
}

uint16_t TimeSystemData::getYear() {return data.year;}

uint8_t TimeSystemData::getMonth() {return data.month;}

uint8_t TimeSystemData::getDay() {return data.day;}

uint8_t TimeSystemData::getHour() {return data.hour;}

uint8_t TimeSystemData::getMinute() {return data.minute;}

uint8_t TimeSystemData::getSecond() {return data.second;}