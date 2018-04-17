#include "Temperature.h"



//! 
Temperature::Temperature() : ds(9), count(0), index(0){

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
  byte temp[8];
  while (ds.search(temp) && (count < MAXIMUM_SENSORS)) {
    ++count;
  }

  ds.reset_search();
  data = new int8_t[count];
  addr = new byte* [count];
  uint8_t i = 0;
  for (i = 0; i < count; ++i) {
    addr[i] = new byte[8];
  }
  i = 0;
  while (ds.search(addr[i]) && (i < MAXIMUM_SENSORS)) {
    char str[8];
    addr2str(str, addr[i]);
#if SERIAL_ENABLED
    Serial.println(str);
#endif     
    ++i;
  }
}



//! Получить буфер температур.
char* Temperature::get() {
  char str[20];
  char* temp = new char[count * 20]; 
  for (uint8_t i = 0; i < count; ++i) {
    addr2str(str, addr[i]);
    str[16] = " ";
    itoa(data[i], str + 17, 10);
    strcat(temp, str);
    strcat(temp, "\n");
  }
  return temp;
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
    
    // Считывание.
    byte temp[12];
    for (uint8_t i = 0; i < 9; ++i) {
      temp[i] = ds.read();
    }
    data[index] = ((temp[1] << 8) | temp[0]) / 16;
    // Преобразование формата.
    //int16_t raw = (temp[1] << 8) | temp[0];
    //itoa(raw / 16, one_sensor_data[index] + 17, 10);

#if SERIAL_ENABLED
    Serial.println(data[index]);
#endif
  }
  ++index;
}
