#include "Temperature.h"



//! 
Temperature::Temperature() : ds(9), addrCount(0), index(0) {

}



//! Преобразовать восьмибитный адрес в шестнадцатеричную строку.
void Temperature::addr2str(char *str, const byte *addr) {
  const char d[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  for (byte i = 0; i < 8; ++i) {
    str[i * 2] = d[addr[i] / 16];
    str[i * 2 + 1] = d[addr[i] % 16];
  }
}



//! Поиск и сохранение адресов температурных датчиков.
void Temperature::findSensors() {
  while (ds.search(addr[addrCount]) && (addrCount < sensorsCount)) {
    addr2str(one_sensor_data[addrCount], addr[addrCount]);
    one_sensor_data[addrCount][16] = ' ';
    ++addrCount;
  }
}



//! Получить буфер температур.
char* Temperature::get() {
  output_buffer[0] = '\0';
  for (uint8_t i = 0; i < addrCount; ++i) {
    strcat(output_buffer, one_sensor_data[i]);
    strcat(output_buffer, "\n");
  }
  return output_buffer;
}



//! Преобразовать температуру датчика в буфер датчика.
void Temperature::conversion() {
  if (index == addrCount) {
    index = 0;
  }

  addr2str(one_sensor_data[index], addr[index]);

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

    // Считывание.
    for (uint8_t i = 0; i < 9; ++i) {
      data[i] = ds.read();
    }

    // Преобразование формата.
    int16_t raw = (data[1] << 8) | data[0];
    itoa(raw / 16, one_sensor_data[index] + 17, 10);

#if SERIAL_ENABLED
    Serial.println(one_sensor_data[index]);
#endif
  }
  ++index;
}
