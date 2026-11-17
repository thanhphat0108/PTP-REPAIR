#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define LCD_CS 5
#define LCD_DC 27
#define LCD_RST 17

Adafruit_ILI9341 tft = Adafruit_ILI9341(LCD_CS, LCD_DC, LCD_RST);

uint8_t rot = 0;
uint16_t bg[] = {ILI9341_NAVY, ILI9341_DARKGREEN, ILI9341_MAROON, ILI9341_PURPLE};

void showRot() {
  tft.setRotation(rot);
  int w = tft.width(), h = tft.height();
  tft.fillScreen(bg[rot]);
  tft.drawRect(0, 0, w, h, ILI9341_WHITE);
  tft.drawRect(1, 1, w - 2, h - 2, ILI9341_WHITE);
  tft.fillRect(0, 0, 24, 24, ILI9341_RED);      // goc TREN-TRAI = do
  tft.fillRect(w - 24, 0, 24, 24, ILI9341_GREEN); // goc TREN-PHAI = xanh
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.setCursor(40, h / 2 - 24);
  tft.print("ROT ");
  tft.print(rot);
  tft.setTextSize(2);
  tft.setCursor(40, h / 2 + 16);
  tft.print(w);
  tft.print("x");
  tft.print(h);
  tft.setTextSize(1);
  tft.setCursor(40, h / 2 + 44);
  tft.print("PTP REPAIR");
}

void setup() {
  tft.begin();
  showRot();
}

void loop() {
  delay(2500);
  rot = (rot + 1) % 4;
  showRot();
}
