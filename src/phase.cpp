#include <Arduino.h>
#include <Gaussian.h>

static Gaussian gaussian = Gaussian();

unsigned long phaseStart = 0;
unsigned long phaseLength = 1000;
double phaseAratio = 1;
unsigned long phaseALength = 500;
unsigned long phaseAStart = 0;
double phaseBratio = 1;
unsigned long phaseBLength = 500;
unsigned long phaseBStart = 500;

double normalDistributionRandom(double mean, double variance) {
  return gaussian.setMean(mean).setVariance(variance).random();
}

double normalDistributionRandomRange(double min, double max) {
  return gaussian.setMean(min + (max - min) / 2)
      .setVariance((max - min) / 4)
      .random();
}

void processPhase() {
  if (millis() - phaseStart < phaseLength)
    return;
  phaseStart = millis();
  // phaseLength = normalDistributionRandom(60.0, 0.75) / 60 * 1000;
  phaseLength = normalDistributionRandom(1, 0.05) * 1000;
  do {
    phaseAratio = normalDistributionRandomRange(0.8, 1.1);
    phaseBratio = normalDistributionRandomRange(0.7, 1.0);
  } while (phaseAratio + phaseBratio > 2);
  phaseALength = phaseLength / 2 * phaseAratio;
  phaseBLength = phaseLength / 2 * phaseBratio;
  unsigned long phaseHalfLength = phaseLength / 2;
  phaseAStart = phaseALength >= phaseHalfLength
                    ? 0
                    : (phaseHalfLength - phaseALength) / 2;
  phaseBStart = phaseBLength >= phaseHalfLength
                    ? phaseLength - phaseBLength
                    : (phaseHalfLength - phaseBLength) / 2 + phaseHalfLength;
  /*
  Serial.print("phaseCalculation:");
  Serial.print(" phaseStart=");
  Serial.print(phaseStart);
  Serial.print(" phaseLength=");
  Serial.print(phaseLength);
  Serial.println();
  Serial.print(" phaseAratio=");
  Serial.print(phaseAratio);
  Serial.print(" phaseBratio=");
  Serial.print(phaseBratio);
  Serial.println();
  Serial.print(" phaseALength=");
  Serial.print(phaseALength);
  Serial.print(" phaseBLength=");
  Serial.print(phaseBLength);
  Serial.print(" phaseHalfLength=");
  Serial.print(phaseHalfLength);
  Serial.println();
  Serial.print(" phaseAStart=");
  Serial.print(phaseAStart);
  Serial.print(" phaseBStart=");
  Serial.print(phaseBStart);
  Serial.println();
  */
}

bool getPhaseA() {
  unsigned long timeSincePhaseStart = millis() - phaseStart;
  return timeSincePhaseStart >= phaseAStart &&
         timeSincePhaseStart < phaseAStart + phaseALength;
}
bool getPhaseB() {
  unsigned long timeSincePhaseStart = millis() - phaseStart;
  return timeSincePhaseStart >= phaseBStart &&
         timeSincePhaseStart < phaseBStart + phaseBLength;
}
