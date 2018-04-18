#include "Temperature.h"



Temperature::Temperature() : ds(9), count(0), index(0) {

}



//! Преобразовать восьмибитный адрес в шестнадцатеричную строку.
void Temperature::addr2str(char *str, const uint8_t *addr) {
  const char d[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  for (uint8_t i = 0; i < 8; ++i) {
    str[i * 2] = d[addr[i] / 16];
    str[i * 2 + 1] = d[addr[i] % 16];
  }
}



//! Поиск и сохранение адресов температурных датчиков.
void Temperature::findSensors() {
  uint8_t temp[8];
  while (ds.search(temp) && (count < MAXIMUM_SENSORS)) {
    ++count;
  }
  ds.reset_search();

  data = new float[count];
  addr = new uint8_t* [count];
  uint8_t i;
  for (i = 0; i < count; ++i) {
    addr[i] = new uint8_t[8];
  }
  for (i = 0; ds.search(addr[i]) && i < count; ++i) {
#if SERIAL_ENABLED
    char* str = new char[17];
    addr2str(str, addr[i]);
    str[16] = '\0';
    Serial.println(str);
    delete[] str;
#endif    
  }
  count = i;
}



//! Получить буфер указанной температуры.
char* Temperature::get(uint8_t n) {
  char* buf = new char[24];
  addr2str(buf, addr[n]);
  buf[16] = ' ';
  itoa(data[n], buf + 17, 10);
  buf[23] = '\0';
  return buf;
}



//! Преобразовать температуру датчика в буфер датчика.
void Temperature::conversion() {
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



//! Считывание температуры в буфер программы.
void Temperature::read() {
  if (OneWire::crc8(addr[index], 7) == addr[index][7]) {
    // Команда на считывание.
    ds.reset();
    ds.select(addr[index]);
    ds.write(0xBE);

    uint8_t temp[12];
    for (uint8_t i = 0; i < 9; ++i) {
      temp[i] = ds.read();
    }
    data[index] = ((temp[1] << 8) | temp[0]) / 16;

#if SERIAL_ENABLED
    char* str = get(index);
    Serial.println(str);
    delete[] str;
#endif
  }
  ++index;
}
