// Copyright © 2018 Stanislav Hnatiuk. All rights reserved.

#ifndef BASESENSOR_H
#define BASESENSOR_H

#include <Arduino.h>



#define MAXIMUM_SENSORS 10




// Базовый класс датчиков.
class BaseSensor {
  public:
    //! Вернуть адресс указанного датчика.
    virtual char* getAddr(const uint8_t sensor) const;
    
    //! Вернуть значение указанного датчика.
    virtual float getValue(const uint8_t sensor) const;
    
    //! Считать значение датчика.
    virtual void read() = 0;
    
    //! Вернуть количество датчиков
    uint8_t getCount() const { return count; }
    
  protected:
    BaseSensor();
    
    //! Количество датчиков.
    uint8_t count;
    
    //! Номер текущего датчика.
    uint8_t index;
    
    //! Адреса датчиков.
    char** addr;
    
    //! Значения датчиков.
    float* data;
};

#endif // BASESENSOR_H
