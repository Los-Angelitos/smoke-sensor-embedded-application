#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Device.h"

// LED Actuator implementation
class LEDActuator : public BaseDevice, public Actuator {
private:
    int pin;
    bool state;
    
public:
    LEDActuator(int ledPin);
    void init() override;
    void update() override;
    void actuate(float value) override;
    void stop() override;
    bool getState() const { return state; }
};

#endif
