#include "./monitor.h"
#include <iostream>
#include <chrono>
#include <thread>

using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

enum VitalStatus {
    LOW,
    NEAR_LOW,
    NORMAL,
    NEAR_HIGH,
    HIGH
};

struct VitalLimits {
    float minValue;
    float maxValue;
    float warningTolerancePercent;
    const char* vitalName;
    const char* criticalMessage;
    const char* nearLowMessage;
    const char* nearHighMessage;
};

float calculateWarningDelta(const VitalLimits& limits) {
    return limits.maxValue * (limits.warningTolerancePercent / 100.0f);
}

VitalStatus determineStatus(float value, const VitalLimits& limits) {
    float warningDelta = calculateWarningDelta(limits);
    float nearLowThreshold = limits.minValue + warningDelta;
    float nearHighThreshold = limits.maxValue - warningDelta;

    if (value < limits.minValue) return LOW;
    if (value < nearLowThreshold) return NEAR_LOW;
    if (value <= nearHighThreshold) return NORMAL;
    if (value <= limits.maxValue) return NEAR_HIGH;
    return HIGH;
}

void displayCriticalAlert(const char* message) {
    cout << message << "\n";
    for (int i = 0; i < 6; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

void displayWarning(VitalStatus status, const VitalLimits& limits) {
    if (status == NEAR_LOW) {
        cout << limits.vitalName << ": " << limits.nearLowMessage << "\n";
    } else if (status == NEAR_HIGH) {
        cout << limits.vitalName << ": " << limits.nearHighMessage << "\n";
    }
}

bool evaluateVital(float value, const VitalLimits& limits) {
    VitalStatus status = determineStatus(value, limits);
    if (status == LOW || status == HIGH) {
        displayCriticalAlert(limits.criticalMessage);
        return false;
    }
    displayWarning(status, limits);
    return true;
}

int checkVitals(float temperature, float pulseRate, float spo2) {
    VitalLimits temperatureLimits = {
        95.0f, 102.0f, 1.5f,
        "Temperature", "Temperature is critical!",
        "Warning: Approaching hypothermia",
        "Warning: Approaching hyperthermia"
    };

    VitalLimits pulseRateLimits = {
        60.0f, 100.0f, 1.5f,
        "Pulse Rate", "Pulse Rate is out of range!",
        "Warning: Approaching bradycardia",
        "Warning: Approaching tachycardia"
    };

    VitalLimits spo2Limits = {
        90.0f, 100.0f, 1.5f,
        "SPO2", "Oxygen Saturation out of range!",
        "Warning: Approaching hypoxia",
        "Warning: Approaching SPO2 ceiling"
    };

    bool temperatureOk = evaluateVital(temperature, temperatureLimits);
    bool pulseRateOk = evaluateVital(pulseRate, pulseRateLimits);
    bool spo2Ok = evaluateVital(spo2, spo2Limits);

    return temperatureOk && pulseRateOk && spo2Ok;
}
