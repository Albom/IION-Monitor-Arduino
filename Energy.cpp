// Copyright © 2018 Stanislav Hnatiuk. All rights reserved.

#include "Energy.h"



Energy::Energy() : BaseSensor() {
  count = N_PHASES;
  data = new float[count];
  // xratio = new float[count];
  filters = new Filter[count];
  addr = new char* [count];
  for (uint8_t i = 0; i < count; ++i) {
    addr[i] = new char[4];
  }
  
  // Задание адресов и корректирующих коэффициентов.
  addr[0] = "5001";
  filters[0].ratio = 1.0533624791050533624791050533625;
#if N_PHASES > 1
  addr[1] = "5002";
  filters[1].ratio = 1.0533624791050533624791050533625;
#if N_PHASES > 2
  addr[2] = "5003";
  filters[2].ratio = 1.0533624791050533624791050533625;
#endif
#endif
}



//! Считать значение потребления энергии и переключить на следующий датчик.
void Energy::read() {
  // Возврат индекса к началу, при достижении последнего датчика.
  if (index == count) {
    index = 0;
  }
  
  // Измерение амплитуд напряжения.
  uint16_t current;
  uint16_t min_ = 1024, max_ = 0;
  for (uint8_t i = 0; i < 200; ++i) {
    current =  analogRead(index + pinShift);
    if (current > max_) {
      max_ = current;
    }
    if (current < min_) {
      min_ = current;
    }
  }
  
  // Вычисление значения потребляемой энергии.
  data[index] = filters[index].filtering(max_ - min_) * calc;
  
#if SERIAL_ENABLED
  Serial.println(data[index]);
#endif

  // Переключение на следующий датчик.
  ++index;
}



//! Фильтровать и сглаживать скачки с задержкой в одну итерацию.
float Filter::filtering(const uint16_t next) {
  if (curr == prev) {
    if (next < 3 && next == curr)
      curr = 0;
    else
      // Пороговый фильтр.
      curr = 0.7 * curr + 0.3 * next;
  } else {
    // Фильтр сглаживания.
    prev = curr;
    curr = 0.4 * curr + 0.6 * next;
  }
  return curr * ratio;
}
