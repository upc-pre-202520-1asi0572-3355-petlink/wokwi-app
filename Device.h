#ifndef DEVICE_H
#define DEVICE_H

#include "EdgeServiceClient.h"
#include "PulseSimulator.h"
#include "HeartRateLog.h"

class Device {
public:
    // Enum para estados del ritmo cardíaco
    enum HeartRateStatus {
        CRITICAL_LOW,    // Bradicardia severa (< 50 BPM)
        NORMAL,          // Rango normal (60-120 BPM)
        ALERT,           // Taquicardia moderada (121-140 BPM)
        CRITICAL_HIGH    // Taquicardia severa (> 160 BPM)
    };

    Device();
    void setup();
    void loop();

private:
    EdgeServiceClient edgeServiceClient;
    PulseSimulator pulseSimulator;
    HeartRateLog* heartRateLog;
    unsigned long lastSendTime;
    
    // Funciones privadas
    void sendDataToBackend(int heartRate);
    int calculateHeartRate(float voltage);
    HeartRateStatus getHeartRateStatus(int bpm);
    void controlLeds(HeartRateStatus status);
    void displayHeartRateInfo(int bpm, HeartRateStatus status);
    void testLeds();
    void sendData(HeartRateStatus status, int passTime, int heartRate);
};

#endif // DEVICE_H