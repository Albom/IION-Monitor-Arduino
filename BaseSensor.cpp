#include "BaseSensor.h"



BaseSensor::BaseSensor(): count(0), index(0) {}



//! Вернуть адресс указанного датчика.
char* BaseSensor::getAddr(const uint8_t sensor) const {
  return addr[sensor];
}



//! Вернуть значение указанного датчика.
float BaseSensor::getValue(const uint8_t sensor) const {
  return data[sensor];
}
