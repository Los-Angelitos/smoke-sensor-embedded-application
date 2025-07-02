#ifndef EVENTHANDLER_IMPL_H
#define EVENTHANDLER_IMPL_H

#include "ModestIO.h"
#include "Actuator.h"
#include "DisplayDevice.h"

class SystemEventHandler : public EventHandler {
private:
    LEDActuator* ledActuator;
    DisplayDevice* display;
    
public:
    SystemEventHandler(LEDActuator* led, DisplayDevice* disp);
    void handleEvent(const Event& event) override;
};

#endif
