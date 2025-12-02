#include "EdgeServiceClient.h"
#include "HeartRateLog.h"
#include "config.h"
#include <time.h> 

EdgeServiceClient::EdgeServiceClient()
  : ssid(WIFI_SSID), 
    password(WIFI_PASSWORD), 
    backendUrl(EDGE_BACKEND_URL),
    deviceId(DEVICE_ID) {}  // 🔧 Usar DEVICE_ID desde config.h

void EdgeServiceClient::connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("🔌 Conectando a WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        Serial.print(".");
        delay(500);
        attempts++;
    }
    Serial.println();
    
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("✅ WiFi conectado");
        Serial.print("   IP: ");
        Serial.println(WiFi.localIP());
        
        // Configurar NTP para obtener tiempo real
        configTime(0, 0, "pool.ntp.org", "time.nist.gov");
        Serial.println("🕐 NTP configurado\n");
    } else {
        Serial.println("❌ Error: No se pudo conectar a WiFi\n");
    }
}

void EdgeServiceClient::setDeviceId(const char* id) {
    deviceId = id;
}

void EdgeServiceClient::sendHeartRateData(int heartRate) {
    if (!isConnected()) {
        Serial.println("⚠️  WiFi desconectado. Reconectando...");
        connectWifi();
        if (!isConnected()) {
            Serial.println("❌ No se pudo enviar datos: Sin conexión WiFi\n");
            return;
        }
    }

    HeartRateLog* heartRateLog = HeartRateLog::getInstance();
    heartRateLog->setHeartRate(heartRate);
    heartRateLog->setDeviceId(deviceId);
    
    // ============================================
    // OBTENER TIMESTAMP EN FORMATO ISO 8601
    // ============================================
    time_t now = time(nullptr);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    
    // ============================================
    // CONSTRUIR JSON EN FORMATO REQUERIDO
    // {
    //   "device_id": "string",
    //   "timestamp": "string",  
    //   "bpm": 0
    // }
    // ============================================
    String jsonData = "{";
    jsonData += "\"device_id\":\"" + String(heartRateLog->getDeviceId()) + "\",";
    jsonData += "\"timestamp\":\"" + String(timestamp) + "\",";
    jsonData += "\"bpm\":" + String(heartRateLog->getHeartRate()); 
    jsonData += "}";

    Serial.println("------------------------------------");
    Serial.println("📤 ENVIANDO DATOS AL BACKEND");
    Serial.println("------------------------------------");
    Serial.println("URL:  " + String(backendUrl));
    Serial.println("JSON: " + jsonData);

    client_http.begin(backendUrl);
    client_http.addHeader("Content-Type", "application/json");

    int httpCode = client_http.POST(jsonData);

    if (httpCode > 0) {
        String payload = client_http.getString();
        Serial.println("✅ Estado: " + String(httpCode));
        
        // Mostrar respuesta (limitada a 200 caracteres)
        if (payload.length() > 200) {
            Serial.println("📥 Respuesta: " + payload.substring(0, 200) + "...");
        } else {
            Serial.println("📥 Respuesta: " + payload);
        }
    } else {
        Serial.println("❌ Error al enviar datos");
        Serial.println("   Código: " + String(httpCode));
        Serial.println("   Detalle: " + client_http.errorToString(httpCode));
    }
    Serial.println("------------------------------------\n");

    client_http.end();
}

bool EdgeServiceClient::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}