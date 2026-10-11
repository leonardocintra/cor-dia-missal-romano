#ifndef DATA_CIVIL_H
#define DATA_CIVIL_H

#include <stdint.h>

#include "TiposLiturgicos.h"

// Representa somente uma data do calendário gregoriano, sem horário ou fuso.
// Os valores recebidos pelo construtor devem formar uma data válida.
class DataCivil {
public:
  DataCivil();
  DataCivil(int16_t ano, uint8_t mes, uint8_t dia);

  bool ehValida() const;
  bool ehIgual(const DataCivil& outra) const;
  bool ehAnteriorA(const DataCivil& outra) const;
  bool ehAnteriorOuIgualA(const DataCivil& outra) const;
  bool ehPosteriorA(const DataCivil& outra) const;
  bool ehPosteriorOuIgualA(const DataCivil& outra) const;

  DiaDaSemana obterDiaDaSemana() const;
  DataCivil adicionarDias(int32_t quantidadeDeDias) const;
  int32_t diferencaEmDiasPara(const DataCivil& outra) const;

  static bool anoEhBissexto(int16_t ano);
  static uint8_t obterQuantidadeDeDiasNoMes(int16_t ano, uint8_t mes);

  int16_t ano;
  uint8_t mes;
  uint8_t dia;

private:
  int32_t obterNumeroDeDiasDesdeEpoca() const;
  static DataCivil criarAPartirDoNumeroDeDias(int32_t numeroDeDias);
};

#endif
