#ifndef LEITOR_BOTOES_H
#define LEITOR_BOTOES_H

#include <Arduino.h>

#include "TiposLiturgicos.h"

// Converte os quatro botões físicos em uma escolha de cor com debounce.
class LeitorBotoes {
public:
  void iniciar();
  bool obterCorSelecionada(CorLiturgica& corSelecionada);

private:
  struct EstadoDoBotao {
    uint8_t pino;
    bool leituraAnterior;
    bool estadoEstavel;
    unsigned long instanteDaUltimaMudanca;
    CorLiturgica cor;
  };

  static const unsigned long TEMPO_DE_DEBOUNCE_EM_MILISSEGUNDOS = 40;
  EstadoDoBotao botoes[4];

  static bool foiPressionado(EstadoDoBotao& botao);
};

#endif
