#ifndef CALENDARIO_LITURGICO_H
#define CALENDARIO_LITURGICO_H

#include "CalculadoraPascoa.h"

// Dados do período que envolve uma data civil.
struct PeriodoLiturgico {
  TempoLiturgico tempo;
  CorLiturgica corPadrao;
  DataCivil inicio;
  DataCivil fim;
};

// Celebração individual que pode ocorrer em um dia litúrgico.
struct CelebracaoLiturgica {
  const char* nome;
  TipoCelebracao tipo;
  CorLiturgica cor;
};

// Resultado completo do cálculo para uma data.
struct DiaLiturgico {
  static const uint8_t QUANTIDADE_MAXIMA_DE_CELEBRACOES = 2;

  DataCivil data;
  PeriodoLiturgico periodo;
  CelebracaoLiturgica celebracoes[QUANTIDADE_MAXIMA_DE_CELEBRACOES];
  uint8_t quantidadeDeCelebracoes;
  CorLiturgica cor;
};

// Determina os períodos litúrgicos a partir de uma data civil.
class CalendarioLiturgico {
public:
  PeriodoLiturgico obterPeriodoLiturgico(const DataCivil& data) const;
  DiaLiturgico obterDiaLiturgico(const DataCivil& data) const;
  CorLiturgica obterCorDoDia(const DataCivil& data) const;

  static DataCivil calcularInicioDoAdvento(int16_t ano);
  static DataCivil calcularQuartaFeiraDeCinzas(int16_t ano);

private:
  static PeriodoLiturgico criarPeriodo(
    TempoLiturgico tempo,
    CorLiturgica corPadrao,
    const DataCivil& inicio,
    const DataCivil& fim
  );
  static void adicionarCelebracao(
    DiaLiturgico& dia,
    const char* nome,
    TipoCelebracao tipo,
    CorLiturgica cor
  );
  static void ordenarCelebracoesPorPrecedencia(DiaLiturgico& dia);
  static const char* obterNomeDoDomingo(TempoLiturgico tempo);
  static const char* obterNomePadraoDoPeriodo(TempoLiturgico tempo);
};

#endif
