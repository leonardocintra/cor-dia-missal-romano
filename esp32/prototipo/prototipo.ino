#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LARGURA_TELA 128
#define ALTURA_TELA 64

Adafruit_SSD1306 tela(LARGURA_TELA, ALTURA_TELA, &Wire, -1);

// =========================
// BOTÕES
// =========================

const int PINO_BOTAO_ROXO = 25;
const int PINO_BOTAO_VERMELHO = 26;
const int PINO_BOTAO_BRANCO = 27;
const int PINO_BOTAO_VERDE = 32;

// =========================
// RELÉS
// =========================

const int PINO_RELE_ROXO = 4;
const int PINO_RELE_VERMELHO = 16;
const int PINO_RELE_BRANCO = 17;
const int PINO_RELE_VERDE = 18;

// =========================
// MODO E CORES
// =========================

enum Modo {
  MODO_AUTO,
  MODO_MANUAL
};

enum Cor {
  COR_NENHUMA,
  COR_ROXO,
  COR_VERMELHO,
  COR_BRANCO,
  COR_VERDE
};

Modo modoAtual = MODO_AUTO;
Cor corAtual = COR_NENHUMA;

// =========================
// DATA SIMULADA
// Temporariamente sem DS3231
// =========================

int anoAtual = 2026;
int mesAtual = 5;
int diaAtual = 24;

// =========================
// DESLIGAR TODOS OS RELÉS
// Relés são ativos em LOW
// =========================

void desligarTodosOsRele() {
  digitalWrite(PINO_RELE_ROXO, HIGH);
  digitalWrite(PINO_RELE_VERMELHO, HIGH);
  digitalWrite(PINO_RELE_BRANCO, HIGH);
  digitalWrite(PINO_RELE_VERDE, HIGH);
}

// =========================
// NOME DA COR
// =========================

const char* obterNomeCor(Cor cor) {
  switch (cor) {
    case COR_ROXO:
      return "ROXO";

    case COR_VERMELHO:
      return "VERMELHO";

    case COR_BRANCO:
      return "BRANCO";

    case COR_VERDE:
      return "VERDE";

    default:
      return "NENHUMA";
  }
}

// =========================
// ACIONAR COR
// =========================

void acionarCor(Cor cor) {
  desligarTodosOsRele();

  switch (cor) {
    case COR_ROXO:
      digitalWrite(PINO_RELE_ROXO, LOW);
      break;

    case COR_VERMELHO:
      digitalWrite(PINO_RELE_VERMELHO, LOW);
      break;

    case COR_BRANCO:
      digitalWrite(PINO_RELE_BRANCO, LOW);
      break;

    case COR_VERDE:
      digitalWrite(PINO_RELE_VERDE, LOW);
      break;

    case COR_NENHUMA:
      break;
  }

  corAtual = cor;
}

// =========================
// COR AUTOMÁTICA
// Temporária para testes
// =========================

Cor obterCorAutomatica() {

  // Pentecostes
  if (anoAtual == 2026 &&
      mesAtual == 5 &&
      diaAtual == 24) {
    return COR_VERMELHO;
  }

  // Domingo de Páscoa
  if (anoAtual == 2026 &&
      mesAtual == 4 &&
      diaAtual == 5) {
    return COR_BRANCO;
  }

  // Corpus Christi
  if (anoAtual == 2026 &&
      mesAtual == 6 &&
      diaAtual == 4) {
    return COR_BRANCO;
  }

  // Primeiro Domingo do Advento
  if (anoAtual == 2026 &&
      mesAtual == 11 &&
      diaAtual == 29) {
    return COR_ROXO;
  }

  // Sábado Santo
  if (anoAtual == 2026 &&
      mesAtual == 4 &&
      diaAtual == 4) {
    return COR_VERMELHO;
  }

  // Padrão temporário
  return COR_VERDE;
}

// =========================
// ATUALIZAR OLED
// =========================

void atualizarTela() {

  tela.clearDisplay();

  tela.setTextColor(SSD1306_WHITE);

  // Título
  tela.setTextSize(1);
  tela.setCursor(0, 0);
  tela.println("CONTROLE LITURGICO");

  // Modo
  tela.setCursor(0, 16);

  if (modoAtual == MODO_AUTO) {
    tela.println("MODO: AUTO");
  } else {
    tela.println("MODO: MANUAL");
  }

  // Cor
  tela.setTextSize(2);
  tela.setCursor(0, 30);
  tela.println(obterNomeCor(corAtual));

  // Data
  tela.setTextSize(1);
  tela.setCursor(0, 54);

  if (diaAtual < 10) {
    tela.print("0");
  }

  tela.print(diaAtual);
  tela.print("/");

  if (mesAtual < 10) {
    tela.print("0");
  }

  tela.print(mesAtual);
  tela.print("/");
  tela.print(anoAtual);

  tela.display();
}

// =========================
// SELEÇÃO MANUAL
// =========================

void selecionarCorManual(Cor cor) {

  modoAtual = MODO_MANUAL;

  acionarCor(cor);

  atualizarTela();

  Serial.print("MODO MANUAL - COR: ");
  Serial.println(obterNomeCor(cor));
}

// =========================
// SETUP
// =========================

void setup() {

  Serial.begin(115200);

  // Botões
  pinMode(PINO_BOTAO_ROXO, INPUT_PULLUP);
  pinMode(PINO_BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(PINO_BOTAO_BRANCO, INPUT_PULLUP);
  pinMode(PINO_BOTAO_VERDE, INPUT_PULLUP);

  // Relés
  pinMode(PINO_RELE_ROXO, OUTPUT);
  pinMode(PINO_RELE_VERMELHO, OUTPUT);
  pinMode(PINO_RELE_BRANCO, OUTPUT);
  pinMode(PINO_RELE_VERDE, OUTPUT);

  // Desliga todos os relés
  desligarTodosOsRele();

  // I2C
  Wire.begin(21, 22);

  // OLED
  if (!tela.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println("ERRO: OLED nao encontrado.");

    while (true) {
      delay(1000);
    }
  }

  // Modo inicial
  modoAtual = MODO_AUTO;

  // Obtém cor automática
  Cor corAutomatica = obterCorAutomatica();

  // Aciona cor
  acionarCor(corAutomatica);

  // Atualiza tela
  atualizarTela();

  // Informações no Serial
  Serial.println("Sistema iniciado.");

  Serial.print("Data: ");
  Serial.print(diaAtual);
  Serial.print("/");
  Serial.print(mesAtual);
  Serial.print("/");
  Serial.println(anoAtual);

  Serial.print("Cor automatica: ");
  Serial.println(obterNomeCor(corAutomatica));
}

// =========================
// LOOP
// =========================

void loop() {

  // Botão roxo
  if (digitalRead(PINO_BOTAO_ROXO) == LOW) {

    selecionarCorManual(COR_ROXO);

    delay(250);
  }

  // Botão vermelho
  if (digitalRead(PINO_BOTAO_VERMELHO) == LOW) {

    selecionarCorManual(COR_VERMELHO);

    delay(250);
  }

  // Botão branco
  if (digitalRead(PINO_BOTAO_BRANCO) == LOW) {

    selecionarCorManual(COR_BRANCO);

    delay(250);
  }

  // Botão verde
  if (digitalRead(PINO_BOTAO_VERDE) == LOW) {

    selecionarCorManual(COR_VERDE);

    delay(250);
  }
}