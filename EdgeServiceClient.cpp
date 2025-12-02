#include "EdgeServiceClient.h"
#include "HeartRateLog.h"
#include "config.h"

#include <time.h> 

EdgeServiceClient::EdgeServiceClient()
  : ssid(WIFI_SSID), password(WIFI_PASSWORD), backendUrl(EDGE_BACKEND_URL), smartBandId(SMART_BAND_ID) {}

void EdgeServiceClient::connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        Serial.print(".");
        delay(500);
        attempts++;
    }
    Serial.println();
    if(WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected. IP address: ");
        Serial.println(WiFi.localIP());
        
        //Configurar NTP para obtener tiempo real
        configTime(0, 0, "pool.ntp.org", "time.nist.gov");
        Serial.println("NTP time configured");
    } else {
        Serial.println("Failed to connect to WiFi");
    }
}

void EdgeServiceClient::setSmartBandId(int id) {
    smartBandId = id;
}

//FUNCIÓN MODIFICADA
void EdgeServiceClient::sendHeartRateData(int heartRate) {
    if (!isConnected()) {
        Serial.println("WiFi not connected. Reconnecting...");
        connectWifi();
    }

    HeartRateLog* heartRateLog = HeartRateLog::getInstance();
    heartRateLog->setHeartRate(heartRate);
    heartRateLog->setSmartBandId(smartBandId);
    
    // ============================================
    // NUEVO FORMATO JSON
    // ============================================
    
    // Obtener timestamp en formato ISO 8601
    time_t now = time(nullptr);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    
    // Crear device_id con formato: ESP32-<smartBandId>
    String deviceId = "ESP32-(" + String(heartRateLog->getSmartBandId()) + ")";
    
    // Construir JSON con formato requerido:
    // { "device_id":"<id>", "timestamp":"<ISO>", "bpm":<valor> }
    String jsonData = "{";
    jsonData += "\"device_id\":\"" + deviceId + "\",";
    jsonData += "\"timestamp\":\"" + String(timestamp) + "\",";
    jsonData += "\"bpm\":" + String(heartRateLog->getHeartRate()); 
    jsonData += "}";

    Serial.println("\n📤 Sending data to backend:");
    Serial.println("URL: " + String(backendUrl));
    Serial.println("JSON: " + jsonData);

    client_http.begin(backendUrl);
    client_http.addHeader("Content-Type", "application/json");

    int httpCode = client_http.POST(jsonData);

    if (httpCode > 0) {
        String payload = client_http.getString();
        Serial.println("✅ Status code: " + String(httpCode));
        Serial.println("Response: " + payload.substring(0, 200)); // Primeros 200 caracteres
    } else {
        Serial.println("❌ Error sending data to backend");
        Serial.println("Error: " + client_http.errorToString(httpCode));
    }

    client_http.end();
}

bool EdgeServiceClient::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}