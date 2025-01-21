#include "tft_commons.h"

TFT_eSPI tft = TFT_eSPI();

void TFTSetup()
{
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
}

void TFTFillScreen(uint16_t color)
{
  tft.fillScreen(color);
}