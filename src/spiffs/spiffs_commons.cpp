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

fs::File SpiffsGetFile(const char *filename, const char *mode)
{
  fs::File file = SPIFFS.open(filename, mode);
  if (!file)
  {
    error.printf("[SPIFFS] Failed to open file %s\n", filename);
  }
  return file;
}

bool SpiffsFileExists(const char *filename)
{
  return SPIFFS.exists(filename);
}

char *SpiffsGetFileContent(const char *filename)
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

void SpiffsWriteFileContent(const char *filename, const char *content)
{
  info.printf("[SPIFFS] Writing file %s\n", filename);

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

  info.println("[SPIFFS] \tFile successfully written");
}

char *fileRead(fs::File &file, uint16_t length)
{
  char *buf = (char *)malloc(length + 1);
  if (!buf)
  {
    error.println("[SPIFFS] \tFailed to allocate memory");
    file.close();
    return nullptr;
  }

  size_t read = file.readBytes(buf, length);
  if (read != length)
  {
    error.println("[SPIFFS] \tFailed to read file");
    free(buf);
    file.close();
    return nullptr;
  }

  buf[length] = '\0';
  file.close();
  return buf;
}

uint8_t fileRead8(fs::File &file)
{
  return file.read();
}

uint16_t fileRead16(fs::File &file)
{
  uint16_t result;
  file.read((uint8_t *)&result, 2);
  return result;
}

uint32_t fileRead32(fs::File &file)
{
  uint32_t result;
  file.read((uint8_t *)&result, 4);
  return result;
}