#include "SensorSerial.h"

SensorSerial::SensorSerial() {
}

void SensorSerial::setup(std::string port, int baud) {
  serial.setup(port, baud);
}

bool SensorSerial::update() {
  nread = serial.readBytes(buffer, sizeof(buffer));
  return nread;
}
