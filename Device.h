#ifndef DEVICE_H
#define DEVICE_H

#include "ModestIO.h"

// Base device class implementation
class BaseDevice : public Device {
protected:
    bool initialized;
    bool ready;
    unsigned long lastUpdateTime;
    
public:
    BaseDevice() : initialized(false), ready(false), lastUpdateTime(0) {}
    
    virtual bool isReady() const override {
        return ready && initialized;
    }
    
    bool isInitialized() const { return initialized; }
    unsigned long getLastUpdateTime() const { return lastUpdateTime; }
};

#endif