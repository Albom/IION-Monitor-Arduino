#ifndef BASESENSOR_H
#define BASESENSOR_H



#define SERIAL_ENABLED 0
#define MAXIMUM_SENSORS 10

#include <Arduino.h>



class BaseSensor {
  public:
    virtual char* getAddr(const uint8_t sensor) const;
    virtual float getValue(const uint8_t sensor) const;
    virtual void read() = 0;
    uint8_t getCount() const { return count; }
  protected:
    BaseSensor();
    uint8_t count;
    uint8_t index;
    char** addr;
    float* data;
};

#endif // BASESENSOR_H
