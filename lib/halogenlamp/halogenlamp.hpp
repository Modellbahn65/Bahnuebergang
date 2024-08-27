#include <stdint.h>

class halogenlamp {
 private:
  uint8_t pin;
  float currentBrightness;
  bool currentState;
  float lastStateChangeBrightness;
  long lastStateChangeTimestamp = 0;

 public:
  bool nextState;
  halogenlamp(uint8_t pin, bool initialState);
  void process();
};
