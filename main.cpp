// For building with cmake.

#ifndef ARDUINO
#include "slowmod.h"

int main() {
  SlowMod sm;
  sm.EnableNormalisationProbe();
  sm.Run();
}

#endif