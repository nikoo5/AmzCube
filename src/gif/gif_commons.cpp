#include "gif_commons.h"

#define DISPLAY_WIDTH external_tft->width()
#define DISPLAY_HEIGHT external_tft->height()

#ifdef USE_DMA
uint16_t usTemp[2][BUFFER_SIZE];
#else
uint16_t usTemp[1][BUFFER_SIZE];
#endif
bool dmaBuf = 0;

TFT_eSPI *external_tft = nullptr;
AnimatedGIF gif;
fs::File file;

void GifSetup()
{
  gif.begin(BIG_ENDIAN_PIXELS);
}

void GifDraw(const char *filename, TFT_eSPI *tft)
{
  external_tft = tft;

  if (gif.open(filename, _gifOpen, _gifClose, _gifRead, _gifSeek, _gifDraw))
  {
    tft->startWrite(); // The TFT chip slect is locked low
    while (gif.playFrame(true, NULL))
    {
    }
    gif.close();
    tft->endWrite(); // Release TFT chip select for the SD Card Reader
  }
}

void *_gifOpen(const char *filename, int32_t *pFileSize)
{
  file = SPIFFSGetFile(filename, FILE_READ);
  if (file)
  {
    *pFileSize = file.size();
  }
  return &file;
}

void _gifClose(void *pHandle)
{
  file.close();
}

int32_t _gifRead(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
  int32_t iBytesRead;
  iBytesRead = iLen;
  if ((pFile->iSize - pFile->iPos) < iLen)
    iBytesRead = pFile->iSize - pFile->iPos;
  if (iBytesRead <= 0)
    return 0;

  file.seek(pFile->iPos);
  int32_t bytesRead = file.read(pBuf, iLen);
  pFile->iPos += iBytesRead;

  return bytesRead;
}

int32_t _gifSeek(GIFFILE *pFile, int32_t iPosition)
{
  if (iPosition < 0)
    iPosition = 0;
  else if (iPosition >= pFile->iSize)
    iPosition = pFile->iSize - 1;
  pFile->iPos = iPosition;
  file.seek(pFile->iPos);
  return iPosition;
}

void _gifDraw(GIFDRAW *pDraw)
{
  uint8_t *s;
  uint16_t *d, *usPalette;
  int x, y, iWidth, iCount;

  // Displ;ay bounds chech and cropping
  iWidth = pDraw->iWidth;
  if (iWidth + pDraw->iX > DISPLAY_WIDTH)
    iWidth = DISPLAY_WIDTH - pDraw->iX;
  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y; // current line
  if (y >= DISPLAY_HEIGHT || pDraw->iX >= DISPLAY_WIDTH || iWidth < 1)
    return;

  // Old image disposal
  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2) // restore to background color
  {
    for (x = 0; x < iWidth; x++)
    {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }

  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency) // if transparency used
  {
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    pEnd = s + iWidth;
    x = 0;
    iCount = 0; // count non-transparent pixels
    while (x < iWidth)
    {
      c = ucTransparent - 1;
      d = &usTemp[0][0];
      while (c != ucTransparent && s < pEnd && iCount < BUFFER_SIZE)
      {
        c = *s++;
        if (c == ucTransparent) // done, stop
        {
          s--; // back up to treat it like transparent
        }
        else // opaque
        {
          *d++ = usPalette[c];
          iCount++;
        }
      } // while looking for opaque pixels
      if (iCount) // any opaque pixels?
      {
        // DMA would degrtade performance here due to short line segments
        external_tft->setAddrWindow(pDraw->iX + x, y, iCount, 1);
        external_tft->pushPixels(usTemp, iCount);
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent)
          x++;
        else
          s--;
      }
    }
  }
  else
  {
    s = pDraw->pPixels;

    // Unroll the first pass to boost DMA performance
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    if (iWidth <= BUFFER_SIZE)
      for (iCount = 0; iCount < iWidth; iCount++)
        usTemp[dmaBuf][iCount] = usPalette[*s++];
    else
      for (iCount = 0; iCount < BUFFER_SIZE; iCount++)
        usTemp[dmaBuf][iCount] = usPalette[*s++];

#ifdef USE_DMA // 71.6 fps (ST7796 84.5 fps)
    external_tft->dmaWait();
    external_tft->setAddrWindow(pDraw->iX, y, iWidth, 1);
    external_tft->pushPixelsDMA(&usTemp[dmaBuf][0], iCount);
    dmaBuf = !dmaBuf;
#else // 57.0 fps
    external_tft->setAddrWindow(pDraw->iX, y, iWidth, 1);
    external_tft->pushPixels(&usTemp[0][0], iCount);
#endif

    iWidth -= iCount;
    // Loop if pixel buffer smaller than width
    while (iWidth > 0)
    {
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      if (iWidth <= BUFFER_SIZE)
        for (iCount = 0; iCount < iWidth; iCount++)
          usTemp[dmaBuf][iCount] = usPalette[*s++];
      else
        for (iCount = 0; iCount < BUFFER_SIZE; iCount++)
          usTemp[dmaBuf][iCount] = usPalette[*s++];

#ifdef USE_DMA
      external_tft->dmaWait();
      external_tft->pushPixelsDMA(&usTemp[dmaBuf][0], iCount);
      dmaBuf = !dmaBuf;
#else
      external_tft->pushPixels(&usTemp[0][0], iCount);
#endif
      iWidth -= iCount;
    }
  }
}