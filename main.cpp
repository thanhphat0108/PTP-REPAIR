#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define LCD_CS 5
#define LCD_DC 27
#define LCD_RST 17

Adafruit_ILI9341 tft = Adafruit_ILI9341(LCD_CS, LCD_DC, LCD_RST);

void setup() {
  tft.begin(10000000);          // SPI cham lai cho breadboard (10MHz)
  tft.setRotation(0);
  int w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(0, 0, w, h, ILI9341_WHITE);
  tft.drawRect(2, 2, w - 4, h - 4, ILI9341_WHITE);

  tft.fillRect(0, 0, 24, 24, ILI9341_RED);        // goc tren-trai
  tft.fillRect(w - 24, 0, 24, 24, ILI9341_GREEN); // goc tren-phai
  tft.fillRect(0, h - 24, 24, 24, ILI9341_BLUE);  // goc duoi-trai
  tft.fillRect(w - 24, h - 24, 24, 24, ILI9341_YELLOW); // goc duoi-phai

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.setCursor(30, h / 2 - 30);
  tft.print("PTP");
  tft.setTextSize(2);
  tft.setCursor(30, h / 2 + 6);
  tft.print("REPAIR");
  tft.setTextSize(1);
  tft.setCursor(30, h / 2 + 34);
  tft.print("FULL SCREEN 10MHz");
}

void loop() {}
