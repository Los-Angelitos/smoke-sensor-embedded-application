#ifndef SMOKESENSORDEVICE_H
#define SMOKESENSORDEVICE_H

#include "Device.h"

// Future expansion - thermostat functionality
class SmokeSensorDevice : public BaseDevice {
private:
    float currentTemp;
    float setPoint;
    bool heatingMode;
    
public:
    SmokeSensorDevice();
    void init() override;
    void update() override;
    void setTemperature(float temp);
    float getTemperature() const { return currentTemp; }
    void setSetPoint(float sp) { setPoint = sp; }
    float getSetPoint() const { return setPoint; }
};

#endif
