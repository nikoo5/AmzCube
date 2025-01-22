#ifndef NVS_COMMONS_H_
#define NVS_COMMONS_H_

namespace NVS
{
  const char *WIFI_SSID = "ssid_wifi";
  const char *WIFI_PASSWORD = "wifi_password";

  const char *INIT_GIF = "init_gif";
}

#include <nvs.h>

#include "amzcube_commons.h"

bool NVSSetValue(const char *key, const char *value);
char *NVSGetValue(const char *key);

#endif