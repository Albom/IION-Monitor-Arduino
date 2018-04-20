#ifndef ENERGY_H
#define ENERGY_H

#include <SPI.h>
#include "BaseSensor.h"



//! Класс датчиков потребления энергии. Наследуется от BaseSensor.
class Energy : public BaseSensor {
  public:
    Energy();
    
    //! Считать значение потребления энергии и переключить на следующий датчик.
    void read() override;
    
  private:
    //! Фильтровать и сглаживать скачки с задержкой в одну итерацию.
    uint16_t filter(const uint16_t next);
    
    //! Значения корректирующих коэффициентов фаз.
    float* xratio;
    
    //! Множитель для вычисления потребляемой энергии.
    const float calc = 1.0 / (2.0 / 0.707 / 5.0 * 1024.0 / 220.0);
};

#endif // ENERGY_H
