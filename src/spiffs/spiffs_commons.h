#ifndef SPIFFS_COMMONS_H_
#define SPIFFS_COMMONS_H_

#include <SPIFFS.h>

#include "amzcube_commons.h"

void SpiffsSetup();
char *getFileFromSPIFFS(const char *filename);
void writeFileToSPIFFS(const char *filename, const char *content);

#endif