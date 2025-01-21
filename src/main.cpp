#include "main.h"

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#include <SPIFFS.h>

#include <nvs.h>

bool setNvsValue(const char *key, const char *value)
{
  nvs_handle handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
  if (err != ESP_OK)
  {
    Serial.printf("Failed to open NVS: %d\n", err);
    return false;
  }

  err = nvs_set_str(handle, key, value);
  if (err != ESP_OK)
  {
    Serial.printf("Failed to set NVS value: %d\n", err);
    nvs_close(handle);
    return false;
  }

  err = nvs_commit(handle);
  if (err != ESP_OK)
  {
    Serial.printf("Failed to commit NVS value: %d\n", err);
    nvs_close(handle);
    return false;
  }

  nvs_close(handle);
  return true;
}

char *getNvsValue(const char *key)
{
  nvs_handle handle;
  esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
  if (err != ESP_OK)
  {
    Serial.printf("Failed to open NVS: %d\n", err);
    return nullptr;
  }

  size_t size;
  err = nvs_get_str(handle, key, nullptr, &size);
  if (err != ESP_OK)
  {
    Serial.printf("Failed to get NVS value size: %d\n", err);
    nvs_close(handle);
    return nullptr;
  }

  char *value = (char *)malloc(size);
  if (!value)
  {
    Serial.println("Failed to allocate memory");
    nvs_close(handle);
    return nullptr;
  }

  err = nvs_get_str(handle, key, value, &size);
  if (err != ESP_OK)
  {
    Serial.printf("Failed to get NVS value: %d\n", err);
    free(value);
    nvs_close(handle);
    return nullptr;
  }

  nvs_close(handle);
  return value;
}

void setClock()
{
  configTime(0, 0, "pool.ntp.org");

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
}

char *getFileFromSPIFFS(const char *filename)
{
  File file = SPIFFS.open(filename, "r");
  if (!file)
  {
    Serial.println(F("Failed to open file for reading"));
    return nullptr;
  }

  size_t size = file.size();
  char *buf = (char *)malloc(size + 1);
  if (!buf)
  {
    Serial.println(F("Failed to allocate memory"));
    file.close();
    return nullptr;
  }

  size_t read = file.readBytes(buf, size);
  if (read != size)
  {
    Serial.println(F("Failed to read file"));
    free(buf);
    file.close();
    return nullptr;
  }

  buf[size] = '\0';
  file.close();
  return buf;
}

WiFiMulti wifiMulti;

void setup()
{
  Serial.begin(SERIAL_SPEED);

  // uncomment to set Wifi SSID and password for the first time
  // setNvsValue("wifi_ssid", "<your_wifi_ssid>");
  // setNvsValue("wifi_password", "<your_wifi_pass>");

  char *wifi_ssid = getNvsValue("wifi_ssid");
  char *wifi_password = getNvsValue("wifi_password");

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(wifi_ssid, wifi_password);

  Serial.print("Waiting for WiFi to connect...");
  while ((wifiMulti.run() != WL_CONNECTED))
  {
    Serial.print(".");
  }
  Serial.println(" connected");

  setClock();

  if (!SPIFFS.begin(true))
  {
    Serial.println("Failed to mount file system");
    return;
  }
}

void loop()
{
  delay(2000);

  const char *testUrl = "https://raw.githubusercontent.com/nikoo5/AmzCube/refs/heads/master/platformio.ini";

  WiFiClientSecure *client = new WiFiClientSecure;
  if (client)
  {
    const char *cert = getFileFromSPIFFS("/certs/DigiCert_CA1.crt");
    client->setCACert(cert);

    {
      HTTPClient http;

      if (http.begin(*client, testUrl))
      {
        int httpCode = http.GET();

        if (httpCode > 0)
        {
          if (httpCode == HTTP_CODE_OK)
          {
            String content = http.getString().c_str();
            Serial.println(content);
          }
          else
          {
            Serial.printf("HTTP code: %d\n", httpCode);
          }
        }

        http.end();
      }
      else
      {
        Serial.println("Failed to connect");
      }
    }

    delete client;
  }
  else
  {
    Serial.println("Failed to create client");
  }

  delay(60000);
}
