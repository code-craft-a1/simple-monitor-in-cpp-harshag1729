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

struct VitalThreshold {
    float min;
    float max;
    float warningTolerancePercent;
    const char* name;
    const char* alertMessage;
    const char* warnLowMessage;
    const char* warnHighMessage;
};

float computeWarningDelta(const VitalThreshold& threshold) {
    return threshold.max * (threshold.warningTolerancePercent / 100.0f);
}

VitalStatus determineLowerRange(float value, float min, float nearLow) {
    if (value < min) return LOW;
    if (value < nearLow) return NEAR_LOW;
    return NORMAL;
}

VitalStatus determineUpperRange(float value, float nearHigh, float max) {
    if (value <= nearHigh) return NORMAL;
    if (value <= max) return NEAR_HIGH;
    return HIGH;
}

VitalStatus evaluateStatus(float value, const VitalThreshold& threshold) {
    float delta = computeWarningDelta(threshold);
    float nearLow = threshold.min + delta;
    float nearHigh = threshold.max - delta;

    return value < nearHigh
           ? determineLowerRange(value, threshold.min, nearLow)
           : determineUpperRange(value, nearHigh, threshold.max);
}

void showAlert(const char* message) {
    cout << message << "\n";
    for (int i = 0; i < 6; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

bool isCritical(VitalStatus status) {
    return status == LOW || status == HIGH;
}

void showWarning(VitalStatus status, const VitalThreshold& threshold) {
    if (status == NEAR_LOW) {
        cout << threshold.name << ": " << threshold.warnLowMessage << "\n";
    } else if (status == NEAR_HIGH) {
        cout << threshold.name << ": " << threshold.warnHighMessage << "\n";
    }
}

bool checkAndAlert(float value, const VitalThreshold& threshold) {
    VitalStatus status = evaluateStatus(value, threshold);
    if (isCritical(status)) {
        showAlert(threshold.alertMessage);
        return false;
    }
    showWarning(status, threshold);
    return true;
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    VitalThreshold tempThresh = {
        95.0f, 102.0f, 1.5f,
        "Temperature", "Temperature is critical!",
        "Warning: Approaching hypothermia",
        "Warning: Approaching hyperthermia"
    };

    VitalThreshold pulseThresh = {
        60.0f, 100.0f, 1.5f,
        "Pulse Rate", "Pulse Rate is out of range!",
        "Warning: Approaching bradycardia",
        "Warning: Approaching tachycardia"
    };

    VitalThreshold spo2Thresh = {
        90.0f, 100.0f, 1.5f,
        "SPO2", "Oxygen Saturation out of range!",
        "Warning: Approaching hypoxia",
        "Warning: Approaching SPO2 ceiling"
    };

    bool tempOk = checkAndAlert(temperature, tempThresh);
    bool pulseOk = checkAndAlert(pulseRate, pulseThresh);
    bool spo2Ok = checkAndAlert(spo2, spo2Thresh);

    return tempOk && pulseOk && spo2Ok;
}
