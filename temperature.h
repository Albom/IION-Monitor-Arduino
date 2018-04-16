#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <OneWire.h>



// Глобальные переменные:
OneWire  ds(9);  // DS18b20 on pin 9 (a 4.7K resistor is necessary)
const byte sensorsCount = 3;
char output_buffer[32 * sensorsCount];
char one_sensor_data[sensorsCount][32];
byte addr[sensorsCount][8];
byte data[12];
uint8_t addrCount = 0;



//! Преобразовать восьмибитный адрес в шестнадцатеричную строку.
void addr2str(char *str, const byte *addr) {
  const char d[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  for (byte i = 0; i < 8; ++i) {
    str[i * 2] = d[addr[i] / 16];
    str[i * 2 + 1] = d[addr[i] % 16];
  }
}



//! Поиск и сохранение адресов температурных датчиков.
void findTemperatureSensors() {
  while (ds.search(addr[addrCount]) && (addrCount < sensorsCount)) {
    addr2str(one_sensor_data[addrCount], addr[addrCount]);
    one_sensor_data[addrCount][16] = ' ';
    ++addrCount;
  }
}



//! Получить буфер температур.
char * getTemperature() {
  output_buffer[0] = '\0';
  for (uint8_t i = 0; i < addrCount; ++i) {
    strcat(output_buffer, one_sensor_data[i]);
    strcat(output_buffer, "\n");
  }
  return output_buffer;
}



//! Считать температуры с датчиков в буфер.
void temperatureUpdate() {
  static uint8_t index = 0;
  if (index == addrCount) {
    index = 0;
  }

  addr2str(one_sensor_data[index], addr[index]);

  if (OneWire::crc8(addr[index], 7) == addr[index][7]) {
    // Команда преобразования.
    ds.reset();
    ds.select(addr[index]);
    ds.write(0x44, 1);

    // Задержка 750+ милисекунд до завершения преобразования.
    delay(750); 

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

#endif // TEMPERATURE_H
