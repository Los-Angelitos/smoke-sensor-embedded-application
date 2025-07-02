#include "CommandHandler.h"

CommandHandler::CommandHandler() : eventHandler(nullptr) {
  inputBuffer = "";
}

void CommandHandler::init() {
  Serial.println("Command Handler initialized");
  Serial.println("Available commands:");
  Serial.println("  status - Show system status");
  Serial.println("  calibrate - Recalibrate sensor");
  Serial.println("  registers - Show Modbus registers");
  
  initialized = true;
  ready = true;
}

void CommandHandler::update() {
  processCommands();
  lastUpdateTime = millis();
}

void CommandHandler::setEventHandler(SystemEventHandler* handler) {
  eventHandler = handler;
}

void CommandHandler::processCommands() {
  while(Serial.available()) {
    char c = Serial.read();
    if(c == '\n' || c == '\r') {
      if(inputBuffer.length() > 0) {
        handleCommand(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }
}

void CommandHandler::handleCommand(const String& command) {
  String cmd = command;
  cmd.trim();
  cmd.toLowerCase();
  
  if(cmd == "status") {
    Serial.println("=== SYSTEM STATUS ===");
    Serial.println("Gas Sensor: Active");
    Serial.println("Modbus RTU: Active");
    Serial.println("Display: Active");
    Serial.println("MODEST Pattern: Applied");
    Serial.println("==================");
  }
  else if(cmd == "calibrate") {
    Serial.println("Calibration command received");
    if(eventHandler) {
      EventHandler::Event evt = {EventHandler::CALIBRATION_COMPLETE, nullptr, millis()};
      eventHandler->handleEvent(evt);
    }
  }
  else if(cmd == "registers") {
    Serial.println("=== MODBUS REGISTERS ===");
    Serial.println("Use Modbus master to read registers");
    Serial.println("====================");
  }
  else if(cmd == "help") {
    init(); // Show help again
  }
  else {
    Serial.println("Unknown command: " + command);
    Serial.println("Type 'help' for available commands");
  }
}