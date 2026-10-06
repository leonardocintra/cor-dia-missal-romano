#include "DataCivil.h"

DataCivil::DataCivil()
  : ano(2000), mes(1), dia(1) {
}

DataCivil::DataCivil(int16_t anoRecebido, uint8_t mesRecebido, uint8_t diaRecebido)
  : ano(anoRecebido), mes(mesRecebido), dia(diaRecebido) {
}

bool DataCivil::ehValida() const {
  return ano >= 1 && mes >= 1 && mes <= 12 && dia >= 1
    && dia <= obterQuantidadeDeDiasNoMes(ano, mes);
}

bool DataCivil::ehIgual(const DataCivil& outra) const {
  return ano == outra.ano && mes == outra.mes && dia == outra.dia;
}

bool DataCivil::ehAnteriorA(const DataCivil& outra) const {
  return obterNumeroDeDiasDesdeEpoca() < outra.obterNumeroDeDiasDesdeEpoca();
}

bool DataCivil::ehAnteriorOuIgualA(const DataCivil& outra) const {
  return ehAnteriorA(outra) || ehIgual(outra);
}

bool DataCivil::ehPosteriorA(const DataCivil& outra) const {
  return outra.ehAnteriorA(*this);
}

bool DataCivil::ehPosteriorOuIgualA(const DataCivil& outra) const {
  return ehPosteriorA(outra) || ehIgual(outra);
}

DiaDaSemana DataCivil::obterDiaDaSemana() const {
  // 01/01/1970 foi uma quinta-feira. A enumeração começa no domingo.
  int32_t indice = (obterNumeroDeDiasDesdeEpoca() + 4) % 7;
  if (indice < 0) {
    indice += 7;
  }

  return static_cast<DiaDaSemana>(indice);
}

DataCivil DataCivil::adicionarDias(int32_t quantidadeDeDias) const {
  return criarAPartirDoNumeroDeDias(obterNumeroDeDiasDesdeEpoca() + quantidadeDeDias);
}

int32_t DataCivil::diferencaEmDiasPara(const DataCivil& outra) const {
  return outra.obterNumeroDeDiasDesdeEpoca() - obterNumeroDeDiasDesdeEpoca();
}

bool DataCivil::anoEhBissexto(int16_t ano) {
  return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}

uint8_t DataCivil::obterQuantidadeDeDiasNoMes(int16_t ano, uint8_t mes) {
  static const uint8_t diasPorMes[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };

  if (mes < 1 || mes > 12) {
    return 0;
  }

  if (mes == 2 && anoEhBissexto(ano)) {
    return 29;
  }

  return diasPorMes[mes - 1];
}

int32_t DataCivil::obterNumeroDeDiasDesdeEpoca() const {
  // Algoritmo gregoriano que usa 01/01/1970 como dia zero.
  int32_t anoCalculado = ano;
  const uint8_t mesCalculado = mes;
  const uint8_t diaCalculado = dia;

  anoCalculado -= mesCalculado <= 2 ? 1 : 0;
  const int32_t era = (anoCalculado >= 0 ? anoCalculado : anoCalculado - 399) / 400;
  const uint32_t anoDaEra = static_cast<uint32_t>(anoCalculado - era * 400);
  const uint32_t diaDoAno = (153 * (mesCalculado + (mesCalculado > 2 ? -3 : 9)) + 2) / 5
    + diaCalculado - 1;
  const uint32_t diaDaEra = anoDaEra * 365 + anoDaEra / 4 - anoDaEra / 100 + diaDoAno;

  return era * 146097 + static_cast<int32_t>(diaDaEra) - 719468;
}

DataCivil DataCivil::criarAPartirDoNumeroDeDias(int32_t numeroDeDias) {
  // Inverso de obterNumeroDeDiasDesdeEpoca().
  numeroDeDias += 719468;
  const int32_t era = (numeroDeDias >= 0 ? numeroDeDias : numeroDeDias - 146096) / 146097;
  const uint32_t diaDaEra = static_cast<uint32_t>(numeroDeDias - era * 146097);
  const uint32_t anoDaEra = (diaDaEra - diaDaEra / 1460 + diaDaEra / 36524 - diaDaEra / 146096) / 365;
  int32_t anoCalculado = static_cast<int32_t>(anoDaEra) + era * 400;
  const uint32_t diaDoAno = diaDaEra - (365 * anoDaEra + anoDaEra / 4 - anoDaEra / 100);
  const uint32_t mesProvisorio = (5 * diaDoAno + 2) / 153;
  const uint8_t diaCalculado = static_cast<uint8_t>(diaDoAno - (153 * mesProvisorio + 2) / 5 + 1);
  const uint8_t mesCalculado = static_cast<uint8_t>(mesProvisorio + (mesProvisorio < 10 ? 3 : -9));

  anoCalculado += mesCalculado <= 2 ? 1 : 0;
  return DataCivil(static_cast<int16_t>(anoCalculado), mesCalculado, diaCalculado);
}
