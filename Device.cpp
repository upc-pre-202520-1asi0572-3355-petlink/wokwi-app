#include "Device.h"
#include <Wire.h>

Device::Device() 
  : edgeServiceClient(),
    pulseSimulator(),
    lastSendTime(0) {
    heartRateLog = HeartRateLog::getInstance();
}

void Device::setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    
    // Configurar los 3 LEDs como salida
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    
    // Apagar todos los LEDs al inicio
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
    
    Serial.println("\n\n");
    Serial.println("====================================");
    Serial.println("   PETLINK - COLLAR MONITOR IoT    ");
    Serial.println("====================================");
    Serial.println("Device ID: " + String(DEVICE_ID));
    Serial.println("------------------------------------");
    Serial.println("📊 UMBRALES DE RITMO CARDÍACO:");
    Serial.println("🔴 Crítico bajo:  < " + String(BPM_CRITICAL_LOW) + " BPM");
    Serial.println("🟢 Normal:        " + String(BPM_NORMAL_LOW) + "-" + String(BPM_NORMAL_HIGH) + " BPM");
    Serial.println("🟡 Alerta:        " + String(BPM_NORMAL_HIGH + 1) + "-" + String(BPM_ALERT_HIGH) + " BPM");
    Serial.println("🔴 Crítico alto:  > " + String(BPM_CRITICAL_HIGH) + " BPM");
    Serial.println("====================================\n");
    
    edgeServiceClient.connectWifi();
    edgeServiceClient.setDeviceId(DEVICE_ID);
    pulseSimulator.begin(PULSE_PIN);
    
    Serial.println("✅ Sistema listo - Iniciando monitoreo...\n");
    
    // Test de LEDs al inicio
    testLeds();
}

void Device::loop() {
    float voltage = pulseSimulator.readVoltage();
    int heartRate = calculateHeartRate(voltage);

    // Determinar estado del ritmo cardíaco
    HeartRateStatus status = getHeartRateStatus(heartRate);
    
    // Mostrar información en consola
    displayHeartRateInfo(heartRate, status);
    
    // Control de LEDs según el estado
    controlLeds(status);
    
    // Enviar datos al backend cada DATA_SEND_INTERVAL
    unsigned long currentTime = millis();
    sendData(status, currentTime-lastSendTime, heartRate);
    lastSendTime = currentTime;
        
    delay(500);
}

void Device::sendData(HeartRateStatus status, int passTime, int heartRate){
    if(status == CRITICAL_LOW || status == CRITICAL_HIGH){
        sendDataToBackend(heartRate);
    }
    if (passTime >= DATA_SEND_INTERVAL){
        sendDataToBackend(heartRate);
    }
}

void Device::sendDataToBackend(int heartRate) {
    edgeServiceClient.sendHeartRateData(heartRate);
}

int Device::calculateHeartRate(float voltage) {
    // Conversión de voltaje a BPM
    // Fórmula: BPM = (voltage / 3.3) * 675
    return (voltage / 3.3) * 675;
}

// ============================================
// NUEVA FUNCIÓN: Determinar estado del BPM
// ============================================
Device::HeartRateStatus Device::getHeartRateStatus(int bpm) {
    if (bpm < BPM_CRITICAL_LOW) {
        return CRITICAL_LOW;  // Bradicardia severa
    } 
    else if (bpm >= BPM_NORMAL_LOW && bpm <= BPM_NORMAL_HIGH) {
        return NORMAL;  // Rango normal
    } 
    else if (bpm > BPM_NORMAL_HIGH && bpm <= BPM_ALERT_HIGH) {
        return ALERT;  // Taquicardia moderada
    } 
    else if (bpm > BPM_CRITICAL_HIGH) {
        return CRITICAL_HIGH;  // Taquicardia severa
    }
    else {
        // Rango entre CRITICAL_LOW y NORMAL_LOW, o entre ALERT_HIGH y CRITICAL_HIGH
        // Lo consideramos como transición hacia alerta
        return ALERT;
    }
}

// ============================================
// NUEVA FUNCIÓN: Control de LEDs según estado
// ============================================
void Device::controlLeds(HeartRateStatus status) {
    // Apagar todos los LEDs primero
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
    
    // Encender el LED correspondiente al estado
    switch(status) {
        case NORMAL:
            digitalWrite(LED_GREEN_PIN, HIGH);  // 🟢 Verde
            break;
            
        case ALERT:
            digitalWrite(LED_YELLOW_PIN, HIGH);  // 🟡 Amarillo
            break;
            
        case CRITICAL_LOW:
        case CRITICAL_HIGH:
            digitalWrite(LED_RED_PIN, HIGH);  // 🔴 Rojo
            break;
    }
}

// ============================================
// NUEVA FUNCIÓN: Mostrar información del BPM
// ============================================
void Device::displayHeartRateInfo(int bpm, HeartRateStatus status) {
    Serial.print("\n💓 Ritmo Cardíaco: ");
    Serial.print(bpm);
    Serial.print(" BPM");
    
    switch(status) {
        case CRITICAL_LOW:
            Serial.println(" 🔴 [CRÍTICO - BRADICARDIA]");
            Serial.println("⚠️  ¡ATENCIÓN! Frecuencia cardíaca peligrosamente baja");
            Serial.println("   Acción requerida: Contactar veterinario inmediatamente");
            break;
            
        case NORMAL:
            Serial.println(" 🟢 [NORMAL]");
            Serial.println("✅ Frecuencia cardíaca dentro de rangos saludables");
            break;
            
        case ALERT:
            Serial.println(" 🟡 [ALERTA]");
            Serial.println("⚠️  Frecuencia cardíaca elevada - Monitorear de cerca");
            break;
            
        case CRITICAL_HIGH:
            Serial.println(" 🔴 [CRÍTICO - TAQUICARDIA]");
            Serial.println("⚠️  ¡ATENCIÓN! Frecuencia cardíaca peligrosamente alta");
            Serial.println("   Acción requerida: Contactar veterinario inmediatamente");
            break;
    }
}

// ============================================
// NUEVA FUNCIÓN: Test de LEDs al iniciar
// ============================================
void Device::testLeds() {
    Serial.println("🔧 Probando LEDs...");
    
    // Test LED Verde
    digitalWrite(LED_GREEN_PIN, HIGH);
    Serial.println("   🟢 LED Verde: OK");
    delay(500);
    digitalWrite(LED_GREEN_PIN, LOW);
    
    // Test LED Amarillo
    digitalWrite(LED_YELLOW_PIN, HIGH);
    Serial.println("   🟡 LED Amarillo: OK");
    delay(500);
    digitalWrite(LED_YELLOW_PIN, LOW);
    
    // Test LED Rojo
    digitalWrite(LED_RED_PIN, HIGH);
    Serial.println("   🔴 LED Rojo: OK");
    delay(500);
    digitalWrite(LED_RED_PIN, LOW);
    
    Serial.println("✅ Test de LEDs completado\n");
}