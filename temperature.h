#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <OneWire.h>

class Temperature {
  public:
    Temperature();
    void findSensors();
    char* get();
    void conversion();
    void read();
  private:
    void addr2str(char *str, const byte *addr);
    OneWire  ds;  // DS18b20 on pin 9 (a 4.7K resistor is necessary)
    static const uint8_t sensorsCount = 3;
    char output_buffer[32 * sensorsCount];
    char one_sensor_data[sensorsCount][32];
    byte addr[sensorsCount][8];
    byte data[12];
    uint8_t addrCount;
    uint8_t index;
};



#endif // TEMPERATURE_H
