#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LARGURA_TELA 128
#define ALTURA_TELA 64

Adafruit_SSD1306 tela(LARGURA_TELA, ALTURA_TELA, &Wire, -1);
RTC_DS3231 rtc;

void setup() {
  Wire.begin(21, 22);

  if (!tela.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {
      delay(1000);
    }
  }

  if (!rtc.begin()) {
    tela.clearDisplay();
    tela.setTextColor(SSD1306_WHITE);
    tela.setTextSize(1);
    tela.setCursor(0, 0);
    tela.println("ERRO DS3231");
    tela.display();

    while (true) {
      delay(1000);
    }
  }

  // Se o RTC perdeu a hora, ajusta para a data/hora
  // em que o programa foi compilado.
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime agora = rtc.now();

  tela.clearDisplay();
  tela.setTextColor(SSD1306_WHITE);

  // Titulo
  tela.setTextSize(1);
  tela.setCursor(0, 0);
  tela.println("TESTE DS3231");

  // Data
  tela.setCursor(0, 18);

  if (agora.day() < 10) tela.print("0");
  tela.print(agora.day());
  tela.print("/");

  if (agora.month() < 10) tela.print("0");
  tela.print(agora.month());
  tela.print("/");
  tela.print(agora.year());

  // Hora
  tela.setTextSize(2);
  tela.setCursor(0, 35);

  if (agora.hour() < 10) tela.print("0");
  tela.print(agora.hour());
  tela.print(":");

  if (agora.minute() < 10) tela.print("0");
  tela.print(agora.minute());
  tela.print(":");

  if (agora.second() < 10) tela.print("0");
  tela.print(agora.second());

  tela.display();

  delay(1000);
}