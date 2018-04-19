#include "Energy.h"



#if N_PHASES > MAXIMUM_SENSORS
#define N_PHASES MAXIMUM_SENSORS
#endif



Energy::Energy() : BaseSensor() {
  count = N_PHASES;
  data = new float[count];
  addr = new char* [count];
  for (uint8_t i = 0; i < count; ++i) {
    addr[i] = new char[16];
  }
  addr[0] = "5000000000000001";
#if N_PHASES > 1
  addr[1] = "5000000000000002";
#if N_PHASES > 2
  addr[2] = "5000000000000003";
#endif
#endif
}



//! Процесс обновления значения текущего потребления энергии.
void Energy::read() {
  if (index == count) {
    index = 0;
  }
  uint16_t current;
  uint16_t min_ = 1024, max_ = 0;
  for (uint8_t i = 0; i < 200; ++i) {
    current =  analogRead(index);
    if (current > max_) {
      max_ = current;
    }
    if (current < min_) {
      min_ = current;
    }
  }
  data[index] = (max_ - min_) / 2.0 * 0.707 * ((5.0 / 1024.0) * 220.0) * 10;
#if SERIAL_ENABLED
  Serial.println(data[index]);
#endif
  ++index;
}
