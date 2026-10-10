#include "ConfiguracaoHardware.h"
#include "LeitorBotoes.h"

void LeitorBotoes::iniciar() {
  botoes[0] = { PINO_BOTAO_ROXO, HIGH, HIGH, 0, CorLiturgica::VIOLETA };
  botoes[1] = { PINO_BOTAO_VERMELHO, HIGH, HIGH, 0, CorLiturgica::VERMELHO };
  botoes[2] = { PINO_BOTAO_BRANCO, HIGH, HIGH, 0, CorLiturgica::BRANCO };
  botoes[3] = { PINO_BOTAO_VERDE, HIGH, HIGH, 0, CorLiturgica::VERDE };

  for (uint8_t indice = 0; indice < 4; indice++) {
    pinMode(botoes[indice].pino, INPUT_PULLUP);
  }
}

bool LeitorBotoes::obterCorSelecionada(CorLiturgica& corSelecionada) {
  for (uint8_t indice = 0; indice < 4; indice++) {
    if (foiPressionado(botoes[indice])) {
      corSelecionada = botoes[indice].cor;
      return true;
    }
  }

  return false;
}

bool LeitorBotoes::foiPressionado(EstadoDoBotao& botao) {
  const bool leituraAtual = digitalRead(botao.pino);
  const unsigned long instanteAtual = millis();

  if (leituraAtual != botao.leituraAnterior) {
    botao.instanteDaUltimaMudanca = instanteAtual;
    botao.leituraAnterior = leituraAtual;
  }

  if (instanteAtual - botao.instanteDaUltimaMudanca < TEMPO_DE_DEBOUNCE_EM_MILISSEGUNDOS) {
    return false;
  }

  if (leituraAtual == botao.estadoEstavel) {
    return false;
  }

  botao.estadoEstavel = leituraAtual;
  return botao.estadoEstavel == LOW;
}
