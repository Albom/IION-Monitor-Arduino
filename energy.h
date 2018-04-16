#ifndef ENERGY_H
#define ENERGY_H



// Глобальные переменные:

float getEnergy(){
  uint16_t current;
  float d;
  uint16_t min_ = 1024, max_ = 0;
  for (uint8_t i = 0; i < 200; ++i) {
    current =  analogRead(0);
    if (current > max_) {
      max_ = current;
    }
    if (current < min_) {
      min_ = current;
    }
  }
  d = (max_ - min_) / 2.0 * 0.707 * ((5.0 / 1024.0) * 220.0) * 10;
#if SERIAL_ENABLED
    Serial.println(d);
#endif
  return d;
}
#endif // ENERGY_H
