#ifndef GIF_COMMONS_H_
#define GIF_COMMONS_H_

#define BUFFER_SIZE 240 // Optimum is >= GIF width or integral division of width

#include <AnimatedGIF.h>
#include <TFT_eSPI.h>

#include "spiffs/spiffs_commons.h"

void GifSetup();

void GifDraw(const char *filename, TFT_eSPI *tft);

void *_gifOpen(const char *filename, int32_t *pFileSize);
void _gifClose(void *pHandle);
int32_t _gifRead(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen);
int32_t _gifSeek(GIFFILE *pFile, int32_t iPosition);
void _gifDraw(GIFDRAW *pDraw);

#endif