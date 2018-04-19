#ifndef ENERGY_H
#define ENERGY_H

#include <SPI.h>
#include "BaseSensor.h"

#define N_PHASES 1



class Energy : public BaseSensor {
  public:
    Energy();
    void read() override;
  private:
    uint16_t filter(const uint16_t next);
    float* xratio;
    const float calc = 1.0 / (2.0 / 0.707 / 5.0 * 1024.0 / 220.0);
};

#endif // ENERGY_H
