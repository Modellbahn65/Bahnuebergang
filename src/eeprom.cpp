#include <EEPROM.h>

// eeprom storage address for saving on/off state
#define EEPROM_ON_OFF 0

bool readOnOffState() {
  return EEPROM.read(EEPROM_ON_OFF);
}

void writeOnOffState(bool state) {
  if (readOnOffState() == state)
    return;
  EEPROM.write(EEPROM_ON_OFF, state);
}
