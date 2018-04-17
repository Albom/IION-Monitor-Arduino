#ifndef ENERGY_H
#define ENERGY_H

#include <SPI.h>



class Energy {
  public:
    Energy();
    float get();
    void read();
  private:
    float consumptionCurrent;
};

#endif // ENERGY_H
