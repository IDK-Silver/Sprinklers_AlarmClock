#ifndef SPRINKLERS_IO_STREAM
#define SPRINKLERS_IO_STREAM

#include <Arduino.h>

#define btnEditMode 27
#define btnAddHour  14
#define btnAddMinute 12
#define btnAlarmConfirm 13
#define pin_Buzz 25
#define pin_Relay 33


#define throwAddHour 9
#define throwAddMin 10
#define throwExit 11
#define throwConfirm 12

class SP_IOStream
{
private:
    bool editMode = false;
public:
    SP_IOStream(/* args */);
    ~SP_IOStream();
    bool isClickedEditBtn();
    bool isExitEditMode(int&);
    void runEditAlarmMode(int&);
};




#endif