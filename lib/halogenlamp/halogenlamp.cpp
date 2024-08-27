#include "halogenlamp.hpp"
#include <Arduino.h>

// clamp output to range from 0 to 1
#define CLAMP_ONE(x) min(max(x, 0), 1)

float mapRange(float progress, float out_min, float out_max) {
  return progress * (out_max - out_min) + out_min;
}

#define GAMMA 2.2
float ledBrightnessCorrection(float brightness) {
  // return CLAMP_ONE(brightness * brightness);  // gamma = 2
  return CLAMP_ONE(pow(brightness, GAMMA));
}

float halogenDimUp(float progress) {
  return CLAMP_ONE(1 - pow(EULER, -10 * progress));
}
float halogenDimDown(float progress) {
  return CLAMP_ONE(pow(EULER, -5 * (progress)));
}

/**
 * @param preswitchbrightness brightness the halogen lamp had before state
 * switched
 * @param state the state the lamp currently has
 * @param seconds the amount of seconds passed since state change
 */
float dimHalogen(float preswitchbrightness, bool state, float seconds) {
  return state ? mapRange(ledBrightnessCorrection(halogenDimUp(seconds)),
                          preswitchbrightness, 1)
               : mapRange(ledBrightnessCorrection(halogenDimDown(seconds)), 0,
                          preswitchbrightness);
}

halogenlamp::halogenlamp(uint8_t pin, bool initialState) {
  this->nextState = initialState;
  this->currentState = initialState;
  this->pin = pin;
  this->currentBrightness = initialState;
  this->lastStateChangeTimestamp = millis();
  pinMode(this->pin, OUTPUT);
#ifdef ARDUINO_ARCH_AVR
  if (digitalPinToTimer(this->pin) == NOT_ON_TIMER) {
    Serial.print("Pin ");
    Serial.print(this->pin);
    Serial.println(" is not on timer, PWM is not possible.");
  }
#endif
}

void highResAnalogWrite(uint8_t pin, float value);

void halogenlamp::process() {
  if (this->nextState != this->currentState) {
    this->lastStateChangeBrightness = this->currentBrightness;
    this->currentState = this->nextState;
    this->lastStateChangeTimestamp = millis();
  }
  float seconds = (float)(millis() - this->lastStateChangeTimestamp) / 1000;
  this->currentBrightness =
      dimHalogen(this->lastStateChangeBrightness, this->currentState, seconds);
  highResAnalogWrite(this->pin, this->currentBrightness);
}
