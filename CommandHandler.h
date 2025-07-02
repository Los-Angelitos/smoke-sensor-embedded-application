#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "Device.h"
#include "EventHandler.h"

class CommandHandler : public BaseDevice {
private:
  String inputBuffer;
  SystemEventHandler* eventHandler;
  
public:
  CommandHandler();
  void init() override;
  void update() override;
  void setEventHandler(SystemEventHandler* handler);
  void processCommands();
  void handleCommand(const String& command);
};

#endif