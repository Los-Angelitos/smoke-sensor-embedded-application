#include "DisplayDevice.h"

DisplayDevice::DisplayDevice() : lcd(0x27, 16, 2) {
}

void DisplayDevice::init() {
  Serial.println("Initializing Display...");
  
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  
  initialized = true;
  ready = true;
  Serial.println("Display initialized successfully");
}

void DisplayDevice::update() {
  lastUpdateTime = millis();
  // Display updates are handled by updateDisplay method
}

void DisplayDevice::showStartupMessage() {
  lcd.clear();
  lcd.print("Gas Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Modbus RTU");
  delay(2000);
  
  lcd.clear();
  lcd.print("Ready - Modbus");
  lcd.setCursor(0, 1);
  lcd.print("ID: 1");
  delay(1000);
}

void DisplayDevice::showCalibrationProgress(int progress) {
  lcd.clear();
  lcd.print("Calibrating...");
  lcd.setCursor(0, 1);
  for(int i = 0; i < progress && i < 16; i++) {
    lcd.print(".");
  }
}

void DisplayDevice::updateDisplay(const GasData& gasData) {
  if(millis() - lastUpdateTime >= DISPLAY_UPDATE_INTERVAL) {
    lcd.clear();
    
    // First line: Gas level
    lcd.setCursor(0, 0);
    lcd.print("Gas: ");
    lcd.print(gasData.gasPercentage);
    lcd.print("%");
    
    // Show PPM if there's space
    if (gasData.gasPercentage < 100) {
      lcd.print(" ");
      if (gasData.ppmValue < 1000) {
        lcd.print((int)gasData.ppmValue);
        lcd.print("ppm");
      }
    }
    
    // Second line: Status and Modbus info
    lcd.setCursor(0, 1);
    if (gasData.isUnsafe) {
      lcd.print("UNSAFE");
    } else {
      lcd.print("SAFE");
    }
    
    lcd.print(" MB:1");
    
    lastUpdateTime = millis();
  }
}

void DisplayDevice::showError(const String& message) {
  lcd.clear();
  lcd.print("ERROR:");
  lcd.setCursor(0, 1);
  lcd.print(message.substring(0, 16));
}