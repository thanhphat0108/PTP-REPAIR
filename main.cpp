#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define LCD_CS   5
#define LCD_DC   27
#define LCD_RST  17

Adafruit_ILI9341 tft = Adafruit_ILI9341(LCD_CS, LCD_DC, LCD_RST);

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.drawCircle(160, 78, 40, ILI9341_RED);
  tft.drawCircle(160, 78, 36, ILI9341_RED);
  tft.setTextSize(3);
  tft.setCursor(74, 140);
  tft.setTextColor(ILI9341_RED);   tft.print("PTP ");
  tft.setTextColor(ILI9341_WHITE); tft.print("REPAIR");
  tft.setTextSize(1);
  tft.setTextColor(0x8C71);
  tft.setCursor(60, 175);
  tft.print("BANG DIEU KHIEN BAN LAM VIEC");
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(120, 205);
  tft.print("MAN HINH OK");
}

void loop() {
  static bool on = false;
  static uint32_t t = 0;
  if (millis() - t > 600) {
    t = millis();
    on = !on;
    tft.fillCircle(104, 208, 3, on ? ILI9341_GREEN : ILI9341_BLACK);
  }
}
