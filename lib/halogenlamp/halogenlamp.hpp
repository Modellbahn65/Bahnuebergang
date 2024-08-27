#include <stdint.h>

class halogenlamp {
 private:
  uint8_t pin;
  uint8_t currentBrightness;
  bool currentState;
  uint8_t lastStateChangeBrightness;
  long lastStateChangeTimestamp = 0;

 public:
  bool nextState;
  halogenlamp(uint8_t pin, bool initialState);
  void process();
};
