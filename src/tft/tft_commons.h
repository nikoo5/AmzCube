#ifndef TFT_COMMONS_H_
#define TFT_COMMONS_H_

#include <SPI.h>
#include <TFT_eSPI.h>

#include "amzcube_commons.h"

void TFTSetup();
void TFTFillScreen(uint16_t color);

#endif