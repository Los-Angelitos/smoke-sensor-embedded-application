#ifndef MODESTIO_H
#define MODESTIO_H

#include <Arduino.h>

// Abstract base class for all devices following MODEST pattern
class Device {
public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual bool isReady() const = 0;
    virtual ~Device() {}
};

// Abstract actuator interface
class Actuator : public Device {
public:
    virtual void actuate(float value) = 0;
    virtual void stop() = 0;
};

// Event handling system
class EventHandler {
public:
    enum EventType {
        SENSOR_DATA_UPDATED,
        THRESHOLD_EXCEEDED,
        CALIBRATION_COMPLETE,
        ERROR_OCCURRED
    };
    
    struct Event {
        EventType type;
        void* data;
        unsigned long timestamp;
    };
    
    virtual void handleEvent(const Event& event) = 0;
    virtual ~EventHandler() {}
};

#endif