#ifndef ENERGY_H
#define ENERGY_H

#include <SPI.h>
#include "BaseSensor.h"

#define N_PHASES 1



class Energy : public BaseSensor {
  public:
    Energy();
    void read() override;
};

#endif // ENERGY_H
