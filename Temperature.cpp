// Copyright © 2018 Stanislav Hnatiuk. All rights reserved.

#include "Temperature.h"



Temperature::Temperature(const uint8_t seriesByte) : BaseSensor(), ds(9), series(seriesByte) {}



//! Преобразовать 8-битный адрес в 16-битную строку с HEX представлением.
void Temperature::addr2str(char* str, const uint8_t* addr) {
  const char d[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  for (uint8_t i = 0; i < 8; ++i) {
    str[i * 2] = d[addr[i] / 16];
    str[i * 2 + 1] = d[addr[i] % 16];
  }
}



//! Поиск датчиков в сети 1-wire, соответствующих указанной серии.
void Temperature::find() {
  uint8_t temp[8];
  // Подсчитать колличество датчиков соответствующей серии.
  while (ds.search(temp) && (count < MAXIMUM_SENSORS)) {
    if (temp[0] == series) {
      ++count;
    }
  }
  
  ds.reset_search();

  data = new float[count];
  addr = new char* [count];
  for (uint8_t i = 0; i < count; ++i) {
    addr[i] = new char[8];
  }
  
  // Сохранение адресов датчиков соответствующей серии.
  uint8_t i = 0;
  while (ds.search(addr[i]) && i < count) {
    if (addr[i][0] == series) {
      ++i;
#if SERIAL_ENABLED
      char* str = new char[17];
      addr2str(str, addr[i]);
      str[16] = '\0';
      Serial.println(str);
      delete[] str;
#endif
    }
  }
}



//! Вернуть адресс указанного датчика.
char* Temperature::getAddr(const uint8_t sensor) const {
  static char* buf = new char[17];
  addr2str(buf, this->addr[sensor]);
  buf[16] = '\0';
  return buf;
}



//! Запустить конвертацию температуры в датчике.
void Temperature::conversion() {
  // Возврат индекса к началу, при достижении последнего датчика.
  if (index == count) {
    index = 0;
  }
  
  if (OneWire::crc8(addr[index], 7) == addr[index][7]) {
    // Команда преобразования.
    ds.reset();
    ds.select(addr[index]);
    ds.write(0x44, 1);
  }
}



//! Считать значение температуры и переключить на следующий датчик.
void Temperature::read() {
  if (OneWire::crc8(addr[index], 7) == addr[index][7]) {
    // Команда считывания.
    ds.reset();
    ds.select(addr[index]);
    ds.write(0xBE);

    // Считывание.
    uint8_t byte0 = ds.read();
    int16_t byte1 = ds.read();
    data[index] = ((byte1 << 8) + byte0) * 0.0625;   // 12 байт;

#if SERIAL_ENABLED
    Serial.println(data[index]);
#endif
  }
  
  // Переключение на следующий датчик.
  ++index;
}
