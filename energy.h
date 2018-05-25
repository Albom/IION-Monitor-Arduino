// Copyright © 2018 Stanislav Hnatiuk. All rights reserved.

#ifndef ENERGY_H
#define ENERGY_H

#include <SPI.h>
#include "BaseSensor.h"



class Filter;



//! Класс датчиков потребления энергии. Наследуется от BaseSensor.
class Energy : public BaseSensor {
  public:
    Energy();
    
    //! Считать значение потребления энергии и переключить на следующий датчик.
    void read() override;
    
  private:
    //! Фильтры для каждой фазы.
    Filter* filters;
    
    //! Значения корректирующих коэффициентов фаз.
    float* xratio;
    
    //! Множитель для вычисления потребляемой энергии.
    const float calc = 1.0 / (2.0 / 0.707 / 5.0 * 1024.0 / 220.0);
};



//! Класс для фильтрации и сглаживания энергопотребления по каждой фазе.
class Filter {
  public:
    Filter() {prev = 0; curr = 0;};

    //! Фильтровать и сглаживать скачки с задержкой в одну итерацию.
    uint16_t calc(const uint16_t next);
    
  private:
    uint16_t prev = 0;
    uint16_t curr = 0;
};

#endif // ENERGY_H
