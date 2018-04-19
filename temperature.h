#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <OneWire.h>
#include "BaseSensor.h"



class Temperature : public BaseSensor {
  public:
    Temperature();
    void findSensors();
    char* getAddr(const uint8_t sensor) const override;
    void read() override;
    void conversion();
  private:
    void addr2str(char* str, const uint8_t* addr);
    OneWire  ds;  // DS18b20 on pin 9 (a 4.7K resistor is necessary)
};

#endif // TEMPERATURE_H
