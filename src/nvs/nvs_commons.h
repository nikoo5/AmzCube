#ifndef NVS_COMMONS_H_
#define NVS_COMMONS_H_

#include <nvs.h>

#include "amzcube_commons.h"

bool setNvsValue(const char *key, const char *value);
char *getNvsValue(const char *key);

#endif