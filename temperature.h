#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <OneWire.h>



#define SERIAL_ENABLED 0
#define MAXIMUM_SENSORS 10



class Temperature {
  public:
    Temperature();
    void findSensors();
    char* get(uint8_t n);
    void conversion();
    void read();
    uint8_t count;
    void addr2str(char *str, const uint8_t *addr);
    OneWire  ds;  // DS18b20 on pin 9 (a 4.7K resistor is necessary)
    uint8_t index;
    uint8_t** addr;
    float* data;
};

#endif // TEMPERATURE_H
