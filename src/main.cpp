#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <SprinklersAlarmDisplay.h>
#include <SprinklersTimeSystem.h>
#include <SprinklersIOStream.h>
#include <Wire.h>
#include <pt.h>
#include <ESP32Tone.h>
#include <LineNotifyESP32.h>
#include <analogWrite.h>

#define LINE_TOKEN "YOUR_LINE_NOTIFY_TOKEN"
#define WIFI_TIMEOUT 30000

static LiquidCrystal_I2C lcd(0x27, 16, 2);
static DS3231 time_clock;
static AlarmDisplay ui(&lcd);
static TimeSystemData date(&time_clock);
static SP_IOStream io;
static HTTPClientESP32Ex http;
static struct pt pt_ui, pt_io, pt_sprinklers;

void setup()
{
    /* Clock connect */
    time_clock.begin();

    /* LCD Screen init */
    lcd.init();
    lcd.backlight();

    /* protothread init */
    PT_INIT(&pt_ui);
    PT_INIT(&pt_io);
    PT_INIT(&pt_sprinklers);

    /* WiFi connecting */
    {
        /* set WiFi config */
        WiFi.begin("Syaro", "istheorderarabbit");

        /* the first request connect time */
        double last_time = millis();

        /* waiting WiFi connect */
        while (WiFi.status() != WL_CONNECTED && millis() - last_time <= WIFI_TIMEOUT)
        {
            Serial.print("Connecting");
            Serial.print(".");
            delay(500);
        }

        /* print ipifo */
        if (WiFi.status() == WL_CONNECTED)
        {
            lineNotify.init(LINE_TOKEN);
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
        }
    }

    /* set pin to output mode */
    pinMode(pin_Relay, OUTPUT);
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
            if (throwEdit == throwConfirm)
            {
                time_clock.setAlarm2(date.getDay(), date.getHour(), date.getMinute(), DS3231_MATCH_H_M, false);
            }
            else if (throwEdit == throwAddHour)
            {
                date.addHour();
                ui.displayEditModeUI(date);
            }
            else if (throwEdit == throwAddMin)
            {
                date.addMinute();
                ui.displayEditModeUI(date);
            }
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
            if ((millis() - lastTime) >= 1000 * 10)
            {
                lcd.backlight();
                for (int i = 0; i < 400; i++)
                {
                    io.runEditAlarmMode(throwEdit);
                    analogWrite(pin_Relay, 140);
                    delay(10);
                    if (throwEdit == throwExit || throwEdit == throwConfirm)
                        break;
                }
                lcd.noBacklight();
                for (int i = 0; i < 200; i++)
                {
                    io.runEditAlarmMode(throwEdit);
                    analogWrite(pin_Relay, LOW);
                    delay(10);
                    if (throwEdit == throwExit || throwEdit == throwConfirm)
                        break;
                }
            }
            if ((millis() - lastTime) >= 1000 * 20)
            {
                lineNotify.sendLineMessage(http, "\n您的孩子還在睡覺");
                lineNotify.sendLineMessage(http, "");
            }
        }

        lcd.backlight();
        noTone(pin_Buzz);
        analogWrite(pin_Relay, LOW);
    }
    PT_END(pt);
}

void loop()
{
    ptUIDisplay(&pt_ui);
    ptIO(&pt_io);
    ptSprinklers(&pt_sprinklers);
}
