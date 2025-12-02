#ifndef HEART_RATE_LOG_H
#define HEART_RATE_LOG_H

#include <Arduino.h>

class HeartRateLog {
private:
    HeartRateLog();
    HeartRateLog(const HeartRateLog&) = delete;
    HeartRateLog& operator=(const HeartRateLog&) = delete;
    static HeartRateLog* instance;
    int heartRate;
    int smartBandId;

public:
    static HeartRateLog* getInstance();
    void setHeartRate(int rate);
    int getHeartRate();
    void setSmartBandId(int id);
    int getSmartBandId();
};

#endif