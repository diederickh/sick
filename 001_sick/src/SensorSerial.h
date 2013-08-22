#ifndef SENSOR_SERIAL_H
#define SENSOR_SERIAL_H

#include "ofMain.h"

class SensorSerial {
 public:
  SensorSerial();
  void setup(std::string port, int baud);
  bool update();
 public:
  ofSerial serial;
  int nread;
  unsigned char buffer[12];
};
#endif
