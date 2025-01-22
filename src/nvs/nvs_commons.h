#ifndef NVS_COMMONS_H_
#define NVS_COMMONS_H_

#define NVS_KEY_WIFI_SSID "wifi_ssid"
#define NVS_KEY_WIFI_PASSWORD "wifi_password"
#define NVS_KEY_INIT_GIF "init_gif"

#include <nvs.h>

#include "amzcube_commons.h"

bool NVSSetValue(const char *key, const char *value);
char *NVSGetValue(const char *key);

#endif