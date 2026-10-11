#include <Wire.h>
#include <RTClib.h>
#include "CalendarioLiturgico.h"
#include "ConfiguracaoHardware.h"
#include "ControladorReles.h"
#include "LeitorBotoes.h"
#include "TelaOled.h"

RTC_DS3231 rtc;
CalendarioLiturgico calendario;
ControladorReles controladorReles;
LeitorBotoes leitorBotoes;
TelaOled tela;

bool rtcEstaDisponivel = false;
bool telaEstaDisponivel = false;
bool existeDataExibida = false;
DataCivil ultimaDataExibida;
ModoDeOperacao modoDeOperacao = ModoDeOperacao::AUTOMATICO;
CorLiturgica corManual = CorLiturgica::VERDE;
unsigned long ultimaLeituraDoRelogio = 0;
char linhaSerial[40];
uint8_t tamanhoDaLinhaSerial = 0;

const unsigned long INTERVALO_DE_LEITURA_DO_RELOGIO_EM_MILISSEGUNDOS = 500;

void processarLinhaSerial(const char* linha) {
  int ano, mes, dia, hora, minuto, segundo;
  if (sscanf(linha, "DATA %d-%d-%d %d:%d:%d",
             &ano, &mes, &dia, &hora, &minuto, &segundo) == 6) {
    const DataCivil candidata(
      static_cast<int16_t>(ano),
      static_cast<uint8_t>(mes),
      static_cast<uint8_t>(dia)
    );

    if (candidata.ehValida() && hora >= 0 && hora < 24
        && minuto >= 0 && minuto < 60 && segundo >= 0 && segundo < 60) {
      rtc.adjust(DateTime(ano, mes, dia, hora, minuto, segundo));
      existeDataExibida = false;
      Serial.println("DS3231 ajustado.");
    } else {
      Serial.println("Data/hora invalida.");
    }
  } else {
    Serial.println("Formato: DATA YYYY-MM-DD HH:MM:SS");
  }
}

void lerComandoSerial() {
  while (Serial.available() > 0) {
    const char caractere = static_cast<char>(Serial.read());

    if (caractere == '\n' || caractere == '\r') {
      if (tamanhoDaLinhaSerial > 0) {
        linhaSerial[tamanhoDaLinhaSerial] = '\0';
        processarLinhaSerial(linhaSerial);
        tamanhoDaLinhaSerial = 0;
      }
    } else if (tamanhoDaLinhaSerial < sizeof(linhaSerial) - 1) {
      linhaSerial[tamanhoDaLinhaSerial++] = caractere;
    }
  }
}

void atualizarSaidaAutomatica(const DataCivil& data) {
  const DiaLiturgico dia = calendario.obterDiaLiturgico(data);
  controladorReles.aplicarCor(dia.cor);

  if (telaEstaDisponivel) {
    tela.exibirDiaLiturgico(dia, ModoDeOperacao::AUTOMATICO, dia.cor);
  }

  Serial.print("MODO AUTO - CELEBRACAO: ");
  Serial.println(dia.celebracoes[0].nome);
}

void atualizarSaidaManual(const DataCivil& data, CorLiturgica cor) {
  const DiaLiturgico dia = calendario.obterDiaLiturgico(data);
  modoDeOperacao = ModoDeOperacao::MANUAL;
  corManual = cor;
  controladorReles.aplicarCor(corManual);

  if (telaEstaDisponivel) {
    tela.exibirDiaLiturgico(dia, ModoDeOperacao::MANUAL, corManual);
  }

  Serial.println("MODO MANUAL - RELE SELECIONADO");
}

void setup() {
  Serial.begin(115200);
  controladorReles.iniciar();
  leitorBotoes.iniciar();
  Wire.begin(PINO_I2C_SDA, PINO_I2C_SCL);
  Wire.setClock(400000);

  telaEstaDisponivel = tela.iniciar();
  if (!telaEstaDisponivel) {
    Serial.println("Erro ao iniciar o OLED");
  }

  rtcEstaDisponivel = rtc.begin();
  if (!rtcEstaDisponivel) {
    Serial.println("Erro ao iniciar o DS3231");

    if (telaEstaDisponivel) {
      tela.exibirErro("DS3231 AUSENTE");
    }

    return;
  }

  if (rtc.lostPower()) {
    Serial.println("DS3231 perdeu a hora; use o comando: DATA YYYY-MM-DD HH:MM:SS");
  }
}

void loop() {
  if (telaEstaDisponivel) {
    tela.atualizarRolagem();
  }

  if (!rtcEstaDisponivel) {
    return;
  }

  lerComandoSerial();

  CorLiturgica corSelecionada;
  if (leitorBotoes.obterCorSelecionada(corSelecionada)) {
    const DateTime agora = rtc.now();
    const DataCivil data(agora.year(), agora.month(), agora.day());
    atualizarSaidaManual(data, corSelecionada);
  }

  const unsigned long instanteAtual = millis();
  if (instanteAtual - ultimaLeituraDoRelogio < INTERVALO_DE_LEITURA_DO_RELOGIO_EM_MILISSEGUNDOS) {
    return;
  }

  ultimaLeituraDoRelogio = instanteAtual;
  DateTime agora = rtc.now();
  const DataCivil data(agora.year(), agora.month(), agora.day());

  if (!existeDataExibida || !data.ehIgual(ultimaDataExibida)) {
    // A mudança de data encerra qualquer escolha manual e restaura o calendário.
    modoDeOperacao = ModoDeOperacao::AUTOMATICO;
    atualizarSaidaAutomatica(data);
    ultimaDataExibida = data;
    existeDataExibida = true;
  }
}
