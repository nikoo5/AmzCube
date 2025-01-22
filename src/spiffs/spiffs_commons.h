#ifndef SPIFFS_COMMONS_H_
#define SPIFFS_COMMONS_H_

#include <SPIFFS.h>

#include "amzcube_commons.h"

void SPIFFSSetup();
uint8_t SPIFFSPrintDirectory(const char *rootPath, uint8_t level);
uint32_t SPIFFSGetTotalSpace();
uint32_t SPIFFSGetFreeSpace();
bool SPIFFSDeleteFile(const char *filename);
uint32_t SPIFFSGetFileSize(const char *filename);
fs::File SPIFFSGetFile(const char *filename, const char *mode);
bool SPIFFSFileExists(const char *filename);
char *SPIFFSGetFileContent(const char *filename);
void SPIFFSWriteFileContent(const char *filename, const char *content);

char *fileRead(fs::File &file, uint16_t length);
uint8_t fileRead8(fs::File &f);
uint16_t fileRead16(fs::File &f);
uint32_t fileRead32(fs::File &f);

#endif