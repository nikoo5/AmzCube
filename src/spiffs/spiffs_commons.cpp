#include "spiffs_commons.h"

void SpiffsSetup()
{
  if (!SPIFFS.begin(true))
  {
    error.println("[SPIFFS] Failed to mount file system");
    return;
  }
  else
  {
    info.println("[SPIFFS] File system mounted");
  }
}

char *getFileFromSPIFFS(const char *filename)
{
  File file = SPIFFS.open(filename, "r");
  if (!file)
  {
    error.println("[SPIFFS] Failed to open file for reading");
    return nullptr;
  }

  size_t size = file.size();
  char *buf = (char *)malloc(size + 1);
  if (!buf)
  {
    error.println("[SPIFFS] Failed to allocate memory");
    file.close();
    return nullptr;
  }

  size_t read = file.readBytes(buf, size);
  if (read != size)
  {
    error.println("[SPIFFS] Failed to read file");
    free(buf);
    file.close();
    return nullptr;
  }

  buf[size] = '\0';
  file.close();
  return buf;
}

void writeFileToSPIFFS(const char *filename, const char *content)
{
  File file = SPIFFS.open(filename, "w");
  if (!file)
  {
    error.println("[SPIFFS] Failed to open file for writing");
    return;
  }

  size_t written = file.print(content);
  if (written != strlen(content))
  {
    error.println("[SPIFFS] Failed to write file");
    file.close();
    return;
  }

  file.close();
}