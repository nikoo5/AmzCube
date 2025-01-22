#include "tft_commons.h"

TFT_eSPI tft = TFT_eSPI();

void TFTSetup()
{
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
}

void TFTFillScreen(uint16_t color)
{
  tft.fillScreen(color);
}

bool TFTDrawAnimatedGif(const char *filename, uint8_t x, uint8_t y)
{
  info.printf("[TFT] Drawing GIF file: %s\n", filename);
  fs::File file = SPIFFS.open(filename, "r");

  if (!file)
  {
    error.println("[TFT] \tFile not found");
    file.close();
    return false;
  }

  char *header = fileRead(file, 6);
  if (strcmp(header, "GIF87a") != 0 && strcmp(header, "GIF89a") != 0)
  {
    error.printf("[TFT] \tInvalid GIF file (Header: %s)\n", header);
    file.close();
    return false;
  }

  uint16_t width = fileRead16(file);
  uint16_t height = fileRead16(file);
  uint8_t flags = fileRead8(file);
  uint8_t bgColorIndex = fileRead8(file);
  uint8_t aspectRatio = fileRead8(file);

  info.println("[TFT] \tImage Data:");
  info.printf("[TFT] \t\tHeader: %s\r\n", header);
  info.printf("[TFT] \t\tWidth: %d\r\n", width);
  info.printf("[TFT] \t\tHeight: %d\r\n", height);
  info.printf("[TFT] \t\tFlags: 0b%s\r\n", String(flags, BIN).c_str());
  info.printf("[TFT] \t\tBackground Color Index: 0x%02X\r\n", bgColorIndex);
  info.printf("[TFT] \t\tAspect Ratio: %d\r\n", aspectRatio);

  file.close();

  if (width > tft.width() || height > tft.height())
    return false;

  GifDraw(filename, &tft);

  return true;
}

void TFTDrawText(const char *text, int x, int y)
{
  if (x != -1 && y != -1)
    tft.setCursor(x, y);
  tft.print(text);
}
void TFTDrawText(const char *text)
{
  TFTDrawText(text, -1, -1);
}