#define ETHERNET_ENABLED 1
#define SERIAL_ENABLED !ETHERNET_ENABLED

#include <StaticThreadController.h>
#include "Energy.h"
#include "Temperature.h"

#if ETHERNET_ENABLED
#include "request.h"
#endif



Temperature tmpr;
Energy enrg;

float getEnergy();
char* getTemperature();

void consumptionUpdate();
void temperatureUpdate();

Thread temperatureT = Thread(temperatureUpdate);
Thread consumptionT = Thread(consumptionUpdate);
StaticThreadController<2> threads(&temperatureT, &consumptionT);



//! Настройка устройства.
void setup() {
#if SERIAL_ENABLED
  Serial.begin(9600); // Можно больше???
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif

#if ETHERNET_ENABLED
  startServer();
#endif

  temperatureT.setInterval(49);
  consumptionT.setInterval(499);
  tmpr.findSensors();
}



//! Основная программа.
void loop() {
#if ETHERNET_ENABLED
  checkRequest();
#endif
  threads.run();
}



//! Получить потребление для реквеста.
float getEnergy() {
  return enrg.get();
}



//! Получить температуры для реквеста.
char* getTemperature() {
  return tmpr.get();
}



//! Процесс обновления текущего потребления энергии.
void consumptionUpdate() {
  enrg.read();
}



//! Процесс обновления значений буфера температур.
void temperatureUpdate() {
  static bool state = true;
  if (state) {
    state = !state;
    tmpr.conversion();
    temperatureT.setInterval(751);
  } else {
    state = !state;
    tmpr.read();
    temperatureT.setInterval(49);
  }
}
