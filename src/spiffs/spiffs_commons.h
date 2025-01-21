#ifndef SPIFFS_COMMONS_H_
#define SPIFFS_COMMONS_H_

#include <SPIFFS.h>

#include "amzcube_commons.h"

void SpiffsSetup();
fs::File SpiffsGetFile(const char *filename, const char *mode);
bool SpiffsFileExists(const char *filename);
char *SpiffsGetFileContent(const char *filename);
void SpiffsWriteFileContent(const char *filename, const char *content);

char *fileRead(fs::File &f, uint16_t length);
uint8_t fileRead8(fs::File &f);
uint16_t fileRead16(fs::File &f);
uint32_t fileRead32(fs::File &f);

#endif