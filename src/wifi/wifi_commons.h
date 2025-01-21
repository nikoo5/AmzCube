#ifndef WIFI_COMMONS_H_
#define WIFI_COMMONS_H_

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#include "amzcube_commons.h"
#include "nvs/nvs_commons.h"
#include "spiffs/spiffs_commons.h"

void WiFiSetup();
bool setNtpClock();
char *executeGetRequest(const char *url, const char *caCertificate);

#endif // WIFI_H_