#include "EventHandler.h"

SystemEventHandler::SystemEventHandler(LEDActuator* led, DisplayDevice* disp) 
    : ledActuator(led), display(disp) {
}

void SystemEventHandler::handleEvent(const Event& event) {
    switch(event.type) {
        case SENSOR_DATA_UPDATED:
            // Data is handled by main loop
            break;
            
        case THRESHOLD_EXCEEDED:
            if(ledActuator) {
                ledActuator->actuate(1.0); // Turn on LED
            }
            if(display) {
                display->showError("GAS ALERT!");
            }
            break;
            
        case CALIBRATION_COMPLETE:
            Serial.println("Event: Calibration completed");
            break;
            
        case ERROR_OCCURRED:
            if(ledActuator) {
                ledActuator->stop();
            }
            Serial.println("Event: Error occurred");
            break;
    }
}
