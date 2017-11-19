#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>

//BRUKTE PINNER
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);//(rs, e, d4, d5, d6, d7)
int JoyStick_X = 0;
int JoyStick_Y = 1;
int JoyStick_Z = 8;
int btnback = 9;
int btnSnooze = 10;

//GLOBALE VARIABLER
int h = hour(); //setter "h" til å være nåværende time til klokken.
int mi = minute(); //setter "mi" til å være nåvernde minutt til klokken.
int d = day(); //setter "d" til å være nåværende dag til klokken (1-7).
int mo = month(); //setter "mo" til nåværende måned til klokken (1-12).
int yr = year(); //setter "yr" til å være nåværende år til klokken.
int se = 0; //Konstant for å få sekundene til å starte på 0.
int countAlarm = 0; //Kontrollvariabel for engangsalarmene.
int snoozeTime = 61;//Kontrollvariabel for snooze.
int setAlarmDailyVar = 0; //Kontrollvariabel for å vite om en alarm er satt eller ikke.
int setAlarmWeekdayVar = 0; //Kontrollvariabel for å vite om en alarm er satt eller ikke.
int setAlarmWeeklyVar = 0; //Kontrollvariabel for å vite om en alarm er satt eller ikke.
int setAlarmOnceVar = 0; //Kontrollvariabel for å vite om en alarm er satt eller ikke.
int snoozeVar = 10; //Variabel for å endre snoozetiden.

//DEKLARERTE FUNKSJONER
void printTime();//Skriver ut tiden.
void printDate();//Skriver ut datoen.
void change_state(); //Kaller på riktig funksjon med tanke på hvilken state klokken er i.
void displayMenuSetTime();//Viser menyvalg "set time" til skjerm.
void displayMenuSetAlarm();//Viser menyvalg "set alarm" til skjerm.
void displayMenuSetSnooze();//Viser menyvalget "set snooze" til skjerm.
void displayalarm(); //Viser at alarmen går på skjerm.
void FreqWeekday(); //Viser alarmtype "Weekday" til skjerm.
void FreqDaily(); //Viser alarmtype "Daily" til skjerm.
void FreqWeekly(); //Viser alarmtype "Weekly" til skjerm.
void FreqOnce(); //Viser alarmtype "Once" til skjerm.
void clearAlarmOnce1();//Sletting av engangsalarmen i posisjon 1
void clearAlarmOnce2();//Sletting av engangsalarmen i posisjon 2
void clearAlarmOnce3();//Sletting av engangsalarmen i posisjon 3
void clearAlarmDaily(); //Sletter alarmen som går av daglig.
void clearAlarmWeekly(); //Sletter alarmen som går av ukentlig.
void clearAlarmWeekday(); //Sletter alarmen som går av på hverdager.
void SetSnooze();//Viser instillingsvalg av snoozetid til skjerm.
void snooze(); //utsetter alarm x-antall min. Istede for å sette klokken på nytt, velger vi derfor å regner ut (minutt klokke går av) + X min
void checkAlarm(int type, int h, int mi, int d, int mo, int yr); //Sjekker om en alarm kan lagres eller ikke og viser til skjerm.
void snoozeTimeChecker(int snoozeTime); //Sjekker når alarmen skal gå igjen etter slumring.
void alarmSound();//Alarmlyd.
void printCorrect(int digits);//Passer på at tallene blir skrevet ut i riktig format.
void MenyPil(); //Viser pil til høyre og venstre til skjerm.
void menyPilTopp(); ////Viser piler på øverste del av skjerm under sletting av engangsalarmer.
void alarmOnceChecker(); //Sjekker om engangsalarmene skal gå av.
void alarmWeekdayChecker();//Sjekker om ukedagsalarmen skal gå av.
void alarmWeeklyChecker();//Sjekker om den ukentlige alarmen skal gå av.
void alarmDailyChecker(); //Sjekker om den daglige alarmen skal gå av.
void buttonBack(); //Navigeringsfunksjon som gjør at man kan gå tilbake i menyene.
void setAlarmOnce(int h, int mi, int d, int mo); //Aktiverer en engangsalarm (once).
void setAlarmDaily(int h, int mi, int d); //Aktiverer den daglige alarmen (daily).
void setAlarmWeekday(int h, int mi, int d); //Aktiverer hverdagsalarmen.
void setAlarmWeekly(int h, int mi, int d); //Aktiverer den ukentlige alarmen.
void ringTheAlarm() { //alarm ringer, kaller på snooze eller skrur av alarm. 
void displayMenuDeleteAlarm(); //Viser "Delete Alarm"-funksjonen til skjerm.
void display_set_time(); //Viser "Set time" og tidspunkt/dato til skjerm.
void display_alarm_onetime();//Viser "Once" til skjerm og tidspunkt/dato til skjerm.
void display_alarm_daily_Weekday(); //Viser "daily" eller "weekday" til skjerm, samt tidspunkt. 
void display_alarm_weekly(); //Viser "set alarm" og tidspunkt/dato til skjerm.
void displayDeleteDaily(); //Viser "Delete alarm daily" til skjerm
void displayDeleteOnce(); //Viser "Delete alarm once" til skjerm
void displayDeleteOnce1(); //Viser "Delete alarm once" og klokkeslettet til alarm #1 til skjerm
void displayDeleteOnce2(); //Viser "Delete alarm once" og klokkeslettet til alarm #2 til skjerm.
void displayDeleteOnce3(); //Viser "Delete alarm once" og klokkeslettet til alarm #3 til skjerm.
void displayDeleteWeekday(); //Viser "Delete alarm Weekday" til skjerm.
void displayDeleteWeekly(); //Viser "Delete alarm weekly" til skjerm.

int joystick(); //Manøvrering av joystick.
int checkSnooze (int mi);//Setter grenseverdier for justering av snoozetiden.
int checkHour(int h); //Funksjon som sørger for at man ikke går over eller under lovlige verdier (Mellom 0 - 23).
int checkSizeMonth(int month1, int day1, int year1); //Sørger for at ikke det blir satt "ulovlig" dag. Basert på måned.
int checkMinute(int mi);//Funksjon som sørger for at man ikke går over eller under lovlige verdier (Mellom 0 - 59).
int checkMonth(int m);//Funksjon som sørger for at man ikke går over eller under lovlige verdier (Mellom 1 - 12).
int digits;//Kontrollvariabel for utskrift av klokkeslett.

boolean activeAlarm(int spot); //sjekker om alarm allerede eksisterer.




byte PIL_RIGHT[8] = {  //symbol for pil høyre
  B01000,
  B01100,
  B01110,
  B01111,
  B01110,
  B01100,
  B01000,
  B00000
};


byte PIL_LEFT[8] = {  //symbol for pil venstre
  B00010,
  B00110,
  B01110,
  B11110,
  B01110,
  B00110,
  B00010,
  B00000
};


byte bell_symbol[8] = { //bjelle symbol
  B00100,
  B01110,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100
};

enum STATES //Menyvalg.
{
  MAIN_SCREEN, //Display av klokke til skjerm.
  MENU_SET_TIMEDATE,//Menyvalg: Stille tid
  MENU_SET_ALARM, //Menyvalg: Sette alarm
  MENU_SET_SNOOZE, //Menyvalg: Justere slumring
  MENU_DELETE_ALARM, //Menyvalg: Slette alarmer

  //menyvalg i MENU_SET_TIMEDATE
  SET_TIME_HOUR,
  SET_TIME_MINUTE,
  SET_TIME_DAY,
  SET_TIME_MONTH,
  SET_TIME_YEAR,
  SET_SNOOZE,

  //menyvalg i MENU_SET_ALARM
  DELETE_ALARM_DAILY,
  DELETE_ALARM_Weekday,
  DELETE_ALARM_WEEKLY,
  DELETE_ALARM_ONETIME,

  //menyvalg i MENU_DELETE_ALARM
  DELETE_ALARM_ONCE1,
  DELETE_ALARM_ONCE2,
  DELETE_ALARM_ONCE3,

  SET_FREQ_DAILY,//sett alarmen til å gå daglig
  SET_DAILY_HOUR,
  SET_DAILY_MINUTE,

  SET_FREQ_Weekday, //sett alarmen til å gå i hverdager
  SET_Weekday_HOUR,
  SET_Weekday_MINUTE,

  SET_FREQ_ONETIME,//sett alarmen til å gå kun en gang.
  SET_ONETIME_HOUR,
  SET_ONETIME_MINUTE,
  SET_ONETIME_DAY,
  SET_ONETIME_MONTH,

  SET_FREQ_WEEKLY, // sett alarm til å gå en gang i uken.
  SET_WEEKLY_HOUR,
  SET_WEEKLY_MINUTE,
  SET_WEEKLY_DAY,


STATES state; //holder programstatus så man vet hvor man er i programmet.
}

void setup() {
  lcd.begin(16, 2); //for å bruke alle linjene og radene på lcd-skjermen.
  lcd.createChar(0, PIL_RIGHT); //lagrer symbol pil høyre
  lcd.createChar(1, PIL_LEFT); //lagrer symbol pil venstre
  lcd.createChar(2, bell_symbol); //bjellesymbol - skal bli aktivert når en alarm er satt.
  pinMode(btnback, INPUT); //tilbakeknapp
  pinMode(btnSnooze, INPUT); //snoozeknapp

  /*for (int p = 0; p < 43; p++) { //sletter hele internminnet før bruk
    EEPROM.write(p, 0);
  }*/

  h = 0, mi = 0, d = 1, mo = 1, yr = 17;
  setTime(h, mi, se, d, mo, yr); //Setter klokken til å være 00:00:00 - 01.01.17 ved oppstart.
  state = MAIN_SCREEN; //Viser dette til hovedskjerm.
}

void loop() {//kaller alle funksjoner som må gå kontinuerlig
  alarmOnceChecker();
  alarmWeekdayChecker();
  alarmWeeklyChecker();
  alarmDailyChecker();
  buttonBack();
  joystick();
  change_state();
  snoozeTimeChecker(snoozeTime);
}

void change_state() { //Kaller på riktig funksjon med tanke på hvilken state klokken er i.
  switch (state) {
    case MAIN_SCREEN:
      printTime();//Skriver ut tiden.
      printDate();//Skriver ut datoen.
      if (activeAlarm(3) == true || activeAlarm(8) == true || activeAlarm(13) == true || activeAlarm(22) == true || activeAlarm(32) == true || activeAlarm(42) == true || snoozeTime != 61) { //Aktiverer bjellesymbolet hvis en alarm er satt.
        lcd.setCursor(14, 0);
        lcd.write(byte(2));
      }
      break;
    case MENU_SET_TIMEDATE:
      displayMenuSetTime();
      break;

    case MENU_SET_ALARM:
      displayMenuSetAlarm();
      break;

    case MENU_SET_SNOOZE:
      displayMenuSetSnooze();
      break;

    case MENU_DELETE_ALARM:
      displayMenuDeleteAlarm();
      break;

    case SET_TIME_HOUR:
      display_set_time();
      lcd.setCursor(1, 1); //Tallet brukeren er på blinker for å vise for hvilken verdi han er på.
      lcd.blink();
      delay(150);
      break;

    case SET_TIME_MINUTE:
      display_set_time();
      lcd.setCursor(4, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_TIME_DAY:
      display_set_time();
      lcd.setCursor(7, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_TIME_MONTH:
      display_set_time();
      lcd.setCursor(10, 1);
      lcd.blink();
      delay(150);
      break;

    case  SET_TIME_YEAR:
      display_set_time();
      lcd.setCursor(15, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_SNOOZE:
      SetSnooze();
      break;

    case SET_FREQ_ONETIME:
      FreqOnce();
      break;

    case SET_ONETIME_HOUR:
      display_alarm_onetime();
      lcd.setCursor(2, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_ONETIME_MINUTE:
      display_alarm_onetime();
      lcd.setCursor(5, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_ONETIME_DAY:
      display_alarm_onetime();
      lcd.setCursor(11, 1);
      lcd.blink();
      delay(150);
      break;
    case SET_ONETIME_MONTH:
      display_alarm_onetime();
      lcd.setCursor(14, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_FREQ_DAILY:
      FreqDaily();
      break;

    case SET_DAILY_HOUR:
      display_alarm_daily_Weekday();
      lcd.setCursor(6, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_DAILY_MINUTE:
      display_alarm_daily_Weekday();
      lcd.setCursor(9, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_FREQ_Weekday:
      FreqWeekday();
      break;

    case SET_Weekday_HOUR:
      display_alarm_daily_Weekday();
      lcd.setCursor(6, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_Weekday_MINUTE:
      display_alarm_daily_Weekday();
      lcd.setCursor(9, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_FREQ_WEEKLY:
      FreqWeekly();
      break;

    case SET_WEEKLY_HOUR:
      display_alarm_weekly();
      lcd.setCursor(2, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_WEEKLY_MINUTE:
      display_alarm_weekly();
      lcd.setCursor(5, 1);
      lcd.blink();
      delay(150);
      break;

    case SET_WEEKLY_DAY:
      display_alarm_weekly();
      lcd.setCursor(14, 1);
      lcd.blink();
      delay(150);
      break;

    case DELETE_ALARM_DAILY:
      displayDeleteDaily();
      break;

    case DELETE_ALARM_WEEKLY:
      displayDeleteWeekly();
      break;

    case DELETE_ALARM_Weekday:
      displayDeleteWeekday();
      break;

    case DELETE_ALARM_ONETIME:
      displayDeleteOnce();
      break;

    case DELETE_ALARM_ONCE1:
      displayDeleteOnce1();
      break;

    case DELETE_ALARM_ONCE2:
      displayDeleteOnce2();
      break;

    case DELETE_ALARM_ONCE3:
      displayDeleteOnce3();
      break;
  }
}

void inbetween(int trigger) //Manøvreringsvalg i menyene. Valg for trigger blir sendt fra joystick() og btnback(); hvor: 0 = down, 1 = up, 2 = left, 3 = right, 4 = push, 5 = btnback.
{
  switch (state)
  {
    case MAIN_SCREEN:
      if (trigger == 4) { //button push
        lcd.clear();
        state = MENU_SET_TIMEDATE;
      }
      delay(200);
      break;

    case MENU_SET_TIMEDATE:
      if (trigger == 2) {
        lcd.clear();
        state = MENU_SET_ALARM;
      } else if (trigger == 3) {
        lcd.clear();
        state = MENU_SET_SNOOZE;
      } else if (trigger == 4) {
        lcd.clear();
        state = SET_TIME_HOUR;
      } else if (trigger == 5) {
        lcd.clear();
        state = MAIN_SCREEN;
      }
      delay(200);
      break;

    case MENU_SET_ALARM:
      if (trigger == 2) {
        lcd.clear();
        state = MENU_DELETE_ALARM;
      } else if (trigger == 3) {
        lcd.clear();
        state = MENU_SET_TIMEDATE;
      } else if (trigger == 4) {
        lcd.clear();
        state = SET_FREQ_ONETIME;
      } else if (trigger == 5) {
        lcd.clear();
        state = MAIN_SCREEN;
      }
      delay(200);
      break;

    case MENU_SET_SNOOZE:
      if (trigger == 2) {
        lcd.clear();
        state = MENU_SET_TIMEDATE;
      } else if (trigger == 3) {
        lcd.clear();
        state = MENU_DELETE_ALARM;
      } else if (trigger == 4) {
        lcd.clear();
        state = SET_SNOOZE;
      } else if (trigger == 5) {
        lcd.clear();
        state = MAIN_SCREEN;
      }
      delay(200);
      break;

    case MENU_DELETE_ALARM:
      if (trigger == 2) {
        lcd.clear();
        state = MENU_SET_SNOOZE;
      } else if (trigger == 3) {
        lcd.clear();
        state = MENU_SET_ALARM;
      } else if (trigger == 4) {
        lcd.clear();
        state = DELETE_ALARM_ONETIME;
      } else if (trigger == 5) {
        lcd.clear();
        state = MAIN_SCREEN;
      }
      delay(200);
      break;

    case DELETE_ALARM_ONETIME:
      if (trigger == 2) {
        lcd.clear();
        state = DELETE_ALARM_Weekday;
      } else if (trigger == 3) {
        lcd.clear();
        state = DELETE_ALARM_DAILY;
      } else if (trigger == 4) {
        lcd.clear();
        if (countAlarm == 0) {
          lcd.setCursor(0, 0);
          lcd.print("No alarms exist");
          delay(2000);
          lcd.clear();
          state = MAIN_SCREEN;
        } else {
          state = DELETE_ALARM_ONCE1;
        }
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_DELETE_ALARM;
      } delay(200);
      break;
      
    case DELETE_ALARM_ONCE1:
      switch (countAlarm) {
        case 0:
          lcd.setCursor(0, 0);
          lcd.print("No alarms exist");
          delay(2000);
          lcd.clear();
          state = MAIN_SCREEN;

        case 1:
          if (trigger == 4) {
            clearAlarmOnce1();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Alarm deleted 1");
            delay(1000);
            lcd.clear();
            state = MAIN_SCREEN;
          } else if (trigger == 5) {
            lcd.clear();
            state = DELETE_ALARM_ONETIME;
          } delay(200);
          break;
          
        case 2:
          if (trigger == 4) {
            clearAlarmOnce1();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Alarm deleted 1");
            delay(1000);
            lcd.clear();
            state = MAIN_SCREEN;
          } else if (trigger == 5) {
            lcd.clear();
            state = DELETE_ALARM_ONETIME;
          } else if (trigger == 2) {
            state = DELETE_ALARM_ONCE2;
          } else if (trigger == 3) {
            state = DELETE_ALARM_ONCE2;
          } delay(200);
          break;
          
        case 3:
          if (trigger == 4) {
            clearAlarmOnce1();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Alarm deleted 1");
            delay(1000);
            lcd.clear();
            state = MAIN_SCREEN;
          } else if (trigger == 5) {
            lcd.clear();
            state = DELETE_ALARM_ONETIME;
          } else if (trigger == 2) {
            state = DELETE_ALARM_ONCE3;
          } else if (trigger == 3) {
            state = DELETE_ALARM_ONCE2;
          } delay(200);
          break;
      }
      break;
      
    case DELETE_ALARM_ONCE2:
      switch (countAlarm) {
        case 2:
          if (trigger == 4) {
            clearAlarmOnce2();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Alarm deleted 2");
            delay(1000);
            lcd.clear();
            state = MAIN_SCREEN;
          } else if (trigger == 5) {
            lcd.clear();
            state = DELETE_ALARM_ONETIME;
          } else if (trigger == 2) {
            state = DELETE_ALARM_ONCE1;
          } else if (trigger == 3) {
            state = DELETE_ALARM_ONCE1;
          } delay(200);
          break;
          
        case 3:
          if (trigger == 4) {
            clearAlarmOnce2();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Alarm deleted 2");
            delay(1000);
            lcd.clear();
            state = MAIN_SCREEN;
          } else if (trigger == 5) {
            lcd.clear();
            state = DELETE_ALARM_ONETIME;
          } else if (trigger == 2) {
            state = DELETE_ALARM_ONCE1;
          } else if (trigger == 3) {
            state = DELETE_ALARM_ONCE3;
          } delay(200);
          break;
      }
      break;
      
    case DELETE_ALARM_ONCE3:
      if (trigger == 4) {
        clearAlarmOnce3();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alarm deleted 3");
        delay(1000);
        lcd.clear();
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = DELETE_ALARM_ONETIME;
      } else if (trigger == 2) {
        state = DELETE_ALARM_ONCE2;
      } else if (trigger == 3) {
        state = DELETE_ALARM_ONCE1;
      } delay(200);
      break;
      
    case DELETE_ALARM_DAILY:
      if (trigger == 2) {
        lcd.clear();
        state = DELETE_ALARM_ONETIME;
      } else if (trigger == 3) {
        lcd.clear();
        state = DELETE_ALARM_WEEKLY;
      } else if (trigger == 4) {
        if (setAlarmDailyVar == 1) {
          clearAlarmDaily();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Alarm deleted");
          delay(1000);
          lcd.clear();
        } else if (setAlarmDailyVar == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("No alarms exist");
          delay(1000);
          lcd.clear();
        }
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_DELETE_ALARM;
      } delay(200);
      break;
      
    case DELETE_ALARM_WEEKLY:
      if (trigger == 2) {
        lcd.clear();
        state = DELETE_ALARM_DAILY;
      } else if (trigger == 3) {
        lcd.clear();
        state = DELETE_ALARM_Weekday;
      } else if (trigger == 4) {
        if (setAlarmWeeklyVar == 1) {
          clearAlarmWeekly();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Alarm deleted");
          delay(1000);
          lcd.clear();
        } else if (setAlarmWeeklyVar == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("No alarms exist");
          delay(1000);
          lcd.clear();
        }
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_DELETE_ALARM;
      } delay(200);
      break;
      
    case DELETE_ALARM_Weekday:
      if (trigger == 2) {
        lcd.clear();
        state = DELETE_ALARM_WEEKLY;
      } else if (trigger == 3) {
        lcd.clear();
        state = DELETE_ALARM_ONETIME;
      } else if (trigger == 4) {
        if (setAlarmWeekdayVar == 1) {
          clearAlarmWeekday();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Alarm deleted");
          delay(1000);
          lcd.clear();
        } else if (setAlarmWeekdayVar == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("No alarms exist");
          delay(2000);
          lcd.clear();
        }
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_DELETE_ALARM;
      } delay(200);
      break;
      
    case SET_TIME_HOUR:
      if (trigger == 4) {
        setTime(h, mi, se, d, mo, yr);
        lcd.clear();
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_TIMEDATE;
      } else if (trigger == 0) {
        h--;
        h = checkHour(h); 
      } else if (trigger == 1) {
        h++;
        h = checkHour(h);
      } else if (trigger == 2) {
        state = SET_TIME_YEAR;
      } else if (trigger == 3) {
        state = SET_TIME_MINUTE;
      } delay(200);
      break;
      
    case SET_TIME_MINUTE:
      if (trigger == 4) {
        setTime(h, mi, se, d, mo, yr);
        lcd.clear();
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_TIMEDATE;
      } else if (trigger == 0) {
        mi--;
        mi = checkMinute(mi);
      } else if (trigger == 1) {
        mi++;
        mi = checkMinute(mi);
      } else if (trigger == 2) {
        state = SET_TIME_HOUR;
      } else if (trigger == 3) {
        state = SET_TIME_DAY;
      } delay(200);
      break;
      
    case SET_TIME_DAY:
      if (trigger == 4) {
        setTime(h, mi, se, d, mo, yr);
        lcd.clear();
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_TIMEDATE;
      } else if (trigger == 0) {
        d--;
        d = checkSizeMonth(mo, d, yr); 
      } else if (trigger == 1) {
        d++;
        d = checkSizeMonth(mo, d, yr);
      } else if (trigger == 2) {
        state = SET_TIME_MINUTE;
      } else if (trigger == 3) {
        state = SET_TIME_MONTH;
      } delay(200);
      break;
      
    case SET_TIME_MONTH:
      if (trigger == 4) {
        setTime(h, mi, se, d, mo, yr);
        lcd.clear();
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_TIMEDATE;
      } else if (trigger == 0) {
        mo--;
        mo = checkMonth(mo); 
      } else if (trigger == 1) {
        mo++;
        mo = checkMonth(mo);
      } else if (trigger == 2) {
        state = SET_TIME_DAY;
      } else if (trigger == 3) {
        state = SET_TIME_YEAR;
      } delay(200);
      break;
      
    case SET_TIME_YEAR:
      if (trigger == 4) {
        setTime(h, mi, se, d, mo, yr);
        lcd.clear();
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_TIMEDATE;
      } else if (trigger == 0) {
        yr--;
      } else if (trigger == 1) {
        yr++;
      } else if (trigger == 2) {
        state = SET_TIME_MONTH;
      } else if (trigger == 3) {
        state = SET_TIME_HOUR;
      } delay(200);
      break;
      
    case SET_SNOOZE:
      if (trigger == 1) {
        snoozeVar++;
        snoozeVar = checkSnooze(snoozeVar);
      } else if (trigger == 0) {
        snoozeVar--;
        snoozeVar = checkSnooze(snoozeVar);
      } else if (trigger == 4) {
        lcd.clear();
        state = MAIN_SCREEN;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_SNOOZE;
      } delay(200);
      break;
      
    case SET_FREQ_ONETIME:
      if (trigger == 2) {
        lcd.clear();
        state = SET_FREQ_WEEKLY;
      } else if (trigger == 3) {
        lcd.clear();
        state = SET_FREQ_DAILY;
      } else if (trigger == 4) {
        lcd.clear();
        state = SET_ONETIME_HOUR;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_ALARM;
      } delay(200);
      break;
      
    case SET_ONETIME_HOUR:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(1, h, mi, d, mo, yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_ONETIME;
      } else if (trigger == 0) {
        h--;
        h = checkHour(h); 
      } else if (trigger == 1) {
        h++;
        h = checkHour(h);
      } else if (trigger == 2) {
        state = SET_ONETIME_MONTH;
      } else if (trigger == 3) {
        state = SET_ONETIME_MINUTE;
      } delay(200);
      break;
      
    case SET_ONETIME_MINUTE:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(1, h, mi, d, mo, yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_ONETIME;
      } else if (trigger == 0) {
        mi--;
        mi = checkMinute(mi); 
      } else if (trigger == 1) {
        mi++;
        mi = checkMinute(mi);
      } else if (trigger == 2) {
        state = SET_ONETIME_HOUR;
      } else if (trigger == 3) {
        state = SET_ONETIME_DAY;
      } delay(200);
      break;
      
    case SET_ONETIME_DAY:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(1, h, mi, d, mo, yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_ONETIME;
      } else if (trigger == 0) {
        d--;
        d = checkSizeMonth(mo, d, yr); 
      } else if (trigger == 1) {
        d++;
        d = checkSizeMonth(mo, d, yr);
      } else if (trigger == 2) {
        state = SET_ONETIME_MINUTE;
      } else if (trigger == 3) {
        state = SET_ONETIME_MONTH;
      } delay(200);
      break;
      
    case SET_ONETIME_MONTH:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(1, h, mi, d, mo, yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_ONETIME;
      } else if (trigger == 0) {
        mo--;
        mo = checkMonth(mo); 
      } else if (trigger == 1) {
        mo++;
        mo = checkMonth(mo);
      } else if (trigger == 2) {
        state = SET_ONETIME_DAY;
      } else if (trigger == 3) {
        state = SET_ONETIME_HOUR;
      } delay(200);
      break;
      
    case SET_FREQ_DAILY:
      if (trigger == 2) {
        lcd.clear();
        state = SET_FREQ_ONETIME;
      } else if (trigger == 3) {
        lcd.clear();
        state = SET_FREQ_Weekday;
      } else if (trigger == 4) {
        lcd.clear();
        state = SET_DAILY_HOUR;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_ALARM;
      } delay(200);
      break;
      
    case SET_DAILY_HOUR:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(2, h, mi, d, month(), yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_DAILY;
      } else if (trigger == 0) {
        h--;
        h = checkHour(h); 
      } else if (trigger == 1) {
        h++;
        h = checkHour(h);
      } else if (trigger == 2) {
        state = SET_DAILY_MINUTE;
      } else if (trigger == 3) {
        state = SET_DAILY_MINUTE;
      } delay(200);
      break;
      
    case SET_DAILY_MINUTE:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(2, h, mi, d, month(), yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_DAILY;
      } else if (trigger == 0) {
        mi--;
        mi = checkMinute(mi); 
      } else if (trigger == 1) {
        mi++;
        mi = checkMinute(mi);
      } else if (trigger == 2) {
        state = SET_DAILY_HOUR;
      } else if (trigger == 3) {
        state = SET_DAILY_HOUR;
      } delay(200);
      break;
      
    case SET_FREQ_Weekday:
      if (trigger == 2) {
        lcd.clear();
        state = SET_FREQ_DAILY;
      } else if (trigger == 3) {
        lcd.clear();
        state = SET_FREQ_WEEKLY;
      } else if (trigger == 4) {
        lcd.clear();
        state = SET_Weekday_HOUR;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_ALARM;
      } delay(200);
      break;
      
    case SET_Weekday_HOUR:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(3, h, mi, d, mo, yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_Weekday;
      } else if (trigger == 0) {
        h--;
        h = checkHour(h); 
      } else if (trigger == 1) {
        h++;
        h = checkHour(h);
      } else if (trigger == 2) {
        state = SET_Weekday_MINUTE;
      } else if (trigger == 3) {
        state = SET_Weekday_MINUTE;
      } delay(200);
      break;
      
    case SET_Weekday_MINUTE:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(3, h, mi, d, mo, yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_Weekday;
      } else if (trigger == 0) {
        mi--;
        mi = checkMinute(mi); 
      } else if (trigger == 1) {
        mi++;
        mi = checkMinute(mi);
      } else if (trigger == 2) {
        state = SET_Weekday_HOUR;
      } else if (trigger == 3) {
        state = SET_Weekday_HOUR;
      } delay(200);
      break;
      
    case SET_FREQ_WEEKLY:
      if (trigger == 2) {
        lcd.clear();
        state = SET_FREQ_Weekday;
      } else if (trigger == 3) {
        lcd.clear();
        state = SET_FREQ_ONETIME;
      } else if (trigger == 4) {
        lcd.clear();
        state = SET_WEEKLY_HOUR;
      } else if (trigger == 5) {
        lcd.clear();
        state = MENU_SET_ALARM;
      } delay(200);
      break;
      
    case SET_WEEKLY_HOUR:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(4, h, mi, d, mo, yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_WEEKLY;
      } else if (trigger == 0) {
        h--;
        h = checkHour(h); 
      } else if (trigger == 1) {
        h++;
        h = checkHour(h);
      } else if (trigger == 2) {
        state = SET_WEEKLY_DAY;
      } else if (trigger == 3) {
        state = SET_WEEKLY_MINUTE;
      } delay(200);
      break;
      
    case SET_WEEKLY_MINUTE:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(4, h, mi, d, mo, yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_WEEKLY;
      } else if (trigger == 0) {
        mi--;
        mi = checkMinute(mi); 
      } else if (trigger == 1) {
        mi++;
        mi = checkMinute(mi);
      } else if (trigger == 2) {
        state = SET_WEEKLY_HOUR;
      } else if (trigger == 3) {
        state = SET_WEEKLY_DAY;
      } delay(200);
      break;
      
    case SET_WEEKLY_DAY:
      if (trigger == 4) {
        lcd.clear();
        checkAlarm(4, h, mi, d, mo, yr);
      } else if (trigger == 5) {
        lcd.clear();
        state = SET_FREQ_WEEKLY;
      } else if (trigger == 0) {
        d--;
        d = checkSizeMonth(mo, d, yr); 
      } else if (trigger == 1) {
        d++;
        d = checkSizeMonth(mo, d, yr);
      } else if (trigger == 2) {
        state = SET_WEEKLY_MINUTE;
      } else if (trigger == 3) {
        state = SET_WEEKLY_HOUR;
      } delay(200);
      break;
  }
}

void printTime() { //Skriver ut tiden.
  lcd.setCursor(4, 0);
  printCorrect(hour());//henter funskjon som putter "0" forran timer, minutter og sekunder hvis timer er mellom 0-9
  lcd.print(":");
  printCorrect(minute());
  lcd.print(":");
  printCorrect(second());
  }

void printDate() { //Skriver ut datoen.
  lcd.setCursor(3, 1);
  printCorrect(day());//henter funskjon som putter "0" forran dager, måneder hvis de er mellom 0-9.
  lcd.print("/");
  printCorrect(month());
  lcd.print("/");
  lcd.print(year());
}

void printCorrect(int digits){ //Passer på at tallene blir skrevet ut i riktig format.
  if ( digits < 10)
    lcd.print('0');
    lcd.print(digits);
 }

void display_set_time(){ //Viser "Set time" og tidspunkt/dato til skjerm.
  lcd.setCursor(3, 0);
  lcd.print("Set time");
  lcd.setCursor(0, 1);
  printCorrect(h);
  lcd.print(":");
  printCorrect(mi);
  lcd.print(" ");
  printCorrect(d);
  lcd.print("/");
  printCorrect(mo);
  lcd.print("/");
  lcd.print(yr + 2000);
}

void display_alarm_weekly(){ //Viser "set alarm" og tidspunkt/dato til skjerm.
  lcd.setCursor(4, 0);
  lcd.print("Set alarm");
  lcd.setCursor(1, 1);
  printCorrect(h);
  lcd.print(":");
  printCorrect(mi);
  lcd.print("   day:");
  printCorrect(d);
}

void display_alarm_daily_Weekday(){ //Viser "daily" eller "weekday" til skjerm, samt tidspunkt. 
  lcd.setCursor(3, 0);
  lcd.print("Set alarm");
  lcd.setCursor(5, 1);
  printCorrect(h);
  lcd.print(":");
  printCorrect(mi);
}

void display_alarm_onetime(){ //Viser "Once" til skjerm og tidspunkt/dato til skjerm.
   lcd.setCursor(3, 0);
   lcd.print("Set alarm");
   lcd.setCursor(1, 1);
   printCorrect(h);
   lcd.print(":");
   printCorrect(mi);
   lcd.print("    ");
   printCorrect(d);
   lcd.print("/");
   printCorrect(mo);
 }
 
void displayDeleteOnce(){ ////Viser "Delete alarm once" til skjerm.
  lcd.setCursor(2, 0);
  lcd.print("Delete alarm");
  lcd.setCursor(6, 1);
  lcd.print("Once");
  MenyPil();
}

void displayDeleteOnce1(){ //Viser "Delete alarm once" og klokkeslettet til alarm #1 til skjerm
  lcd.setCursor(1, 0);
  lcd.print("Delete Alarm 1");
  lcd.setCursor(0, 1);
  printCorrect(EEPROM.read(0));//hour
  lcd.print(":");
  printCorrect(EEPROM.read(1));//minute
  lcd.print("    ");
  printCorrect(EEPROM.read(2));//day
  lcd.print("/");
  printCorrect(EEPROM.read(3));//month
  menyPilTopp();
}

void displayDeleteOnce2(){ //Viser "Delete alarm once" og klokkeslettet til alarm #2 til skjerm.
  lcd.setCursor(1, 0);
  lcd.print("Delete Alarm 2");
  lcd.setCursor(0, 1);
  printCorrect(EEPROM.read(5));//hour
  lcd.print(":");
  printCorrect(EEPROM.read(6));//minute
  lcd.print("    ");
  printCorrect(EEPROM.read(7));//day
  lcd.print("/");
  printCorrect(EEPROM.read(8));//month
  menyPilTopp();
}

void displayDeleteOnce3(){ //Viser "Delete alarm once" og klokkeslettet til alarm #3 til skjerm.
  lcd.setCursor(1, 0);
  lcd.print("Delete Alarm 3");
  lcd.setCursor(0, 1);
  printCorrect(EEPROM.read(10));//hour
  lcd.print(":");
  printCorrect(EEPROM.read(11));//minute
  lcd.print("    ");
  printCorrect(EEPROM.read(12));//day
  lcd.print("/");
  printCorrect(EEPROM.read(13));//month
  menyPilTopp();
}

void displayDeleteWeekly(){ //Viser "Delete alarm weekly" til skjerm.
  lcd.setCursor(2, 0);
  lcd.print("Delete alarm");
  lcd.setCursor(5, 1);
  lcd.print("Weekly");
  MenyPil();
}

void displayDeleteDaily(){ //Viser "Delete alarm daily" til skjerm.
  lcd.setCursor(2, 0);
  lcd.print("Delete alarm");
  lcd.setCursor(5, 1);
  lcd.print("Daily");
  MenyPil();
}

void displayDeleteWeekday(){ //Viser "Delete alarm Weekday" til skjerm.
  lcd.setCursor(2, 0);
  lcd.print("Delete alarm");
  lcd.setCursor(4, 1);
  lcd.print("Weekday");
  MenyPil();
}

void displayMenuSetTime(){ //Viser menyvalg "set time" til skjerm.
  lcd.setCursor(6, 0);
  lcd.print("Menu");
  lcd.setCursor(4, 1);
  lcd.print("Set Time");
  MenyPil();
}

void displayMenuSetAlarm(){ //Viser menyvalg "set alarm" til skjerm.
  lcd.setCursor(6, 0);
  lcd.print("Menu");
  lcd.setCursor(4, 1);
  lcd.print("Set Alarm");
  MenyPil();
}

void displayMenuSetSnooze(){//Viser menyvalg "Set snooze" til skjerm. 
  lcd.setCursor(6, 0);
  lcd.print("Menu");
  lcd.setCursor(3, 1);
  lcd.print("Set Snooze");
  MenyPil();
}

void displayMenuDeleteAlarm(){ //Viser menyvalg "Delete Alarm" til skjerm.
  lcd.setCursor(6, 0);
  lcd.print("Menu");
  lcd.setCursor(2, 1);
  lcd.print("Delete Alarm");
  MenyPil();
}

void SetSnooze(){ //Viser instillingsvalg av snoozetid til skjerm.
  lcd.setCursor(3, 0);
  lcd.print("Set Snooze:");
  lcd.setCursor(5, 1);
  printCorrect(snoozeVar);
  lcd.setCursor(8, 1);
  lcd.print("min");
}

void FreqOnce(){ //Viser alarmtype "Once" til skjerm.
  lcd.setCursor(2, 0);
  lcd.print("Alarm Types:");
  lcd.setCursor(6, 1);
  lcd.print("Once");
  MenyPil();
}

void FreqWeekly(){ //Viser alarmtype "Weekly" til skjerm. 
  lcd.setCursor(2, 0);
  lcd.print("Alarm Types:");
  lcd.setCursor(5, 1);
  lcd.print("Weekly");
  MenyPil();
}

void FreqDaily(){ //Viser alarmtype "Daily" til skjerm.
  lcd.setCursor(2, 0);
  lcd.print("Alarm Types:");
  lcd.setCursor(6, 1);
  lcd.print("Daily");
  MenyPil();
}

void FreqWeekday(){ //Viser alarmtype "Weekday" til skjerm.
  lcd.setCursor(2, 0);
  lcd.print("Alarm Types:");
  lcd.setCursor(4, 1);
  lcd.print("Weekday");
  MenyPil();
}

void MenyPil(){ //Viser pil til høyre og venstre til skjerm.
  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
}

void menyPilTopp(){ //Viser piler på øverste del av skjerm under sletting av engangsalarmer.
  lcd.setCursor(15, 0);
  lcd.write(byte(0));
  lcd.setCursor(0, 0);
  lcd.write(byte(1));
}

void buttonBack(){ //Navigeringsfunksjon som gjør at man kan gå tilbake i menyene.
  int joy = 6;
  if (digitalRead(btnback) == HIGH){
      delay(45);
      joy = 5;
      inbetween(joy);
  }
}

int joystick() { //Manøvrering av joystick.
  int x, y, z;
  int joy = 6;
  x = analogRead(JoyStick_X); //Analog port 0.
  y = analogRead(JoyStick_Y); //Analog port 1.
  z = digitalRead(JoyStick_Z); //Digital port 8.
  
  if (y > 255 & y < 767) { //Opp og ned
    if (x >= 0 & x <= 255) { //Ned.
      delay(75);
      joy = 0;
      inbetween(joy);
    }
    if (x >= 767 & x <= 1023) { //Opp.
      delay(75);
      joy = 1;
      inbetween(joy);
    }
  }
  if (x >= 255 & x <= 767) { //Venstre og høyre
    if (y >= 0 & y <= 255) { //Venstre
      delay(45);
      joy = 2;
      inbetween(joy);
    }
    if (y >= 767 & y <= 1023) { //Høyre
      delay(45);
      joy = 3;
      inbetween(joy);
    }
  }
  if (z == 1) { //Push/enter
    delay(45);
    joy = 4;
    inbetween(joy);
  }
}

int checkMonth(int m) { //Funksjon som sørger for at man ikke går over eller under lovlige verdier (Mellom 1 - 12).

  if (m == 13) { //Viss måneden går over 12 så returneres 1.
    return 1;
  }
  if (m == 0) { //Viss måneden går under 1 så returneres 12.
    return 12;
  } else {
    return m; //Gyldig verdi og returnerer det som er skrevet inn.
  }
}

int checkMinute (int mi) { //Funksjon som sørger for at man ikke går over eller under lovlige verdier (Mellom 0 - 59).

  if (mi == -1) { //Viss minutter går under 0 så returneres 59.
    return 59;
  } if (mi == 60) { // Viss minutter går over 59 så returneres 0.
    return 0;
  } else {
    return mi; //Gyldig verdi og returnerer det som er skrevet inn.
  }
}

int checkHour(int h) { //Funksjon som sørger for at man ikke går over eller under lovlige verdier (Mellom 0 - 23).

  if (h == -1) { //Viss timer går under 0 så returneres 23.
    return 23;
  } if (h == 24) { //Viss timer går over 23 så returneres 0.
    return 0;
  } else {
    return h; //Gyldig verdi og returnerer det som er skrevet inn.
  }
}

int checkSnooze (int mi) { //Setter grenseverdier for justering av snoozetiden.
  if (mi == 0) {
    return 59;
  } if (mi == 60) {
    return 1;
  } else {
    return mi;
  }
}

int checkSizeMonth(int month1, int day1, int year1) { //Sørger for at ikke det blir satt "ulovlig" dag. Basert på måned.
  if (month1 == 1 || month1 == 3 || month1 == 5 || month1 == 7 || month1 == 8 || month1 == 10 || month1 == 12) { //måneder det er 31 dager.
    if (day1 >= 32) {
      return 1;
    }
    if (day1 == 0) {
      return 31;
    } else {
      return day1;
    }
  }
  if (month1 == 4 || month1 == 6 || month1 == 9 || month1 == 11) { //måneder det er 30 dager.
    if (day1 >= 31) {
      return 1;
    }
    if (day1 == 0) {
      return 30;
    } else {
      return day1;
    }
  }
  if (month1 == 2) { //Sjekker om det er skuddår.
    if ((year1 % 4 == 0) && (year1 % 100 != 0) || (year1 % 400 == 0)) {
      if (day1 == 0) {
        return 29;
      }
      if (day1 >= 30) {
        return 1;
      } else {
        return day1;
      }
    } else {
      if (day1 == 0) {
        return 28;
      }
      if (day1 >= 29) {
        return 1;
      } else {
        return day1;
      }
    }
  }
}
void checkAlarm(int type, int h, int mi, int d, int mo, int yr) { //Sjekker om en alarm kan lagres eller ikke og viser til skjerm.
  if (type == 1) { //engangsalarm
    setAlarmOnce(h, mi, d, mo);
    if (setAlarmOnceVar == 1) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Alarm capacity");
      lcd.setCursor(4, 1);
      lcd.print("reached");
      delay(2000);
      lcd.clear();
    } else if (setAlarmOnceVar == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alarm activated");
      delay(1000);
      lcd.clear();
    }
    state = MAIN_SCREEN;
  }
  if (type == 2) { //daglig alarm
    setAlarmDaily(h, mi, day());
    if (setAlarmDailyVar == 0) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Alarm capacity");
      lcd.setCursor(4, 1);
      lcd.print("reached");
      delay(2000);
      lcd.clear();
    } else if (setAlarmDailyVar == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alarm activated");
      delay(1000);
      lcd.clear();
    }
    state = MAIN_SCREEN;
  }
  if (type == 3) { //alarm ukedag
    setAlarmWeekday(h, mi, day());
    if (setAlarmWeekdayVar == 0) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Alarm capacity");
      lcd.setCursor(4, 1);
      lcd.print("reached");
      delay(2000);
      lcd.clear();
    } else if (setAlarmWeekdayVar == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alarm activated");
      delay(1000);
      lcd.clear();
    }
    state = MAIN_SCREEN;
  }
  if (type == 4) { //alarm ukentlig
    setAlarmWeekly(h, mi, d);
    if (setAlarmWeeklyVar == 0) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Alarm capacity");
      lcd.setCursor(4, 1);
      lcd.print("reached");
      delay(2000);
      lcd.clear();
    } else if (setAlarmWeeklyVar == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alarm activated");
      delay(1000);
      lcd.clear();
    }
    state = MAIN_SCREEN;
  }
}


boolean activeAlarm(int spot) { //sjekker om alarm allerede eksisterer.
  if (EEPROM.read(spot) > 0) {
    return true;
  } else {
    return false;
  }
}

void alarmWeeklyChecker() { //må legges i loop, da denne sjekker om alarmWeekly skal "gå av"
  if (EEPROM.read(40) == hour() && EEPROM.read(41) == minute() && EEPROM.read(42) == day() && setAlarmWeeklyVar == 1 && second() == 0) {
    ringTheAlarm();
    for (int x = 0; x < 7; x++) { //Plusser på en dag om gangen slik at checkSizeMonth() fungerer. Dette sørger for at dato ikke blir satt til ugyldig verdi. 
      int changeDay = EEPROM.read(42);   
      changeDay ++;
      EEPROM.write(42, (checkSizeMonth(month(), changeDay, year())));
    }

  }
}

void alarmWeekdayChecker() { //Sjekker om ukedagsalarmen skal gå av.
  if (EEPROM.read(30) == hour() && EEPROM.read(31) == minute() && weekday() < 7 && weekday() > 1 && setAlarmWeekdayVar == 1 && second() == 0) {
    ringTheAlarm();
  }
}

void alarmDailyChecker() { //Sjekker om den daglige alarmen skal gå av.
  if (EEPROM.read(20) == hour() && EEPROM.read(21) == minute() && setAlarmDailyVar == 1 && second() == 0) {
    ringTheAlarm();
  }
}

void alarmOnceChecker() { //Sjekker om engangsalarmene skal gå av.
  if (EEPROM.read(0) == hour() && EEPROM.read(1) == minute() && EEPROM.read(2) == day() && EEPROM.read(3) == month() && second() == 0) { //sjekker alarm 1
    ringTheAlarm();
    clearAlarmOnce1();
  }

  if (EEPROM.read(5) == hour() && EEPROM.read(6) == minute() && EEPROM.read(7) == day() && EEPROM.read(8) == month() && second() == 0) { //sjekker alarm 2
    ringTheAlarm();
    clearAlarmOnce2();
  }

  if (EEPROM.read(10) == hour() && EEPROM.read(11) == minute() && EEPROM.read(12) == day() && EEPROM.read(13) == month() && second() == 0) { //sjekker alarm 3
    ringTheAlarm();
    clearAlarmOnce3();
  }
}

void clearAlarmWeekly() { //Sletter alarmen som går av ukentlig.
  EEPROM.write(40, 0);
  EEPROM.write(41, 0);
  EEPROM.write(42, 0);
  setAlarmWeeklyVar = 0;
}

void clearAlarmWeekday() { //Sletter alarmen som går av på hverdager.
  EEPROM.write(30, 0);
  EEPROM.write(31, 0);
  EEPROM.write(32, 0);
  setAlarmWeekdayVar = 0;
}

void clearAlarmDaily() { //Sletter alarmen som går av daglig.
  EEPROM.write(20, 0);
  EEPROM.write(21, 0);
  EEPROM.write(22, 0);
  setAlarmDailyVar = 0;
}

void setAlarmOnce(int h, int mi, int d, int mo) { //Aktiverer en engangsalarm (once).

  countAlarm++; //antall engangsalarmer økes
  if (countAlarm == 1) {
    EEPROM.write(0, h);
    EEPROM.write(1, mi);
    EEPROM.write(2, d);
    EEPROM.write(3, mo);
  }
  if (countAlarm == 2) {
    EEPROM.write(5, h);
    EEPROM.write(6, mi);
    EEPROM.write(7, d);
    EEPROM.write(8, mo);
  }
  if (countAlarm == 3) { 
    EEPROM.write(10, h);
    EEPROM.write(11, mi);
    EEPROM.write(12, d);
    EEPROM.write(13, mo);
  }
  if (countAlarm > 3) { //Maks er tre alarmer. 
    setAlarmOnceVar = 1;
    countAlarm--;
  } else {
    setAlarmOnceVar = 0;
  }
}

void setAlarmDaily(int h, int mi, int d) { //Aktiverer den daglige alarmen (daily).
  if (activeAlarm(22) == false) { //sjekker om det allerede er satt alarm. Kun mulig å sette en alarm ukentlig.
    EEPROM.write(20, h);
    EEPROM.write(21, mi);
    EEPROM.write(22, d);
    setAlarmDailyVar = 1;
  } else {
    setAlarmDailyVar = 0;
  }
}

void setAlarmWeekday(int h, int mi, int d) { //Aktiverer hverdagsalarmen.
  if (activeAlarm(32) == false) { //sjekker om det allerede er satt alarm. Kun mulig å sette en alarm ukentlig.
    EEPROM.write(30, h); 
    EEPROM.write(31, mi);
    EEPROM.write(32, d);
    setAlarmWeekdayVar = 1;
  } else {
    setAlarmWeekdayVar = 0;
  }
}

void setAlarmWeekly(int h, int mi, int d) { //Aktiverer den ukentlige alarmen.
  if (activeAlarm(42) == false) { //sjekker om det allerede er satt alarm. Kun mulig å sette en alarm ukentlig.
    EEPROM.write(40, h);
    EEPROM.write(41, mi);
    EEPROM.write(42, d);
    setAlarmWeeklyVar = 1;
  } else {
    setAlarmWeeklyVar = 0;
  }
}
void snooze() { //utsetter alarm x-antall min. Istede for å sette klokken på nytt, velger vi derfor å regner ut (minutt klokke går av) + X min
  snoozeTime = 0;
  snoozeTime = snoozeVar + minute();
  if (snoozeTime > 59) {
    snoozeTime = snoozeTime - 59;
  }
}

void ringTheAlarm() { //alarm ringer, kaller på snooze eller skrur av alarmen. 
  int inn;
  unsigned long stopp = 0;
  unsigned long diff = 0;
  unsigned long start = 0;
  while (diff == 0) {
    alarmSound();
    inn = digitalRead(btnSnooze);
    if (inn == 1) {
      start = millis();
      while (inn == 1) {
        inn = digitalRead(btnSnooze);
      }
      stopp = millis();
      diff = stopp - start;
    }
    if (diff < 2000 && diff > 0) { //Snooze-funksjonen blir aktivert.
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Snooze");
      lcd.setCursor(3, 1);
      lcd.print("activated");
      delay(1000);
      snooze();
    }
    if (diff >= 2000) { //Alarmen blir deaktivert. 
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Alarm");
      lcd.setCursor(2, 2);
      lcd.print("deactivated");
      delay(1000);
      snoozeTime = 61;
    }
  }
  delay(500);
  lcd.clear();
}

void clearAlarmOnce1() { //Sletting av engangsalarmen i posisjon 1.
  if (countAlarm == 1) {
    for (int x = 0; x < 4; x++) { //Sletter alarm 1. 
      EEPROM.write(x, 0);
    }
    countAlarm = 0;
  }

  if (countAlarm == 2) { //Flytter alarm 2 til 1.
    EEPROM.write(0, (EEPROM.read(5)));
    EEPROM.write(1, (EEPROM.read(6)));
    EEPROM.write(2, (EEPROM.read(7)));
    EEPROM.write(3, (EEPROM.read(8)));

    for (int x = 5; x < 9; x++) { //Sletter alarm 2.
      EEPROM.write(x, 0);
    }
    countAlarm--;
  }
  if (countAlarm == 3) {
    EEPROM.write(0, (EEPROM.read(10))); //Flytter alarm 3 til 1.
    EEPROM.write(1, (EEPROM.read(11)));
    EEPROM.write(2, (EEPROM.read(12)));
    EEPROM.write(3, (EEPROM.read(13)));

    for (int x = 10; x < 14; x++) { //Sletter alarm 3.
      EEPROM.write(x, 0);
    }
    countAlarm--;
  }

}
void clearAlarmOnce2() { //Sletting av engangsalarmen i posisjon 2.

  for (int x = 5; x < 9; x++) {
    EEPROM.write(x, 0);
  }
  EEPROM.write(5, (EEPROM.read(10))); //Flytter alarm 3 til 2.
  EEPROM.write(6, (EEPROM.read(11)));
  EEPROM.write(7, (EEPROM.read(12)));
  for (int x = 10; x < 14; x++) { //Sletter alarm 3.
    EEPROM.write(x, 0);
  }
  countAlarm--;
}
void clearAlarmOnce3() { //Sletting av engangsalarmen i posisjon 3.
  for (int x = 10; x < 14; x++) { //Sletter alarm 3.
    EEPROM.write(x, 0);
  }
  countAlarm--;
}
void snoozeTimeChecker(int snoozeTime) { //Sjekker når alarmen skal gå igjen etter slumring.
  if (minute() == snoozeTime) {
    snoozeTime = 0;
    ringTheAlarm();
  }
}

void alarmSound() { //Alarmlyd.
  int inn = digitalRead(btnSnooze);
  while (inn == 0) {
    tone(12, 2000, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1700, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1500, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1340, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1500, 500);
    displayalarm();
    inn = digitalRead(btnSnooze);
    delay(500);
    tone(12, 1700, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    };
    delay(500);
    tone(12, 2000, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1700, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1500, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1340, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1500, 250);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(250);
    tone(12, 1700, 250);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(250);
    tone(12, 1500, 250);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(250);
    tone(12, 1700, 250);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(250);
    tone(12, 2000, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1700, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 2000, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 2240, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1700, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 2240, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 2000, 500);
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1700, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1500, 500);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(500);
    tone(12, 1340, 1000);
    displayalarm();
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
    delay(2000);
    if (inn = digitalRead(btnSnooze) == 1) {
      break;
    }
  }
}

void displayalarm() { //Viser at alarmen går på skjerm.
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("Alarm");
  lcd.setCursor(4, 0);
  lcd.write(byte(2));
  lcd.setCursor(12, 0);
  lcd.write(byte(2));
}
