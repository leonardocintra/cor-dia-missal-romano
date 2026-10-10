#include "TelaOled.h"

#include <stdio.h>
#include <string.h>

TelaOled::TelaOled()
  : tela(LARGURA_TELA, ALTURA_TELA, &Wire, -1),
    larguraDoTextoDaCelebracao(0),
    deslocamentoDaRolagem(0),
    instanteDaUltimaRolagem(0),
    rolagemEstaAtiva(false) {
  textoDaCelebracao[0] = '\0';
}

bool TelaOled::iniciar() {
  if (!tela.begin(SSD1306_SWITCHCAPVCC, ENDERECO_I2C)) {
    return false;
  }
  tela.setTextWrap(false);  // texto rolante é recortado, nunca deslocado para baixo
  return true;
}

void TelaOled::exibirDiaLiturgico(
  const DiaLiturgico& dia,
  ModoDeOperacao modoDeOperacao,
  CorLiturgica corExibida
) {
  char cabecalho[20];
  snprintf(
    cabecalho,
    sizeof(cabecalho),
    "%02d/%02d/%04d - %s",
    dia.data.dia,
    dia.data.mes,
    dia.data.ano,
    obterAbreviacaoDoDia(dia.data.obterDiaDaSemana())
  );

  normalizarTextoParaOled(
    dia.celebracoes[0].nome,
    textoDaCelebracao,
    sizeof(textoDaCelebracao)
  );
  larguraDoTextoDaCelebracao = strlen(textoDaCelebracao) * 6;
  deslocamentoDaRolagem = 0;
  instanteDaUltimaRolagem = millis();
  rolagemEstaAtiva = larguraDoTextoDaCelebracao > LARGURA_TELA;

  char nomeDoTempo[32];
  normalizarTextoParaOled(obterNomeDoTempo(dia.periodo.tempo), nomeDoTempo, sizeof(nomeDoTempo));

  tela.clearDisplay();
  tela.setTextColor(SSD1306_WHITE);
  tela.setTextSize(1);
  tela.setCursor(0, 0);
  tela.print(cabecalho);
  tela.setCursor(108, 0);
  tela.print(modoDeOperacao == ModoDeOperacao::AUTOMATICO ? "[A]" : "[M]");
  desenharCelebracao();
  desenharLinhaCentralizada(nomeDoTempo, POSICAO_Y_DO_PERIODO, 1);
  desenharLinhaCentralizada(obterNomeDaCor(corExibida), POSICAO_Y_DA_COR, 2);
  tela.display();
}

void TelaOled::atualizarRolagem() {
  if (!rolagemEstaAtiva) {
    return;
  }

  const unsigned long instanteAtual = millis();
  if (instanteAtual - instanteDaUltimaRolagem < INTERVALO_DA_ROLAGEM_EM_MILISSEGUNDOS) {
    return;
  }

  instanteDaUltimaRolagem = instanteAtual;
  deslocamentoDaRolagem++;

  if (deslocamentoDaRolagem > larguraDoTextoDaCelebracao + ESPACO_ENTRE_REPETICOES) {
    deslocamentoDaRolagem = 0;
  }

  tela.fillRect(
    0,
    POSICAO_Y_DA_CELEBRACAO,
    LARGURA_TELA,
    ALTURA_DA_LINHA_DA_CELEBRACAO,
    SSD1306_BLACK
  );
  desenharCelebracao();
  tela.display();
}

void TelaOled::exibirErro(const char* mensagem) {
  tela.clearDisplay();
  tela.setTextColor(SSD1306_WHITE);
  desenharLinhaCentralizada("ERRO", 16, 2);
  desenharLinhaCentralizada(mensagem, 40, 1);
  tela.display();
}

void TelaOled::desenharLinhaCentralizada(const char* texto, uint8_t posicaoY, uint8_t tamanhoDoTexto) {
  const int16_t larguraDoTexto = strlen(texto) * 6 * tamanhoDoTexto;
  const int16_t posicaoX = (LARGURA_TELA - larguraDoTexto) / 2;

  tela.setTextSize(tamanhoDoTexto);
  tela.setCursor(posicaoX > 0 ? posicaoX : 0, posicaoY);
  tela.print(texto);
}

void TelaOled::desenharCelebracao() {
  tela.setTextSize(1);

  if (!rolagemEstaAtiva) {
    desenharLinhaCentralizada(textoDaCelebracao, POSICAO_Y_DA_CELEBRACAO, 1);
    return;
  }

  const int16_t posicaoX = -deslocamentoDaRolagem;
  tela.setCursor(posicaoX, POSICAO_Y_DA_CELEBRACAO);
  tela.print(textoDaCelebracao);

  if (posicaoX + larguraDoTextoDaCelebracao < LARGURA_TELA) {
    tela.setCursor(
      posicaoX + larguraDoTextoDaCelebracao + ESPACO_ENTRE_REPETICOES,
      POSICAO_Y_DA_CELEBRACAO
    );
    tela.print(textoDaCelebracao);
  }
}

void TelaOled::normalizarTextoParaOled(const char* origem, char* destino, size_t capacidade) {
  size_t indiceDaOrigem = 0;
  size_t indiceDoDestino = 0;

  while (origem[indiceDaOrigem] != '\0' && indiceDoDestino + 1 < capacidade) {
    const uint8_t caractere = static_cast<uint8_t>(origem[indiceDaOrigem]);

    if (caractere == 0xC3 && origem[indiceDaOrigem + 1] != '\0') {
      const uint8_t acento = static_cast<uint8_t>(origem[indiceDaOrigem + 1]);
      char substituto = '?';

      switch (acento) {
        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: substituto = 'A'; break;
        case 0x87: substituto = 'C'; break;
        case 0x89: case 0x8A: substituto = 'E'; break;
        case 0x8D: substituto = 'I'; break;
        case 0x93: case 0x94: case 0x95: substituto = 'O'; break;
        case 0x9A: substituto = 'U'; break;
        case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: substituto = 'a'; break;
        case 0xA7: substituto = 'c'; break;
        case 0xA9: case 0xAA: substituto = 'e'; break;
        case 0xAD: substituto = 'i'; break;
        case 0xB3: case 0xB4: case 0xB5: substituto = 'o'; break;
        case 0xBA: substituto = 'u'; break;
      }

      destino[indiceDoDestino++] = substituto;
      indiceDaOrigem += 2;
      continue;
    }

    destino[indiceDoDestino++] = origem[indiceDaOrigem++];
  }

  destino[indiceDoDestino] = '\0';
}

const char* TelaOled::obterAbreviacaoDoDia(DiaDaSemana diaDaSemana) {
  switch (diaDaSemana) {
    case DiaDaSemana::DOMINGO: return "DOM";
    case DiaDaSemana::SEGUNDA_FEIRA: return "SEG";
    case DiaDaSemana::TERCA_FEIRA: return "TER";
    case DiaDaSemana::QUARTA_FEIRA: return "QUA";
    case DiaDaSemana::QUINTA_FEIRA: return "QUI";
    case DiaDaSemana::SEXTA_FEIRA: return "SEX";
    case DiaDaSemana::SABADO: return "SAB";
  }

  return "---";
}

const char* TelaOled::obterNomeDoTempo(TempoLiturgico tempo) {
  switch (tempo) {
    case TempoLiturgico::ADVENTO: return "Advento";
    case TempoLiturgico::NATAL: return "Tempo de Natal";
    case TempoLiturgico::TEMPO_COMUM: return "Tempo Comum";
    case TempoLiturgico::QUARESMA: return "Quaresma";
    case TempoLiturgico::SEMANA_SANTA: return "Semana Santa";
    case TempoLiturgico::PASCOA: return "Páscoa";
    case TempoLiturgico::TEMPO_PASCAL: return "Tempo Pascal";
  }

  return "";
}

const char* TelaOled::obterNomeDaCor(CorLiturgica cor) {
  switch (cor) {
    case CorLiturgica::VIOLETA: return "VIOLETA";
    case CorLiturgica::BRANCO: return "BRANCO";
    case CorLiturgica::VERMELHO: return "VERMELHO";
    case CorLiturgica::VERDE: return "VERDE";
  }

  return "";
}
