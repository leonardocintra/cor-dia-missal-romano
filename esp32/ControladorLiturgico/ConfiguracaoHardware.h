#ifndef CONFIGURACAO_HARDWARE_H
#define CONFIGURACAO_HARDWARE_H

#include <stdint.h>

// Barramento I2C compartilhado pelo OLED e pelo DS3231.
const uint8_t PINO_I2C_SDA = 21;
const uint8_t PINO_I2C_SCL = 22;

// Botões ligados ao GND e lidos com resistor de pull-up interno.
const uint8_t PINO_BOTAO_ROXO = 25;
const uint8_t PINO_BOTAO_VERMELHO = 26;
const uint8_t PINO_BOTAO_BRANCO = 27;
const uint8_t PINO_BOTAO_VERDE = 32;

// Entradas dos quatro módulos de relé.
const uint8_t PINO_RELE_ROXO = 4;
const uint8_t PINO_RELE_VERMELHO = 16;
const uint8_t PINO_RELE_BRANCO = 17;
const uint8_t PINO_RELE_VERDE = 18;

// O módulo de relés liga quando recebe nível baixo.
const bool RELES_ATIVOS_EM_NIVEL_BAIXO = true;

#endif
