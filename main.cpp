#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

void button(int x, int y, int w, int h, uint16_t col,
            const char* l1, const char* l2) {
  tft.fillRoundRect(x, y, w, h, 8, col);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  if (l2[0]) {
    tft.drawString(l1, x + w / 2, y + h / 2 - 11);
    tft.drawString(l2, x + w / 2, y + h / 2 + 11);
  } else {
    tft.drawString(l1, x + w / 2, y + h / 2);
  }
}

void setup() {
  tft.init();
  tft.setRotation(1);
  int W = tft.width(), H = tft.height();
  tft.fillScreen(TFT_BLACK);

  // thanh tren
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(2);
  tft.drawString("14:35", 6, 6);
  tft.setTextSize(1);
  tft.drawString("Thu 5, 18/09/2026", 120, 12);
  for (int i = 0; i < 3; i++) tft.drawCircle(302, 22, 4 + i * 5, TFT_GREEN);
  tft.fillCircle(302, 22, 2, TFT_GREEN);

  // tieu de
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(4);
  tft.drawString("PTP Repair", W / 2, 46);

  // so dien thoai
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.drawString("0977885774 - 0339287797", W / 2, 74);

  // READY
  tft.drawRoundRect(W / 2 - 70, 84, 140, 22, 11, TFT_GREEN);
  tft.fillCircle(W / 2 - 52, 95, 5, TFT_GREEN);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.drawString("READY", W / 2 + 8, 95);

  // hang nut 1
  button(4,   116, 100, 52, TFT_GREEN, "START", "AUTO");
  button(110, 116, 100, 52, TFT_BLUE,  "CHI HUT", "C.KHONG");
  button(216, 116, 100, 52, TFT_RED,   "STOP", "");

  // hang nut 2
  uint16_t dk = tft.color565(28, 40, 56);
  button(4,   174, 100, 52, dk, "CAI DAT", "");
  button(110, 174, 100, 52, dk, "THU CONG", "");
  button(216, 174, 100, 52, dk, "TRANG", "THAI");
}

void loop() {}
