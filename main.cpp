#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#define BG     tft.color565(21,23,28)
#define CARD   tft.color565(32,36,44)
#define ORANGE tft.color565(242,162,74)
#define MUTED  tft.color565(150,160,180)
#define GRN    tft.color565(61,214,140)

int hh = 20, mm = 52, ss = 49;
uint32_t lastSec = 0;

void drawClock() {
  tft.fillRect(8, 24, 150, 44, BG);
  tft.fillRect(158, 24, 50, 30, BG);
  char b[6]; sprintf(b, "%02d:%02d", hh, mm);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_WHITE, BG);
  tft.setTextSize(5); tft.drawString(b, 8, 26);
  char s[3]; sprintf(s, "%02d", ss);
  tft.setTextSize(2); tft.setTextColor(ORANGE, BG);
  tft.drawString(s, 160, 28);
}

void drawHome() {
  tft.fillScreen(BG);
  // thanh trang thai
  tft.fillCircle(10, 10, 3, GRN);
  tft.setTextDatum(TL_DATUM); tft.setTextColor(MUTED, BG); tft.setTextSize(1);
  tft.drawString("Wi-Fi", 18, 6);
  tft.setTextDatum(TR_DATUM); tft.drawString("28C  20:52", 314, 6);
  // logo
  tft.drawCircle(292, 46, 20, TFT_RED);
  tft.drawCircle(292, 46, 17, TFT_RED);
  tft.setTextDatum(MC_DATUM); tft.setTextColor(TFT_WHITE, BG);
  tft.drawString("PTP", 292, 42); tft.drawString("REPAIR", 292, 52);
  // ngay
  tft.setTextDatum(TL_DATUM); tft.setTextColor(MUTED, BG);
  tft.drawString("Thu Ba, 17 Thang 11, 2026", 8, 74);
  // lich tuan
  const char* dow[] = {"T2","T3","T4","T5","T6","T7","CN"};
  for (int i = 0; i < 7; i++) {
    int x = 8 + i * 43; bool t = (i == 1);
    tft.fillRoundRect(x, 90, 40, 34, 5, t ? ORANGE : CARD);
    tft.setTextDatum(MC_DATUM); tft.setTextSize(1);
    tft.setTextColor(t ? tft.color565(90,52,16) : MUTED, t ? ORANGE : CARD);
    tft.drawString(dow[i], x + 20, 100);
    tft.setTextSize(2);
    tft.setTextColor(t ? tft.color565(26,18,6) : TFT_WHITE, t ? ORANGE : CARD);
    tft.drawString(String(16 + i), x + 20, 114);
  }
  // su kien
  tft.setTextDatum(TL_DATUM); tft.setTextSize(1);
  tft.setTextColor(MUTED, BG); tft.drawString("Su kien tiep theo:", 8, 136);
  tft.setTextColor(ORANGE, BG); tft.drawString("Giao hang khach 15:00", 108, 136);
  // thuong hieu
  tft.setTextColor(ORANGE, BG); tft.drawString("PTP ", 8, 152);
  tft.setTextColor(MUTED, BG);  tft.drawString("REPAIR", 32, 152);
  // nut micro
  tft.fillCircle(292, 168, 15, tft.color565(74,168,255));
  tft.setTextDatum(MC_DATUM); tft.setTextColor(tft.color565(6,18,31));
  tft.setTextSize(2); tft.drawString("M", 292, 168);
  // thanh tab
  const char* tabs[] = {"Trang chu","Deck","May in","Cai dat"};
  for (int i = 0; i < 4; i++) {
    int x = 6 + i * 78; bool a = (i == 0);
    tft.fillRoundRect(x, 198, 74, 36, 7, a ? ORANGE : CARD);
    tft.setTextDatum(MC_DATUM); tft.setTextSize(1);
    tft.setTextColor(a ? tft.color565(26,18,6) : MUTED, a ? ORANGE : CARD);
    tft.drawString(tabs[i], x + 37, 216);
  }
}

void setup() {
  tft.init();
  tft.setRotation(1);
  drawHome();
  drawClock();
}

void loop() {
  if (millis() - lastSec >= 1000) {
    lastSec = millis();
    if (++ss >= 60) { ss = 0; if (++mm >= 60) { mm = 0; hh = (hh + 1) % 24; } }
    drawClock();
  }
}
