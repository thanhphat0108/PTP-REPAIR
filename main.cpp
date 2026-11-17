#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

uint8_t r = 0;
uint16_t bg[] = {TFT_NAVY, TFT_DARKGREEN, TFT_MAROON, TFT_PURPLE};

void show() {
  tft.setRotation(r);
  int w = tft.width(), h = tft.height();
  tft.fillScreen(bg[r]);
  tft.drawRect(0, 0, w, h, TFT_WHITE);
  tft.drawRect(2, 2, w - 4, h - 4, TFT_WHITE);
  tft.fillRect(0, 0, 30, 30, TFT_RED);
  tft.fillRect(w - 30, 0, 30, 30, TFT_GREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(4);
  tft.setCursor(40, h / 2 - 30);
  tft.print("ROT ");
  tft.print(r);
  tft.setTextSize(2);
  tft.setCursor(40, h / 2 + 20);
  tft.print(w);
  tft.print("x");
  tft.print(h);
}

void setup() {
  tft.init();
  show();
}

void loop() {
  delay(2500);
  r = (r + 1) % 4;
  show();
}
