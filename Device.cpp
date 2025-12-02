#include "Device.h"
#include <Wire.h>

Device::Device() 
  : edgeServiceClient(),
    pulseSimulator() {
    heartRateLog = HeartRateLog::getInstance();
}

void Device::setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    pinMode(LED_PIN, OUTPUT);
    
    Serial.println("\n\n");
    Serial.println("====================================");
    Serial.println("   SMART-BAND HEART RATE MONITOR   ");
    Serial.println("====================================");
    Serial.println("Device ID: ESP32-" + String(SMART_BAND_ID));
    Serial.println("Alert Threshold: " + String(BPM_THRESHOLD) + " BPM");
    Serial.println("====================================\n");
    
    edgeServiceClient.connectWifi();
    edgeServiceClient.setSmartBandId(SMART_BAND_ID);
    pulseSimulator.begin(PULSE_PIN);
    
    Serial.println("✅ System ready - Starting readings...\n");
}

void Device::loop() {
    float voltage = pulseSimulator.readVoltage();
    int heartRate = calculateHeartRate(voltage);

    Serial.print("\n💓 Heart Rate: ");
    Serial.print(heartRate);
    Serial.println(" BPM");
    
    sendDataToBackend(heartRate);
    controlLed(heartRate);
    
    delay(500);
}

void Device::sendDataToBackend(int heartRate) {
    edgeServiceClient.sendHeartRateData(heartRate);
}

int Device::calculateHeartRate(float voltage) {
    return (voltage / 3.3) * 675;
}

// ⭐ FUNCIÓN MODIFICADA 
void Device::controlLed(int heartRate) {
    // ============================================
    // LÓGICA DE ALERTA CON THRESHOLD
    // ============================================
    
    if (heartRate < MIN_HEART_RATE) {
        // Bradicardia (frecuencia muy baja)
        digitalWrite(LED_PIN, HIGH);
        Serial.println("\n🚨 ALERT: bpm=" + String(heartRate) + " 🚨");
        Serial.println("⚠️  Bradycardia detected (< " + String(MIN_HEART_RATE) + " BPM)");
        
    } else if (heartRate > BPM_THRESHOLD) {
        // Taquicardia (supera umbral configurable)
        digitalWrite(LED_PIN, HIGH);
        Serial.println("\n🚨 ALERT: bpm=" + String(heartRate) + " 🚨");
        Serial.println("⚠️  Tachycardia detected (> " + String(BPM_THRESHOLD) + " BPM)");
        
    } else {
        // Frecuencia normal
        digitalWrite(LED_PIN, LOW);
        Serial.println("✅ Normal heart rate");
    }
}