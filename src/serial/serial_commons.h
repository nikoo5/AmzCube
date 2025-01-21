#ifndef SERIAL_COMMONS_H_
#define SERIAL_COMMONS_H_

#define SERIAL_SPEED 115200

#define DEBUG_ERROR true
#define DEBUG_WARNING true
#define DEBUG_INFO true

#include "amzcube_commons.h"

class SerialInfo
{
public:
  inline SerialInfo() {}

  inline void print(const char *message = "")
  {
#if DEBUG_INFO == true
    Serial.print(message);
#endif
  }
  inline void print(const String message)
  {
    print(message.c_str());
  }

  inline void println(const char *message = "")
  {
#if DEBUG_INFO == true
    Serial.println(message);
#endif
  }
  inline void println(const String message)
  {
    println(message.c_str());
  }

  inline void printf(const char *format, ...)
  {
#if DEBUG_INFO == true
    va_list args;
    va_start(args, format);
    char buf[256];
    vsnprintf(buf, 256, format, args);
    Serial.print(buf);
    va_end(args);
#endif
  }
};

class SerialWarning
{
public:
  inline SerialWarning() {}

  inline void print(const char *message = "")
  {
#if DEBUG_WARNING == true
    Serial.print(message);
#endif
  }
  inline void print(const String message)
  {
    print(message.c_str());
  }

  inline void println(const char *message = "")
  {
#if DEBUG_WARNING == true
    Serial.println(message);
#endif
  }
  inline void println(const String message)
  {
    println(message.c_str());
  }

  inline void printf(const char *format, ...)
  {
#if DEBUG_WARNING == true
    va_list args;
    va_start(args, format);
    char buf[256];
    vsnprintf(buf, 256, format, args);
    Serial.print(buf);
    va_end(args);
#endif
  }
};

class SerialError
{
public:
  inline SerialError() {}

  inline void print(const char *message = "")
  {
#if DEBUG_ERROR == true
    Serial.print(message);
#endif
  }
  inline void print(const String message)
  {
    print(message.c_str());
  }

  inline void println(const char *message = "")
  {
#if DEBUG_ERROR == true
    Serial.println(message);
#endif
  }
  inline void println(const String message)
  {
    println(message.c_str());
  }

  inline void printf(const char *format, ...)
  {
#if DEBUG_ERROR == true
    va_list args;
    va_start(args, format);
    char buf[256];
    vsnprintf(buf, 256, format, args);
    Serial.print(buf);
    va_end(args);
#endif
  }
};

extern SerialInfo info;
extern SerialWarning warning;
extern SerialError error;

void SerialSetup();

#endif // SERIAL_COMMONS_H_