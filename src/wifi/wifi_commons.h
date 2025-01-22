#ifndef WIFI_COMMONS_H_
#define WIFI_COMMONS_H_

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#include "amzcube_commons.h"
#include "nvs/nvs_commons.h"
#include "spiffs/spiffs_commons.h"

void WiFiSetup();
bool WiFiSetNtpClock();
char *WiFiExecuteGetRequest(const char *url, const char *caCertificate);
bool WiFiDownloadFile(const char *url, const char *caCertificate, const char *filePath);

#endif // WIFI_H_