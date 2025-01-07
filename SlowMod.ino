// For building with Arduino IDE.

#include "slowmod.h"

SlowMod sm;

void setup() {
  sm.EnableNormalisationProbe();
}

void loop() {
  sm.Run();
}
