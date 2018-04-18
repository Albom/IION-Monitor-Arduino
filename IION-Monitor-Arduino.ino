#define SERIAL_ENABLED 0

#include "ThreadLite.h"
#include "Energy.h"
#include "Temperature.h"



#define ETHERNET_ENABLED !SERIAL_ENABLED

#if ETHERNET_ENABLED
#include "request.h"
#endif



Temperature tmpr;
Energy enrg;

float getEnergy();
char* getTemperature();

void consumptionUpdate();
void temperatureUpdate();

Thread temperatureT = Thread(temperatureUpdate, 1);
Thread consumptionT = Thread(consumptionUpdate, 500);



//! Настройка устройства.
void setup() {
#if SERIAL_ENABLED
  Serial.begin(9600); // Можно больше???
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Serial");
#endif

#if ETHERNET_ENABLED
  startServer();
#endif

  tmpr.findSensors();
}



//! Основная программа.
void loop() {
#if ETHERNET_ENABLED
  checkRequest();
#endif
  if (temperatureT.shouldRun()) {
    temperatureT.run();
  }
  if (consumptionT.shouldRun()) {
    consumptionT.run();
  }
}



//! Процесс обновления текущего потребления энергии.
void consumptionUpdate() {
  enrg.read();
}



//! Процесс обновления значений буфера температур.
void temperatureUpdate() {
  static bool state = true;
  if (state) {
    temperatureT.setInterval(750);
    state = !state;
    tmpr.conversion();
  } else {
    temperatureT.setInterval(1);
    state = !state;
    tmpr.read();
  }
}



#if ETHERNET_ENABLED
//! Вывод данных в ответ на запрос.
void responsePrint(EthernetClient& client, Temperature& obj) {
  char* temp;
  for (uint8_t i = 0; i < tmpr.count; ++i) {
    temp = obj.get(i);
    client.println(temp);
    delete[] temp;
  }
}



//! Ответить на запрос.
void requestResponse(EthernetClient& client) {
#if SERIAL_ENABLED
  Serial.println("Request");
#endif
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/plain"));
  client.println(F("Connection: close"));
  client.println();

  responsePrint(client, tmpr);
  client.print("5000000000000001 ");
  client.println(enrg.get());
}
#endif
