#include <Arduino.h>

#include "ConfiguracaoHardware.h"
#include "ControladorReles.h"

void ControladorReles::iniciar() {
  // Define o estado desligado antes de transformar os pinos em saídas.
  const uint8_t nivelParaDesligar = RELES_ATIVOS_EM_NIVEL_BAIXO ? HIGH : LOW;
  digitalWrite(PINO_RELE_ROXO, nivelParaDesligar);
  digitalWrite(PINO_RELE_VERMELHO, nivelParaDesligar);
  digitalWrite(PINO_RELE_BRANCO, nivelParaDesligar);
  digitalWrite(PINO_RELE_VERDE, nivelParaDesligar);

  pinMode(PINO_RELE_ROXO, OUTPUT);
  pinMode(PINO_RELE_VERMELHO, OUTPUT);
  pinMode(PINO_RELE_BRANCO, OUTPUT);
  pinMode(PINO_RELE_VERDE, OUTPUT);
}

void ControladorReles::aplicarCor(CorLiturgica cor) {
  desligarTodos();

  switch (cor) {
    case CorLiturgica::VIOLETA:
      definirEstadoDoRele(PINO_RELE_ROXO, true);
      break;
    case CorLiturgica::VERMELHO:
      definirEstadoDoRele(PINO_RELE_VERMELHO, true);
      break;
    case CorLiturgica::BRANCO:
      definirEstadoDoRele(PINO_RELE_BRANCO, true);
      break;
    case CorLiturgica::VERDE:
      definirEstadoDoRele(PINO_RELE_VERDE, true);
      break;
  }
}

void ControladorReles::desligarTodos() {
  definirEstadoDoRele(PINO_RELE_ROXO, false);
  definirEstadoDoRele(PINO_RELE_VERMELHO, false);
  definirEstadoDoRele(PINO_RELE_BRANCO, false);
  definirEstadoDoRele(PINO_RELE_VERDE, false);
}

void ControladorReles::definirEstadoDoRele(uint8_t pino, bool deveLigar) {
  const uint8_t nivelParaLigar = RELES_ATIVOS_EM_NIVEL_BAIXO ? LOW : HIGH;
  const uint8_t nivelParaDesligar = RELES_ATIVOS_EM_NIVEL_BAIXO ? HIGH : LOW;
  digitalWrite(pino, deveLigar ? nivelParaLigar : nivelParaDesligar);
}
