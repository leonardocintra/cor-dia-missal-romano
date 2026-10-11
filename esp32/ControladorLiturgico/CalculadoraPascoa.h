#ifndef CALCULADORA_PASCOA_H
#define CALCULADORA_PASCOA_H

#include <stdint.h>

#include "DataCivil.h"

// Calcula a data da Páscoa no calendário gregoriano.
class CalculadoraPascoa {
public:
  static DataCivil calcularDataDaPascoa(int16_t ano);
};

#endif
