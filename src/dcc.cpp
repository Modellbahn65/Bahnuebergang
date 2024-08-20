#include <NmraDcc.h>

#define DCC_PIN 2
#define DCC_BOARD_ADDRESS 176u
#define ENABLE_PULLUP

#if (digitalPinToInterrupt(DCC_PIN) == NOT_AN_INTERRUPT)
  #error DCC_PIN is not an interrupt pin
#endif
#ifdef ENABLE_PULLUP
  #define PULLUP 1
#else
  #define PULLUP 0
#endif
#define DCC_ADDRESS (DCC_BOARD_ADDRESS * 4)

NmraDcc dcc;

void setupDcc() {
  dcc.pin(DCC_PIN, PULLUP);
  dcc.initAccessoryDecoder(MAN_ID_DIY, 1, FLAGS_MY_ADDRESS_ONLY, 0);
  while (!dcc.isSetCVReady())
    delay(1);
  dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, DCC_BOARD_ADDRESS & 0b00111111);
  while (!dcc.isSetCVReady())
    delay(1);
  dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB,
            (DCC_BOARD_ADDRESS >> 6) & 0b00000111);
  while (!dcc.isSetCVReady())
    delay(1);
  Serial.print("Current decoder board address: ");
  Serial.println(dcc.getAddr());
  Serial.print("Accessory addresses: ");
  Serial.print((dcc.getAddr() - 1) * 4 + 1);
  Serial.print(" - ");
  Serial.print((dcc.getAddr() - 1) * 4 + 1 + 3);
  Serial.println();
}

void processRequestedStateChange(uint8_t outputpair, bool direction);

void notifyDccAccTurnoutBoard(uint16_t BoardAddr,
                              uint8_t OutputPair,
                              uint8_t Direction,
                              uint8_t OutputPower) {
  /*
  Serial.print("notifyDccAccTurnoutBoard: boardaddr=");
  Serial.print(BoardAddr, DEC);
  Serial.print(" outputpair=");
  Serial.print(OutputPair, DEC);
  Serial.print(" direction=");
  Serial.print(Direction, DEC);
  Serial.print(" power=");
  Serial.println(OutputPower, DEC);
  */
  processRequestedStateChange(OutputPair, Direction);
}

void loopDcc() {
  dcc.process();
}
