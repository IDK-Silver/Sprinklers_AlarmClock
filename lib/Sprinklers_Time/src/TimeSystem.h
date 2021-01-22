#ifndef DS3231_CLOCK_SYSTEM
#define DS3231_CLOCK_SYSTEM

#include <DS3231.h>

class TimeSystemData
{
private:
    DS3231 *clock;
    RTCDateTime data;
public:
    TimeSystemData(DS3231*);
    ~TimeSystemData();
    void updatedData();
    uint16_t getYear();
    uint8_t getMonth();
    uint8_t getDay();
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getSecond();

};








#endif