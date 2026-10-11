#include "CalculadoraPascoa.h"

DataCivil CalculadoraPascoa::calcularDataDaPascoa(int16_t ano) {
  // Algoritmo gregoriano de Meeus/Jones/Butcher, equivalente ao da prova de conceito.
  const int32_t a = ano % 19;
  const int32_t b = ano / 100;
  const int32_t c = ano % 100;
  const int32_t d = b / 4;
  const int32_t e = b % 4;
  const int32_t f = (b + 8) / 25;
  const int32_t g = (b - f + 1) / 3;
  const int32_t h = (19 * a + b - d - g + 15) % 30;
  const int32_t i = c / 4;
  const int32_t k = c % 4;
  const int32_t l = (32 + 2 * e + 2 * i - h - k) % 7;
  const int32_t m = (a + 11 * h + 22 * l) / 451;
  const int32_t mes = (h + l - 7 * m + 114) / 31;
  const int32_t dia = ((h + l - 7 * m + 114) % 31) + 1;

  return DataCivil(ano, static_cast<uint8_t>(mes), static_cast<uint8_t>(dia));
}
