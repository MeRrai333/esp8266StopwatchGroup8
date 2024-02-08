#include <Arduino.h>
#include "Wire.h"
#include <LiquidCrystal_I2C.h>

#define TIMERSTOPSTATE 0
#define TIMERSTATE 1
#define COUNTDOWNSETSTATE 2
#define COUNTDOWNSTOPSTATE 3
#define COUNTDOWNSTARTSTATE 4
#define COUNTDOWNFINISHSTATE 5
#define SETRELAYSTATE 6
#define RESETSTATE 7
 
/* ---- IO Pin ---- */
#define STARTBTN D5
#define RESETBTN D6
#define LED D7
#define RY D8
/* ---- IO Pin ---- */
 
int state, returnState;
unsigned long millisTime = 0, millisTemp = 0, millisDif = 0;
 
unsigned long timer = 0, countDown = 0;
int timermSec = 0,timerSec = 0, timerMin = 0, timerHour = 0;
int countmSec = 0, countSec = 0, countMin = 0;
int relayMode = 0;
String timerText = "00:00:00.00", countDownText = "00:00.00";
 
LiquidCrystal_I2C lcd(0x27,16,2);
 
void setCountDown();
void setCountDownVar(int num, int step);
 
 
void setup() {
    state = TIMERSTOPSTATE;
    returnState = state;
    pinMode(STARTBTN, INPUT_PULLUP);
    pinMode(RESETBTN, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
    pinMode(RY, OUTPUT);
    Serial.begin(115200);
    lcd.begin();
    lcd.backlight();
    Serial.println("Begin");
}
 
void loop(){
  if(state == TIMERSTOPSTATE){
    lcd.setCursor(0,0);
    lcd.print("Stopwatch");
    lcd.setCursor(0,1);
    lcd.print(timerText);
    digitalWrite(LED, 0);
    if(!digitalRead(STARTBTN)){
      while(!digitalRead(STARTBTN))
        delay(50);
      state = TIMERSTATE;
      millisTime = millis();
    }
    if(!digitalRead(RESETBTN)){
      unsigned long millisBTN = millis();
      while(!digitalRead(RESETBTN)){
        if(millis() - millisBTN > 2000){
          digitalWrite(LED, 1);
        }
        delay(50);
      }
      digitalWrite(LED, 0);
      if(millis() - millisBTN < 2000)
        state = RESETSTATE;
      else{
        state = COUNTDOWNSETSTATE;
        returnState = state;
      }
    }
  }
  else if(state == TIMERSTATE){
    millisDif = millis()-millisTime;
    timer = (millisTemp+millisDif)/1000;
    timermSec = ((millisTemp+millisDif)%1000)/10;
    timerSec = timer%60;
    timerMin = timer/60;
    timerHour = timerMin/60;
    timerMin = timerMin%60;
    timerText = String(timerHour/10)+String(timerHour%10)+":";
    timerText += String(timerMin/10)+String(timerMin%10)+":";
    timerText += String(timerSec/10)+String(timerSec%10)+".";
    timerText += String(timermSec/10)+String(timermSec%10);
    digitalWrite(LED, 1);
    lcd.setCursor(0,1);
    lcd.print(timerText);
    if(!digitalRead(STARTBTN)){
      digitalWrite(LED, 0);
      while(!digitalRead(STARTBTN))
        delay(50);
      millisTemp += millisDif;
      state = TIMERSTOPSTATE;
    }
  }
  else if(state == COUNTDOWNSETSTATE){
    setCountDown();
  }
  else if(state == COUNTDOWNSTOPSTATE){
    lcd.setCursor(0,0);
    lcd.print("Countdown");
    if(relayMode != 0)
    lcd.print(" | RY");
    lcd.setCursor(0,1);
    lcd.print(countDownText);
    if(!digitalRead(STARTBTN)){
      while(!digitalRead(STARTBTN))
        delay(50);
      state = COUNTDOWNSTARTSTATE;
      millisTime = millis();
    }
    if(!digitalRead(RESETBTN)){
      unsigned long millisBTN = millis();
      while(!digitalRead(RESETBTN)){
        if(millis() - millisBTN > 4000)
          digitalWrite(LED, 0);
        else if(millis() - millisBTN > 2000)
          digitalWrite(LED, 1);
        delay(50);
      }
      if(millis() - millisBTN < 2000)
        state = RESETSTATE;
      else if(millis() - millisBTN < 4000){
        state = RESETSTATE;
        returnState = TIMERSTOPSTATE;
      }
      else{
        state = SETRELAYSTATE;
      }
    }
  }
  else if(state == COUNTDOWNSTARTSTATE){
    digitalWrite(LED, 1);
    digitalWrite(RY, (relayMode == 1) ? 1 : 0);
    millisDif = millis() - millisTime;
    unsigned long countDownDif = countDown - millisDif;
    countmSec = (countDownDif%1000)/10;
    countSec = countDownDif/1000;
    countMin = countSec/60;
    countSec = countSec%60;
    if(countDownDif > 4200000000){
      digitalWrite(RY, (relayMode == 2) ? 1 : 0);
      state = COUNTDOWNFINISHSTATE;
      countDown = 0;
      countSec = 0;
      countmSec = 0;
      countMin = 0;
      millisTime = millis();
    }
    countDownText = String(countMin/10)+String(countMin%10)+":";
    countDownText += String(countSec/10)+String(countSec%10)+".";
    countDownText += String(countmSec/10)+String(countmSec%10);
    lcd.setCursor(0,0);
    lcd.print("Countdown");
    lcd.setCursor(0,1);
    lcd.print(countDownText);
    if(!digitalRead(STARTBTN)){
      digitalWrite(LED, 0);
      digitalWrite(RY, 0);
      while(!digitalRead(STARTBTN))
        delay(50);
      countDown = countDownDif;
      state = COUNTDOWNSTOPSTATE;
    }
  }
  else if(state == COUNTDOWNFINISHSTATE){
    if(millis() - millisTime > 800)
      millisTime = millis();
    digitalWrite(LED, (millis()-millisTime > 400));
    if(!digitalRead(RESETBTN)){
      digitalWrite(LED, 0);
      unsigned long millisBTN = millis();
      while(!digitalRead(RESETBTN)){
        if(millis() - millisBTN > 2000)
          digitalWrite(LED, 1);
        delay(50);
      }
      if(millis() - millisBTN < 2000)
        state = RESETSTATE;
      else{
        state = RESETSTATE;
        returnState = TIMERSTOPSTATE;
      }
    }
  }
  else if(state == SETRELAYSTATE){
    lcd.clear();
    unsigned long millisBlinkText = millis();
    while(1){
      yield();
      String relayText = (relayMode == 0) ? ("OFF") : (relayMode == 1) ? ("ON->OFF") : ("OFF->ON");
      lcd.setCursor(0,0);
      lcd.print("Relay sett.");
      lcd.setCursor(0,1);
      lcd.print((millis() - millisBlinkText < 750) ? relayText : "       ");
      if(millis() - millisBlinkText > 1000)
        millisBlinkText = millis();
      if(!digitalRead(STARTBTN)){
        while(!digitalRead(STARTBTN)){
          lcd.setCursor(0,0);
          lcd.print("Relay sett.");
          lcd.setCursor(0,1);
          lcd.print((millis() - millisBlinkText < 750) ? relayText : "       ");
          if(millis() - millisBlinkText > 1000)
            millisBlinkText = millis();
          yield();
        }
        relayMode = (relayMode+1 > 2) ? 0 : relayMode+1;
      }
      if(!digitalRead(RESETBTN)){
        while(!digitalRead(RESETBTN)){
          lcd.setCursor(0,0);
          lcd.print("Relay sett.");
          lcd.setCursor(0,1);
          lcd.print((millis() - millisBlinkText < 750) ? relayText : "       ");
          if(millis() - millisBlinkText > 1000)
            millisBlinkText = millis();
          yield();
        }
        state = COUNTDOWNSTOPSTATE;
        lcd.clear();
        break;
      }
    }
  }
  else if(state == RESETSTATE){
    timer = 0;
    timermSec = 0;
    timerSec = 0;
    timerMin = 0;
    timerText = "00:00:00.00";
    countDown = 0;
    countMin = 0;
    countSec = 0;
    countmSec = 0;
    countDownText = "00:00.00";
    relayMode = 0;
    millisTemp = 0;
    digitalWrite(RY, 0);
    state = returnState;
    lcd.clear();
  }
}
 
void setCountDown(){
  int step = 0;
  unsigned long millisBlinkText = millis();
  lcd.clear();
  while(state == COUNTDOWNSETSTATE){
    yield();
    countDownText = (step == 0) ? (millis() - millisBlinkText < 750) ? String(countMin/10)+String(countMin%10)+":" : "  :" : String(countMin/10)+String(countMin%10)+":";
    countDownText += (step == 1) ? (millis() - millisBlinkText < 750) ? String(countSec/10)+String(countSec%10)+".00" : "  .00" : String(countSec/10)+String(countSec%10)+".00";
    if(millis() - millisBlinkText > 1000)
      millisBlinkText = millis();

    lcd.setCursor(0,0);
    lcd.print("Countdown sett.");
    lcd.setCursor(0,1);
    lcd.print(countDownText);

    if(!digitalRead(STARTBTN)){
      while(!digitalRead(STARTBTN)){
        countDownText = (step == 0) ? (millis() - millisBlinkText < 750) ? String(countMin/10)+String(countMin%10)+":" : "  :" : String(countMin/10)+String(countMin%10)+":";
        countDownText += (step == 1) ? (millis() - millisBlinkText < 750) ? String(countSec/10)+String(countSec%10)+".00" : "  .00" : String(countSec/10)+String(countSec%10)+".00";
        lcd.setCursor(0,0);
        lcd.print("Countdown sett.");
        lcd.setCursor(0,1);
        lcd.print(countDownText);
        if(millis() - millisBlinkText > 1000)
          millisBlinkText = millis();
        yield();
      }
      setCountDownVar(1, step);
    }
    if(!digitalRead(RESETBTN)){
      unsigned long millisBTN = millis();
      while(!digitalRead(RESETBTN)){
        countDownText = (step == 0) ? (millis() - millisBlinkText < 750) ? String(countMin/10)+String(countMin%10)+":" : "  :" : String(countMin/10)+String(countMin%10)+":";
        countDownText += (step == 1) ? (millis() - millisBlinkText < 750) ? String(countSec/10)+String(countSec%10)+".00" : "  .00" : String(countSec/10)+String(countSec%10)+".00";
        lcd.setCursor(0,0);
        lcd.print("Countdown sett.");
        lcd.setCursor(0,1);
        lcd.print(countDownText);
        if(millis() - millisBlinkText > 1000)
          millisBlinkText = millis();
        if(millis() - millisBTN > 4000)
          digitalWrite(LED, 0);
        else if(millis() - millisBTN > 2000)
          digitalWrite(LED, 1);
        yield();
      }
      digitalWrite(LED, 0);
      if(millis() - millisBTN < 2000)
        setCountDownVar(-1, step);
      else if(millis() - millisBTN < 4000)
        step++;
      else{
        state = RESETSTATE;
        returnState = TIMERSTOPSTATE;
      }
    }
    if(step > 1){
      state = COUNTDOWNSTOPSTATE;
      countDown = (countMin*60*1000)+(countSec*1000);
      countDownText = String(countMin/10)+String(countMin%10)+":";
      countDownText += String(countSec/10)+String(countSec%10)+".00";
      if(countDown == 0){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("timer: 00:00.00");
        lcd.setCursor(0,1);
        lcd.print("Return to sett.");
        delay(2000);
        state = COUNTDOWNSETSTATE;
      }
      lcd.clear();
      break;
    }
  }
}

void setCountDownVar(int num, int step){
  if(step == 0)
    countMin = (countMin+num >= 0 && countMin+num <= 59) ? countMin+num : countMin;
  else if(step == 1)
    countSec = (countSec+num >= 0 && countSec+num <= 59) ? countSec+num : countSec;
}