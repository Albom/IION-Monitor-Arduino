// Copyright © 2018 Stanislav Hnatiuk. All rights reserved.

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <OneWire.h>
#include "BaseSensor.h"



//! Класс датчиков температур. Наследуется от BaseSensor.
class Temperature : public BaseSensor {
  public:
    Temperature(const uint8_t seriesByte);
    
    //! Поиск датчиков в сети 1-wire, соответствующих указанной серии.
    void find();
    
    //! Вернуть адресс указанного датчика.
    char* getAddr(const uint8_t sensor) const override;
    
    //! Считать значение температуры и переключить на следующий датчик.
    void read() override;
    
    //! Запустить конвертацию температуры в датчике.
    void conversion();
    
  private:
    //! Преобразовать 8-битный адрес в 16-битную строку с HEX представлением.
    void addr2str(char* str, const uint8_t* addr);
    
    //! Сеть 1-wire.
    OneWire  ds;  // DS18b20 on pin 9 (a 4.7K resistor is necessary)
    
    //! Значение серии датчиков.
    uint8_t series;
};

#endif // TEMPERATURE_H
