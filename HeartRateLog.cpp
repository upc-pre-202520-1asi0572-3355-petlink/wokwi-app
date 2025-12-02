#include "HeartRateLog.h"

HeartRateLog* HeartRateLog::instance = nullptr;

HeartRateLog::HeartRateLog() : deviceId("") {}

HeartRateLog* HeartRateLog::getInstance() {
    if (instance == nullptr) {
        instance = new HeartRateLog();
    }
    return instance;
}

void HeartRateLog::setHeartRate(int rate) {
    heartRate = rate;
}

int HeartRateLog::getHeartRate() {
    return heartRate;
}

void HeartRateLog::setDeviceId(const char* id) {
    deviceId = id;
}

const char* HeartRateLog::getDeviceId() {
    return deviceId;
}