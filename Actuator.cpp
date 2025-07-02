#include "Actuator.h"

LEDActuator::LEDActuator(int ledPin) : pin(ledPin), state(false) {
}

void LEDActuator::init() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    state = false;
    initialized = true;
    ready = true;
    Serial.println("LED Actuator initialized");
}

void LEDActuator::update() {
    lastUpdateTime = millis();
    // LED state is controlled by actuate() method
}

void LEDActuator::actuate(float value) {
    state = (value > 0);
    digitalWrite(pin, state ? HIGH : LOW);
}

void LEDActuator::stop() {
    state = false;
    digitalWrite(pin, LOW);
}