// Copyright © 2018 Stanislav Hnatiuk. All rights reserved.

#define SERIAL_ENABLED 0
#define TEMPERATURE_ENABLED 1
#define ENERGY_ENABLED 1
#define ETHERNET_ENABLED 1



#include "ThreadLite.h"



#if TEMPERATURE_ENABLED
#include "Temperature.h"
void temperatureUpdate();
Temperature tmpr = Temperature(0x28);
Thread temperatureT = Thread(temperatureUpdate, 1);
#endif // TEMPERATURE_ENABLED



#if ENERGY_ENABLED
#include "Energy.h"
void consumptionUpdate();
Energy enrg;
Thread consumptionT = Thread(consumptionUpdate, 150);
#endif // ENERGY_ENABLED



#if ETHERNET_ENABLED
#include "request.h"
#endif // ETHERNET_ENABLED



//! Настройка устройства.
void setup() {
#if SERIAL_ENABLED
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("Serial enabled.");
#endif // SERIAL_ENABLED

#if ETHERNET_ENABLED
  startServer();
#endif // ETHERNET_ENABLED

#if TEMPERATURE_ENABLED
  tmpr.find();
#endif // TEMPERATURE_ENABLED
}



//! Основная программа.
void loop() {
#if ETHERNET_ENABLED
  checkRequest();
#endif

#if TEMPERATURE_ENABLED
  temperatureT.run();
#endif // TEMPERATURE_ENABLED

#if ENERGY_ENABLED
  consumptionT.run();
#endif // ENERGY_ENABLED
}



#if ENERGY_ENABLED
//! Процесс обновления буфера потребления энергии.
void consumptionUpdate() {
  enrg.read();
}
#endif // ENERGY_ENABLED



#if TEMPERATURE_ENABLED
//! Процесс обновления буфера температур.
void temperatureUpdate() {
  static bool state = true;
  if (state) {
    // Режим преобразования. Длительность 750+ миллисекунд.
    temperatureT.setInterval(999);
    state = !state;
    tmpr.conversion();
  } else {
    // Режим считывания.
    temperatureT.setInterval(1);
    state = !state;
    tmpr.read();
  }
}
#endif // TEMPERATURE_ENABLED



#if ETHERNET_ENABLED
//! Вывод данных в ответ на запрос.
void responsePrint(EthernetClient& client, const BaseSensor* obj) {
  for (uint8_t i = 0; i < obj->getCount(); ++i) {
    client.print(obj->getAddr(i));
    client.print(" ");
    client.println(obj->getValue(i));
  }
}
#endif // ETHERNET_ENABLED



#if ETHERNET_ENABLED
//! Ответить на запрос.
void requestResponse(EthernetClient& client) {
  client.println("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Type: text/plain\n");

#if TEMPERATURE_ENABLED
  responsePrint(client, &tmpr);
#endif // TEMPERATURE_ENABLED

#if ENERGY_ENABLED
  responsePrint(client, &enrg);
#endif // ENERGY_ENABLED
}
#endif // ETHERNET_ENABLED
