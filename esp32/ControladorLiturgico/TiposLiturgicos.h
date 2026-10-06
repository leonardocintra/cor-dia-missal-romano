#ifndef TIPOS_LITURGICOS_H
#define TIPOS_LITURGICOS_H

#include <stdint.h>

// Cores previstas no Missal Romano e usadas pelo controlador de relés.
enum class CorLiturgica : uint8_t {
  VIOLETA,
  BRANCO,
  VERMELHO,
  VERDE,
  ROSA
};

// Grau que define a precedência entre celebrações do mesmo dia.
enum class TipoCelebracao : uint8_t {
  SEMANA = 1,
  MEMORIA = 2,
  DOMINGO = 3,
  FESTA = 4,
  SOLENIDADE = 5
};

enum class TempoLiturgico : uint8_t {
  ADVENTO,
  NATAL,
  TEMPO_COMUM,
  QUARESMA,
  SEMANA_SANTA,
  PASCOA,
  TEMPO_PASCAL
};

// A semana começa no domingo para coincidir com o DS3231 e as regras do calendário.
enum class DiaDaSemana : uint8_t {
  DOMINGO = 0,
  SEGUNDA_FEIRA = 1,
  TERCA_FEIRA = 2,
  QUARTA_FEIRA = 3,
  QUINTA_FEIRA = 4,
  SEXTA_FEIRA = 5,
  SABADO = 6
};

#endif
