
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

#define PIN_BU_1 8
#define PIN_BU_2 9
#define PIN_ORANGE_BW 7
#define PIN_ORANGE_LS 6
#define PIN_WEISS_BW 3
#define PIN_WEISS_LS 5

#define OUTPUT_UPDATE_INTERVAL 100

typedef struct output {
  bool weissLS = false;
  bool weissBW = false;
  bool orangeLS = true;
  bool orangeBW = true;
  bool blinkenBU = false;
} STATE;
STATE state;

void setupOutput() {
  pinMode(PIN_BU_1, OUTPUT);
  pinMode(PIN_BU_2, OUTPUT);
  pinMode(PIN_ORANGE_BW, OUTPUT);
  pinMode(PIN_ORANGE_LS, OUTPUT);
  pinMode(PIN_WEISS_BW, OUTPUT);
  pinMode(PIN_WEISS_LS, OUTPUT);

  pinMode(LED_BUILTIN_RX, OUTPUT);
  pinMode(LED_BUILTIN_TX, OUTPUT);
  digitalWrite(LED_BUILTIN_RX, true);
  digitalWrite(LED_BUILTIN_TX, true);

  digitalWrite(PIN_BU_1, true);
  digitalWrite(PIN_BU_2, true);
  digitalWrite(PIN_ORANGE_BW, true);
  digitalWrite(PIN_ORANGE_LS, true);
  digitalWrite(PIN_WEISS_BW, true);
  digitalWrite(PIN_WEISS_LS, true);
}

void stateChanged() {
  if (!state.blinkenBU) {
    digitalWrite(PIN_BU_1, false);
    digitalWrite(PIN_BU_2, false);
  }
  digitalWrite(PIN_ORANGE_BW, state.orangeBW);
  digitalWrite(PIN_ORANGE_LS, state.orangeLS);
  if (!state.weissBW)
    digitalWrite(PIN_WEISS_BW, false);
  if (!state.weissLS)
    digitalWrite(PIN_WEISS_LS, false);

  // temp

  digitalWrite(PIN_BU_1, state.blinkenBU);
  digitalWrite(PIN_BU_2, state.blinkenBU);
  digitalWrite(PIN_WEISS_BW, state.weissBW);
  digitalWrite(PIN_WEISS_LS, state.weissLS);
}

void processRequestedStateChange(uint8_t outputpair, bool direction) {
  switch (outputpair) {
    case 0:
      if (state.weissLS == direction)
        return;
      Serial.print("weissLS = ");
      Serial.println(direction);
      state.weissLS = direction;
      break;
    case 1:
      if (state.weissBW == direction)
        return;
      Serial.print("weissBW = ");
      Serial.println(direction);
      state.weissBW = direction;
      break;
    case 2:
      if (state.orangeBW == direction && state.orangeLS == direction)
        return;
      Serial.print("orange = ");
      Serial.println(direction);
      state.orangeBW = direction;
      state.orangeLS = direction;
      if (!direction) {
        state.weissBW = false;
        state.weissLS = false;
      }
      break;
    case 3:
      if (state.blinkenBU == direction)
        return;
      Serial.print("blinkenBU = ");
      Serial.println(direction);
      state.blinkenBU = direction;
      break;
    default:
      return;
  }
  stateChanged();
}

uint64_t lastoutputupdatems = 0;
void loopOutput() {
  if (millis() - lastoutputupdatems < OUTPUT_UPDATE_INTERVAL)
    return;
  bool phaseA = ((millis() / 1000) % 2) == 0;
  digitalWrite(PIN_BU_1, state.blinkenBU && phaseA);
  digitalWrite(PIN_BU_2, state.blinkenBU && !phaseA);
  digitalWrite(PIN_WEISS_BW, state.weissBW && phaseA);
  digitalWrite(PIN_WEISS_LS, state.weissLS && phaseA);
  digitalWrite(LED_BUILTIN_RX, phaseA);
  digitalWrite(LED_BUILTIN_TX, !phaseA);
  lastoutputupdatems = millis();
}
