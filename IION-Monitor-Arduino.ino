#define ETHERNET_ENABLED 1
#define SERIAL_ENABLED !ETHERNET_ENABLED

#include <StaticThreadController.h>
#include "energy.h"
#include "temperature.h"

#if ETHERNET_ENABLED
#include "request.h"
#endif



// Обьявления
void temperatureUpdate();
#define consumptionUpdate consumptionRead



// Глобальные переменные
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
  findTemperatureSensors();
}



//! Основная программа.
void loop() {
#if ETHERNET_ENABLED
  checkRequest();
#endif
  threads.run();
}



//! Процесс обновления значений буфера температур.
void temperatureUpdate() {
  static bool state = true;
  if (state) {
    state = !state;
    temperatureConversion();
    temperatureT.setInterval(751);
  } else {
    state = !state;
    temperatureRead();
    temperatureT.setInterval(49);
  }
}
