#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

void bg() {
  for (int y = 0; y < 320; y += 20) tft.fillRect(0, y, 240, 20, TFT_BLACK);
}

void button(int x, int y, int w, int h, uint16_t col, int ts,
            const char* l1, const char* l2) {
  tft.fillRoundRect(x, y, w, h, 8, col);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(ts);
  if (l2[0]) {
    tft.drawString(l1, x + w / 2, y + h / 2 - 5 * ts);
    tft.drawString(l2, x + w / 2, y + h / 2 + 5 * ts);
  } else {
    tft.drawString(l1, x + w / 2, y + h / 2);
  }
}

void setup() {
  tft.init();
  tft.setRotation(0);          // DOC 240x320
  bg();
  int W = 240;

  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(2);
  tft.drawString("14:35", 6, 4);
  for (int i = 0; i < 3; i++) tft.drawCircle(218, 14, 3 + i * 4, TFT_GREEN);
  tft.fillCircle(218, 14, 2, TFT_GREEN);
  tft.setTextSize(1);
  tft.drawString("Thu 5, 18/09/2026", 6, 28);

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(3);
  tft.drawString("PTP Repair", W / 2, 54);

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.drawString("0977885774-0339287797", W / 2, 76);

  tft.drawRoundRect(W / 2 - 72, 86, 144, 22, 11, TFT_GREEN);
  tft.fillCircle(W / 2 - 56, 97, 5, TFT_GREEN);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.drawString("READY", W / 2 + 8, 97);

  uint16_t dk = tft.color565(30, 42, 58);

  button(8,   118, 110, 58, TFT_GREEN, 2, "START", "AUTO");
  button(122, 118, 110, 58, TFT_RED,   2, "STOP", "");
  button(8,   182, 224, 46, TFT_BLUE,  2, "CHI HUT C.KHONG", "");
  button(8,   234, 72,  58, dk, 1, "CAI", "DAT");
  button(84,  234, 72,  58, dk, 1, "THU", "CONG");
  button(160, 234, 72,  58, dk, 1, "TRANG", "THAI");
}

void loop() {}
void loop() {}
