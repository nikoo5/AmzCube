#ifndef NVS_COMMONS_H_
#define NVS_COMMONS_H_

#include <nvs.h>

#include "amzcube_commons.h"

bool NVSSetValue(const char *key, const char *value);
char *NVSGetValue(const char *key);

#endif