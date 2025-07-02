#include "SmokeSensorDevice.h"

SmokeSensorDevice::SmokeSensorDevice() : currentTemp(0), setPoint(25.0), heatingMode(false) {
}

void SmokeSensorDevice::init() {
    Serial.println("Smoke Sensor Device initialized (placeholder)");
    initialized = true;
    ready = true;
}

void SmokeSensorDevice::update() {
    lastUpdateTime = millis();
}

void SmokeSensorDevice::setTemperature(float temp) {
    currentTemp = temp;
}
