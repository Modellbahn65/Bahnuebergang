
// code for disabling the RX and TX leds
#undef TXLED0
#undef TXLED1
#undef RXLED0
#undef RXLED1
#undef TX_RX_LED_INIT
#define TXLED0
#define TXLED1
#define RXLED0
#define RXLED1
#define TX_RX_LED_INIT

#include <Arduino.h>
#include <halogenlamp.hpp>

#define PIN_BU_1 10
#define PIN_BU_2 9
#define PIN_ORANGE 3
#define PIN_WEISS_BW 6
#define PIN_WEISS_LS 5

#define OUTPUT_UPDATE_INTERVAL 10

bool weissLS = true;
halogenlamp weissLSlamp = halogenlamp(PIN_WEISS_LS, false);
bool weissBW = true;
halogenlamp weissBWlamp = halogenlamp(PIN_WEISS_BW, false);
bool blinkenBU = true;
halogenlamp blinkenBUphaseA = halogenlamp(PIN_BU_1, false);
halogenlamp blinkenBUphaseB = halogenlamp(PIN_BU_2, false);
halogenlamp orange = halogenlamp(PIN_ORANGE, true);

void setupPWM();
void highResAnalogWrite(uint8_t pin, uint16_t value);

void setupOutput() {
  pinMode(LED_BUILTIN_RX, OUTPUT);
  pinMode(LED_BUILTIN_TX, OUTPUT);
  digitalWrite(LED_BUILTIN_RX, true);
  digitalWrite(LED_BUILTIN_TX, true);
  setupPWM();
}

void processRequestedStateChange(uint8_t outputpair, bool direction) {
  switch (outputpair) {
    case 0:
      if (weissLS == direction)
        return;
      Serial.print("weissLS = ");
      Serial.println(direction);
      weissLS = direction;
      break;
    case 1:
      if (weissBW == direction)
        return;
      Serial.print("weissBW = ");
      Serial.println(direction);
      weissBW = direction;
      break;
    case 2:
      if (orange.nextState == direction)
        return;
      Serial.print("orange = ");
      Serial.println(direction);
      orange.nextState = direction;
      if (!direction) {
        weissBW = false;
        weissLS = false;
      }
      break;
    case 3:
      if (blinkenBU == direction)
        return;
      Serial.print("blinkenBU = ");
      Serial.println(direction);
      blinkenBU = direction;
      break;
    default:
      return;
  }
}

void processPhase();
bool getPhaseA();
bool getPhaseB();

void processLeds() {
  if (blinkenBU || weissBW || weissLS)
    processPhase();
  bool phaseA = getPhaseA();
  bool phaseB = getPhaseB();

  blinkenBUphaseA.nextState = blinkenBU && phaseA;
  blinkenBUphaseB.nextState = blinkenBU && phaseB;
  weissBWlamp.nextState = weissBW && phaseA;
  weissLSlamp.nextState = weissLS && phaseA;

  weissLSlamp.process();
  weissBWlamp.process();
  blinkenBUphaseA.process();
  blinkenBUphaseB.process();
  orange.process();

  digitalWrite(LED_BUILTIN_RX, !phaseA);
  digitalWrite(LED_BUILTIN_TX, !phaseB);
}

uint64_t lastoutputupdatems = 0;
void loopOutput() {
  if (millis() - lastoutputupdatems < OUTPUT_UPDATE_INTERVAL)
    return;
  processLeds();
  lastoutputupdatems = millis();
}
