#ifndef DISPLAYDEVICE_H
#define DISPLAYDEVICE_H

#include <LiquidCrystal_I2C.h>
#include "Device.h"

struct GasData {
  int gasPercentage;
  float ppmValue;
  int rawADC;
  bool isUnsafe;
  float r0Value;
  unsigned long timestamp;
  bool sensorOK;
};

class DisplayDevice : public BaseDevice {
private:
  LiquidCrystal_I2C lcd;
  const unsigned long DISPLAY_UPDATE_INTERVAL = 1000;
  
public:
  DisplayDevice();
  void init() override;
  void update() override;
  void showStartupMessage();
  void showCalibrationProgress(int progress);
  void updateDisplay(const GasData& gasData);
  void showError(const String& message);
};

#endif