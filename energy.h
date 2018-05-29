// Copyright © 2018 Stanislav Hnatiuk. All rights reserved.

#ifndef ENERGY_H
#define ENERGY_H

#include <SPI.h>
#include "BaseSensor.h"



#define N_PHASES 3



class Filter;



//! Класс датчиков потребления энергии. Наследуется от BaseSensor.
class Energy : public BaseSensor {
  public:
    Energy();
    
    //! Считать значение потребления энергии и переключить на следующий датчик.
    void read() override;
    
  private:
    //! Сдвиг пина
    static const uint8_t pinShift = 1;

    //! Фильтры для каждой фазы.
    Filter* filters;
    
    //! Множитель для вычисления потребляемой энергии.
    static constexpr float calc = (1.0 / (2.0 / 0.707 / 5.0 * 1024.0 / 220.0)) * 10;
};



//! Класс для фильтрации и сглаживания энергопотребления по каждой фазе.
class Filter {
  public:
    Filter() : prev(0), curr(0), ratio(1) {};

    //! Фильтровать и сглаживать скачки с задержкой в одну итерацию.
    float filtering(const uint16_t next);

    //! Корректирующий коэффициент фаз.
    float ratio;
  private:
    uint16_t prev = 0;
    uint16_t curr = 0;  
};

#endif // ENERGY_H
