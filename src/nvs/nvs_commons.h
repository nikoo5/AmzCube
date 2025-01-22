#ifndef NVS_COMMONS_H_
#define NVS_COMMONS_H_

#include <nvs.h>

#include "amzcube_commons.h"

struct NVSKeys
{
  static constexpr const char *WIFI_SSID = "ssid_wifi";
  static constexpr const char *WIFI_PASSWORD = "wifi_password";
  static constexpr const char *INIT_GIF = "init_gif";
};

bool NVSSetValue(const char *key, const char *value);
char *NVSGetValue(const char *key);

#endif