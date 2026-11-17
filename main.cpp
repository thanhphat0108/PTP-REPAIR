#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(0);
  int w = tft.width(), h = tft.height();
  tft.fillScreen(TFT_RED);
  tft.drawRect(0, 0, w, h, TFT_WHITE);
  tft.fillRect(0, 0, 24, 24, TFT_BLUE);
  tft.fillRect(w - 24, 0, 24, 24, TFT_GREEN);
  tft.fillRect(0, h - 24, 24, 24, TFT_YELLOW);
  tft.fillRect(w - 24, h - 24, 24, 24, TFT_CYAN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(30, h / 2 - 40);
  tft.print("TFT V3");
  tft.setTextSize(2);
  tft.setCursor(30, h / 2);
  tft.print(w); tft.print("x"); tft.print(h);
  tft.setTextSize(1);
  tft.setCursor(30, h / 2 + 30);
  tft.print("PTP REPAIR FULL");
}

void loop() {}
