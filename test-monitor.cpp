#include <gtest/gtest.h>
#include "./monitor.h"

TEST(Monitor, NotOkWhenAnyVitalIsOffRange) {
  ASSERT_FALSE(vitalsOk(99, 102, 70));
  ASSERT_TRUE(vitalsOk(98.1, 70, 98));
}

TEST(Monitor, EdgeCases) {
  ASSERT_FALSE(vitalsOk(94.9, 70, 98)); // Temperature just below range
  ASSERT_FALSE(vitalsOk(102.1, 70, 98)); // Temperature just above range
  ASSERT_FALSE(vitalsOk(98.1, 59.9, 98)); // Pulse rate just below range
  ASSERT_FALSE(vitalsOk(98.1, 100.1, 98)); // Pulse rate just above range
  ASSERT_FALSE(vitalsOk(98.1, 70, 89.9)); // Spo2 just below range
  ASSERT_TRUE(vitalsOk(95, 60, 90)); // Lower bounds of all vitals
  ASSERT_TRUE(vitalsOk(102, 100, 90)); // Upper bounds of all vitals
}
