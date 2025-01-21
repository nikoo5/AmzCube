#include "serial_commons.h"

SerialInfo info;
SerialError error;
SerialWarning warning;

void SerialSetup()
{
#if DEBUG_ERROR == true || DEBUG_WARNING == true || DEBUG_INFO == true
  Serial.begin(SERIAL_SPEED);
  info.println("[Serial] Serial port initialized");
#endif
}
