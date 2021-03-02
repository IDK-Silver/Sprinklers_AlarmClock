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
#include <LineMessage.h>

static LiquidCrystal_I2C lcd(0x27,16,2);
static DS3231 time_clock;
static AlarmDisplay ui(&lcd);
static TimeSystemData date(&time_clock);
static SP_IOStream io;
static FirebaseData fbdo;
static LineMessage line("notify-api.line.me", "TaJHhN9mJz1UD4abr18Msal2q5T0jHULGzbAlhEqt9n");
static struct pt pt_ui, pt_io, pt_sprinklers;

void setup() 
{
    time_clock.begin();
    lcd.init();
    lcd.backlight();
    PT_INIT(&pt_ui);
    PT_INIT(&pt_io);
    PT_INIT(&pt_sprinklers);
    WiFi.begin("Syaro", "istheorderarabbit"); 																	//	Connect WiFi
	while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); } 									//	Waiting Connect
	Serial.println("WiFi connected");	Serial.println("IP address: ");	Serial.println(WiFi.localIP());
    Firebase.begin("https://sprinklers-alarmclock-default-rtdb.firebaseio.com/", "oSC6YX0cvA3AlddtQdRH4HmTb4VyDMqmbGXu3w5L");\
    FirebaseJson json;
    json.set("IsAlarm", "false");
    if (Firebase.updateNode(fbdo, "/", json)) {} else { Serial.println(fbdo.errorReason()); }

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
                static FirebaseJson json;
                json.set("Hour",String(date.getHour()));
                json.set("Minute", String(date.getMinute()));
                if (Firebase.updateNode(fbdo, "/", json)) {} else { Serial.println(fbdo.errorReason()); }
                time_clock.setAlarm2(date.getDay(), date.getHour(), date.getMinute(), DS3231_MATCH_H_M, false); 
            }
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
        static FirebaseJson json;
        lastTime = millis();
        int throwEdit = 0;
        tone(pin_Buzz, 835);
        while (!io.isExitEditMode(throwEdit))
        {
            throwEdit = 0;
            io.runEditAlarmMode(throwEdit);
            if ((millis() - lastTime) >= 1000 * 5)
            {
                lcd.backlight();
                for (int i = 0; i < 100; i++)
                {
                    io.runEditAlarmMode(throwEdit);
                    digitalWrite(pin_Relay, HIGH);
                    delay(10);
                    if (throwEdit == throwExit || throwEdit == throwConfirm)
                    break;
                }
                lcd.noBacklight();
                for (int i = 0; i < 400; i++)
                {
                    io.runEditAlarmMode(throwEdit);
                    digitalWrite(pin_Relay, LOW);
                    delay(10);
                    if (throwEdit == throwExit || throwEdit == throwConfirm)
                    break;
                }
            }
            if ((millis() - lastTime) >= 1000 * 15)
            {
                line.setMessage("\n您的孩子還在睡覺");
                json.set("IsAlarm", "true");
                json.set("Sec", String(millis() - lastTime));
                
                line.sendMessage();
            }
            if (Firebase.updateNode(fbdo, "/", json)) {} else { Serial.println(fbdo.errorReason()); }
        }
        json.set("IsAlarm", "false");
        if (Firebase.updateNode(fbdo, "/", json)) {} else { Serial.println(fbdo.errorReason()); }
        lcd.backlight();
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

