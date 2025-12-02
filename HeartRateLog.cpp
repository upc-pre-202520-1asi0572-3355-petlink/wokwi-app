#include "HeartRateLog.h"

HeartRateLog* HeartRateLog::instance = nullptr;
HeartRateLog::HeartRateLog() {}

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

void HeartRateLog::setSmartBandId(int id) {
    smartBandId = id;
}

int HeartRateLog::getSmartBandId() {
    return smartBandId;
}