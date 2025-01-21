#ifndef SPIFFS_COMMONS_H_
#define SPIFFS_COMMONS_H_

#include <SPIFFS.h>

#include "amzcube_commons.h"

void SpiffsSetup();
bool fileExists(const char *filename);
char *getFileFromSPIFFS(const char *filename);
void writeFileToSPIFFS(const char *filename, const char *content);

#endif