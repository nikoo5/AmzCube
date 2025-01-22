#include "nvs_commons.h"

bool NVSSetValue(const char *key, const char *value)
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

char *NVSGetValue(const char *key)
{
  nvs_handle handle;
  esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
  if (err != ESP_OK)
  {
    error.printf("[NVS] Failed to open NVS: %d\n", err);
    return nullptr;
  }

  size_t size;
  err = nvs_get_str(handle, key, nullptr, &size);
  if (err != ESP_OK)
  {
    error.printf("[NVS] Failed to get NVS value size: %d\n", err);
    nvs_close(handle);
    return nullptr;
  }

  char *value = (char *)malloc(size);
  if (!value)
  {
    error.println("[NVS] Failed to allocate memory");
    nvs_close(handle);
    return nullptr;
  }

  err = nvs_get_str(handle, key, value, &size);
  if (err != ESP_OK)
  {
    error.printf("[NVS] Failed to get NVS value: %d\n", err);
    free(value);
    nvs_close(handle);
    return nullptr;
  }

  nvs_close(handle);
  return value;
}