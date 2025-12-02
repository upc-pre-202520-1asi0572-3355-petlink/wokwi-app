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
    const char* deviceId;  // 🔧 Cambiado de smartBandId (int) a deviceId (const char*)

public:
    static HeartRateLog* getInstance();
    
    void setHeartRate(int rate);
    int getHeartRate();
    
    void setDeviceId(const char* id);
    const char* getDeviceId();
};

#endif // HEART_RATE_LOG_H