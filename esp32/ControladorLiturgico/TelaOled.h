#ifndef TELA_OLED_H
#define TELA_OLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "CalendarioLiturgico.h"

// Exibe o resultado litúrgico no OLED SSD1306 de 128 por 64 pixels.
class TelaOled {
public:
  TelaOled();

  bool iniciar();
  void exibirDiaLiturgico(const DiaLiturgico& dia);
  void exibirErro(const char* mensagem);

private:
  static const uint8_t LARGURA_TELA = 128;
  static const uint8_t ALTURA_TELA = 64;
  static const uint8_t ENDERECO_I2C = 0x3C;
  static const uint8_t MAXIMO_DE_CARACTERES_POR_LINHA = 21;
  static const uint8_t ALTURA_DA_FAIXA_AMARELA = 16;
  static const uint8_t POSICAO_Y_DA_CELEBRACAO = ALTURA_DA_FAIXA_AMARELA + 4;
  static const uint8_t POSICAO_Y_DO_PERIODO = 39;
  static const uint8_t POSICAO_Y_DA_COR = 48;
  static const uint8_t MAXIMO_DE_LINHAS_DA_CELEBRACAO = 2;

  Adafruit_SSD1306 tela;

  void desenharLinhaCentralizada(const char* texto, uint8_t posicaoY, uint8_t tamanhoDoTexto);
  void desenharTextoQuebrado(const char* texto, uint8_t posicaoY);
  static void normalizarTextoParaOled(const char* origem, char* destino, size_t capacidade);
  static const char* obterAbreviacaoDoDia(DiaDaSemana diaDaSemana);
  static const char* obterNomeDoTempo(TempoLiturgico tempo);
  static const char* obterNomeDaCor(CorLiturgica cor);
};

#endif
