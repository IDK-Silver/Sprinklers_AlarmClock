#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <SprinklersAlarmDisplay.h>
#include <SprinklersTimeSystem.h>
#include <SprinklersIOStream.h>
#include <Wire.h>
#include <pt.h>
#include <FirebaseESP32.h>
#include <ESP32Tone.h>

static LiquidCrystal_I2C lcd(0x27,16,2);
static DS3231 time_clock;
static AlarmDisplay ui(&lcd);
static TimeSystemData date(&time_clock);
static SP_IOStream io;
static FirebaseData fireData;
static struct pt pt_ui, pt_io, pt_sprinklers;

void setup() 
{
    time_clock.begin();
    lcd.init();
    lcd.backlight();
    PT_INIT(&pt_ui);
    PT_INIT(&pt_io);
    PT_INIT(&pt_sprinklers);
    Firebase.begin("", "");
    //time_clock.setDateTime(__DATE__, "00:27:42");
}

static int ptUIDisplay(struct pt *pt)
{
    PT_BEGIN(pt);
    static decltype(millis()) lastTime = 0;
    while (true)
    {
        date.updatedData();
        ui.displayNowTime(date);
        lastTime = millis();
        PT_WAIT_UNTIL(pt, millis() >= lastTime + 1000);
    }
    PT_END(pt);
}

static int ptIO(struct pt *pt)
{
    PT_BEGIN(pt);
    while (true)
    {
        PT_WAIT_UNTIL(pt, io.isClickedEditBtn());
        ui.displayEditModeUI(date);
        int throwEdit = 0;

        while (!io.isExitEditMode(throwEdit)) 
        {   
            throwEdit = 0;
            io.runEditAlarmMode(throwEdit);
            if (throwEdit == throwConfirm) { time_clock.setAlarm2(date.getDay(), date.getHour(), date.getMinute(), DS3231_MATCH_H_M, false); }
            else if (throwEdit == throwAddHour) { date.addHour(); ui.displayEditModeUI(date);}
            else if (throwEdit == throwAddMin) { date.addMinute(); ui.displayEditModeUI(date);}
            
        }
        delay(1000);
    }
    PT_END(pt);
}

static int ptSprinklers(struct pt *pt)
{
    PT_BEGIN(pt);
    static decltype(millis()) lastTime = 0;
    while (true)
    {   
        PT_WAIT_UNTIL(pt, time_clock.isAlarm2());
        lastTime = millis();
        int throwEdit = 0;
        tone(pin_Buzz, 835);
        while (!io.isExitEditMode(throwEdit))
        {
            throwEdit = 0;
            io.runEditAlarmMode(throwEdit);
            if ((millis() - lastTime) >= 1000 * 5)
            {
                lcd.noBacklight();
                for (int i = 0; i < 100; i++)
                {
                    
                    io.runEditAlarmMode(throwEdit);
                    digitalWrite(pin_Relay, HIGH);
                    delay(10);
                }
                lcd.backlight();
                for (int i = 0; i < 400; i++)
                {
                    
                    io.runEditAlarmMode(throwEdit);
                    digitalWrite(pin_Relay, LOW);
                    delay(10);
                }
            }
        }
        noTone(pin_Buzz);
        digitalWrite(pin_Relay, LOW);
        
    }
    PT_END(pt);
    
}

void loop() 
{
    ptUIDisplay(&pt_ui);
    ptIO(&pt_io);
    ptSprinklers(&pt_sprinklers);


}

