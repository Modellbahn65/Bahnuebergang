#include <TimerFour.h>
#include <TimerOne.h>
#include <TimerThree.h>

#define TIMER_PERIOD_US 20000
void setupPWM() {
  Timer1.initialize(TIMER_PERIOD_US);
  Timer3.initialize(TIMER_PERIOD_US);
  Timer4.initialize(TIMER_PERIOD_US);
}

#define mapTo16bit(value) value * 65535
#define mapTo10bit(value) value * 1023
#define mapTo8bit(value) value * 255

void highResAnalogWrite(uint8_t pin, float value) {
  if (pin == 9 || pin == 10)
    return Timer1.pwm(pin, mapTo10bit(value));
  if (pin == 5)
    return Timer3.pwm(pin, mapTo10bit(value));
  if (pin == 6)
    return Timer4.pwm(pin, mapTo10bit(value));
  analogWrite(pin, mapTo8bit(value));
}
