#ifndef EDGE_SERVICE_CLIENT_H
#define EDGE_SERVICE_CLIENT_H

#include <WiFi.h>
#include <HTTPClient.h>

class EdgeServiceClient {
private:
    HTTPClient client_http;
    const char* ssid;
    const char* password;
    const char* backendUrl;
    const char* deviceId;  // 🔧 Cambiado de int a const char*

public:
    EdgeServiceClient();
    void connectWifi();
    void sendHeartRateData(int heartRate);
    bool isConnected();
    void setDeviceId(const char* id);  // 🔧 Cambiado de int a const char*
};

#endif // EDGE_SERVICE_CLIENT_H