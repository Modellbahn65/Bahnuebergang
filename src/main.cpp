#include <Arduino.h>

void setupDcc();
void loopDcc();
void setupOutput();
void loopOutput();

void setup() {
  setupOutput();
  Serial.begin(9600);
  if (!Serial)
    delay(5000);
  Serial.println("Bahnübergang by programminghoch10");
  setupDcc();
}

void loop() {
  loopDcc();
  loopOutput();
}
