#include "./monitor.h"
#include <assert.h>
#include <iostream>

void testVitalsOk() {
  assert(vitalsOk(98.6, 75, 95) == 1);
  assert(vitalsOk(103, 75, 95) == 0);
  assert(vitalsOk(98.6, 55, 95) == 0);
  assert(vitalsOk(98.6, 75, 85) == 0);
}

void testIndividualChecks() {
  assert(isTemperatureOk(98.6) == true);
  assert(isTemperatureOk(103) == false);
  assert(isPulseRateOk(75) == true);
  assert(isPulseRateOk(55) == false);
  assert(isSpo2Ok(95) == true);
  assert(isSpo2Ok(85) == false);
}

void testEdgeCases() {
  assert(vitalsOk(94.9, 70, 98) == 0); // Temperature just below range
  assert(vitalsOk(102.1, 70, 98) == 0); // Temperature just above range
  assert(vitalsOk(98.1, 59.9, 98) == 0); // Pulse rate just below range
  assert(vitalsOk(98.1, 100.1, 98) == 0); // Pulse rate just above range
  assert(vitalsOk(98.1, 70, 89.9) == 0); // Spo2 just below range
  assert(vitalsOk(95, 60, 90) == 1); // Lower bounds of all vitals
  assert(vitalsOk(102, 100, 90) == 1); // Upper bounds of all vitals
}

int main() {
  testVitalsOk();
  testIndividualChecks();
  testEdgeCases();
  std::cout << "All tests passed!\n";
  std::cout << "Edge case tests passed!\n";
  return 0;
}
