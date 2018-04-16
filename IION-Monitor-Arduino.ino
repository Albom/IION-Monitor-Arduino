#define ETHERNET_ENABLED 1
#define SERIAL_ENABLED 1

#if ETHERNET_ENABLED
#include "request.h"
#endif

#include "temperature.h"



//! Настройка устройства.
void setup() {
#if SERIAL_ENABLED
  Serial.begin(9600); // Можно больше???
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif

  findTemperatureSensors();

#if ETHERNET_ENABLED
  startServer();
#endif
}



//! Основная программа.
void loop() {
  temperatureUpdate();
#if ETHERNET_ENABLED
  checkRequest();
#endif
#if SERIAL_ENABLED
  getEnergy();
#endif
}
