#ifndef TFT_COMMONS_H_
#define TFT_COMMONS_H_

#include <SPI.h>
#include <TFT_eSPI.h>

#include "amzcube_commons.h"
#include "spiffs/spiffs_commons.h"
#include "gif/gif_commons.h"

void TFTSetup();
void TFTFillScreen(uint16_t color);
bool TFTDrawAnimatedGif(const char *filename, uint8_t x, uint8_t y);
void TFTDrawText(const char *text, int x, int y);
void TFTDrawText(const char *text);

#endif