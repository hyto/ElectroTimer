#include <Arduino.h>
#include <Ticker.h>
#include <Bounce2.h>

// #include "naxlib.h"

#define RELAY_A A0
#define RELAY_B A1
#define RELAY_ON LOW
#define RELAY_OFF HIGH
#define BUTTON_LESS A3
#define BUTTON_MORE A2
#define BAR_MODE_PROGRESS 0
#define BAR_MODE_TIME 1


void showProgressBar();
void checkTimerPos();

uint8_t progressMode = BAR_MODE_PROGRESS;
// NaxLib nax(12, 100);
Ticker timer(checkTimerPos, 1000, 0, MILLIS);
Ticker barTimer(showProgressBar, 250, 0, MILLIS);
int32_t count = 0;
int32_t pos = 0;
Bounce bLess = Bounce();
Bounce bMore = Bounce(); 
int bar[] = {3, 5, 6, 9, 10, 11};
int force = 0;

void setup() {
  digitalWrite(RELAY_A, RELAY_OFF);
  digitalWrite(RELAY_B, RELAY_OFF);
  pinMode(RELAY_A, OUTPUT);
  pinMode(RELAY_B, OUTPUT);

  bLess.attach(BUTTON_LESS, INPUT_PULLUP);
  bLess.interval(25);

  bMore.attach(BUTTON_MORE, INPUT_PULLUP);
  bMore.interval(25);
  
  for(int i = 0; i < 6; i++)
    pinMode(bar[i], OUTPUT);

  // put your setup code here, to run once:
  // nax.init();
  Serial.println("hola verano");
  timer.start();
  barTimer.start();
}

void setRelayValue(int value) {
  digitalWrite(RELAY_A, value);
  digitalWrite(RELAY_B, value);  
}


void barPosition(int _pos, int led) {
  int pwm_max = 75;
  float x = _pos / 3600.0 * 6.0;
  int f = round(pwm_max * (x - (led)));
  if(f < 0)
    f = 0;
  else if(f > pwm_max)
    f = pwm_max;

  analogWrite(bar[led], f);
}

void checkTimerPos() {
  if(pos > count) 
    pos = count;
  else if (pos > 0)
    pos--;

  if(pos <= 0) {
    setRelayValue(RELAY_OFF);
    Serial.println("se llego a cero");
  }
  else {
    setRelayValue(RELAY_ON);
  }

  if(pos % 600 == 0)
    count = pos;

  Serial.print("pos/count: ");
  Serial.print(pos);
  Serial.print("/");
  Serial.println(count);
}


void lessTime() {
  Serial.println("Restando 600");
  count -= 600;
  if (count < 0)
    count = 0;

  pos = count;

  checkTimerPos();
}

void moreTime() {
  Serial.println("Sumando 600");
  count += 600;
  if(count > 3600)
    count = 3600;
  pos = count;

  checkTimerPos();
}

void showProgressBar() {
  for(int i = 0; i < 6; i++)
    barPosition(pos, i);
  // if(progressMode == BAR_MODE_TIME) { 

  // }
  // else if (progressMode == BAR_MODE_PROGRESS){
    
  // }
}

void loop() {
  // nax.update();
  timer.update();
  barTimer.update();

  bLess.update();
  if(bLess.fell())
    lessTime();
  
  bMore.update();
  if(bMore.fell())
    moreTime();
}

