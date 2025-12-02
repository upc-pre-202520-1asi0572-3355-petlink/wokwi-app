#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// CONFIGURACIÓN WiFi
// ============================================
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// ============================================
// CONFIGURACIÓN EDGE SERVICE
// ============================================
#define EDGE_BACKEND_URL "https://jsonplaceholder.typicode.com/posts"

// ============================================
// IDENTIFICACIÓN DEL DISPOSITIVO
// ============================================
#define DEVICE_ID "COLLAR-001"  // 🔧 Variable de entorno para device_id

// ============================================
// CONFIGURACIÓN DE PINES
// ============================================
#define PULSE_PIN 35

// LEDs para indicadores de estado
#define LED_GREEN_PIN 12   // LED verde - Estado normal
#define LED_YELLOW_PIN 14  // LED amarillo - Alerta
#define LED_RED_PIN 13     // LED rojo - Crítico

// I2C pins (para futuras expansiones)
#define SDA_PIN 23
#define SCL_PIN 22

// ============================================
// UMBRALES DE RITMO CARDÍACO (BPM)
// 🔧 Variables de entorno configurables
// ============================================

// Umbrales para PERROS (valores estándar veterinarios)
// Razas pequeñas: 70-180 BPM
// Razas medianas: 60-140 BPM  
// Razas grandes: 60-100 BPM

// ESTADO CRÍTICO - Bradicardia severa
#define BPM_CRITICAL_LOW 50      // < 50 BPM = Crítico (LED ROJO)

// ESTADO NORMAL
#define BPM_NORMAL_LOW 60        // >= 60 BPM = Inicio rango normal
#define BPM_NORMAL_HIGH 120      // <= 120 BPM = Fin rango normal (LED VERDE)

// ESTADO ALERTA - Taquicardia moderada
#define BPM_ALERT_HIGH 140       // > 120 y <= 140 BPM = Alerta (LED AMARILLO)

// ESTADO CRÍTICO - Taquicardia severa  
#define BPM_CRITICAL_HIGH 160    // > 140 BPM = Crítico (LED ROJO)

// ============================================
// PARÁMETROS DE SIMULACIÓN
// ============================================
#define SIMULATION_UPDATE_INTERVAL 500 // ms entre actualizaciones

// ============================================
// CONVERSIÓN ADC A VOLTAJE
// ============================================
#define ADC_MAX 4095
#define VOLTAGE_MAX 5.0

// ============================================
// CONFIGURACIÓN DE ENVÍO DE DATOS
// ============================================
#define DATA_SEND_INTERVAL 2000  // ms entre envíos al backend

#endif // CONFIG_H