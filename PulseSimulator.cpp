#include "PulseSimulator.h"

void PulseSimulator::begin(int pin) {
    _pin = pin;
    _simulationEnabled = false; //false?
    _voltage = 0;
    _lastUpdateTime = millis();
    pinMode(_pin, INPUT);
}

float PulseSimulator::readVoltage() {
    if (_simulationEnabled) {
        simulatePulse();
    } else {
        int16_t pulseValue = analogRead(_pin);
        _voltage = pulseValue * (VOLTAGE_MAX / ADC_MAX);
    }
    return _voltage;
}

void PulseSimulator::enableSimulation(bool enable) {
    _simulationEnabled = enable;
}

void PulseSimulator::simulatePulse() {
    unsigned long currentTime = millis();
    if (currentTime - _lastUpdateTime >= SIMULATION_UPDATE_INTERVAL) {
        _voltage = 0.3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.0 - 0.9)));
        _lastUpdateTime = currentTime;
    }
}