#include "SprinklersIOStream.h"

bool SP_IOStream::isClickedEditBtn()
{
    if (digitalRead(btnEditMode) == true) 
    {
        editMode = true;
        delay(500);
        return true;
    }
    return false;
}

bool SP_IOStream::isExitEditMode(int& throw_return)
{

    if (throw_return == throwExit || throw_return == throwConfirm)
    {
        return true;
    }
    else return false;
}

void SP_IOStream::runEditAlarmMode(int& throw_return)
{
    if (digitalRead(btnEditMode) == true) 
    {
        throw_return = throwExit;   
    }
    else if (digitalRead(btnAddHour) == true) 
    {
        throw_return = throwAddHour; while (digitalRead(btnAddHour) == true ) {}; delay(10);
    }
    else if (digitalRead(btnAddMinute) == true) 
    {
        throw_return = throwAddMin; while (digitalRead(btnAddMinute) == true) {}; delay(10);
    }
    else if (digitalRead(btnAlarmConfirm) == true) 
    {
        throw_return = throwConfirm;
    }
}

SP_IOStream::SP_IOStream(/* args */)
{
}

SP_IOStream::~SP_IOStream()
{
}