#include <Wire.h>
#include <RTClib.h>
#include "CalendarioLiturgico.h"
#include "TelaOled.h"

RTC_DS3231 rtc;
CalendarioLiturgico calendario;
TelaOled tela;

bool rtcEstaDisponivel = false;
bool telaEstaDisponivel = false;
bool existeDataExibida = false;
DataCivil ultimaDataExibida;
unsigned long ultimaLeituraDoRelogio = 0;

const unsigned long INTERVALO_DE_LEITURA_DO_RELOGIO_EM_MILISSEGUNDOS = 500;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

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
    Serial.println("O DS3231 perdeu a hora e precisa ser ajustado");
  }
}

void loop() {
  if (!rtcEstaDisponivel) {
    return;
  }

  const unsigned long instanteAtual = millis();
  if (instanteAtual - ultimaLeituraDoRelogio < INTERVALO_DE_LEITURA_DO_RELOGIO_EM_MILISSEGUNDOS) {
    return;
  }

  ultimaLeituraDoRelogio = instanteAtual;
  DateTime agora = rtc.now();
  const DataCivil data(agora.year(), agora.month(), agora.day());

  if (!existeDataExibida || !data.ehIgual(ultimaDataExibida)) {
    const DiaLiturgico dia = calendario.obterDiaLiturgico(data);

    Serial.println(dia.celebracoes[0].nome);
    if (telaEstaDisponivel) {
      tela.exibirDiaLiturgico(dia);
    }

    ultimaDataExibida = data;
    existeDataExibida = true;
  }
}
