#include "spiffs_commons.h"

void SPIFFSSetup()
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

uint8_t SPIFFSPrintDirectory(const char *rootPath, uint8_t level)
{
  File root = SPIFFS.open(rootPath, FILE_READ);
  File child = root.openNextFile();

  for (uint8_t i = 0; i < level; i++)
    info.print("  ");
  // i == level - 1 ? info.print("|- ") : (i == 0 ? info.print("|  ") : info.print("  "));

  info.println(String(rootPath).substring(String(rootPath).lastIndexOf("/")));

  uint16_t printed = 0;
  while (child)
  {
    String childPath = String(child.path());
    child.close();

    uint8_t separator = childPath.indexOf("/", String(rootPath).length() + (String(rootPath) == "/" ? 0 : 1)) + 1;

    if (childPath.indexOf("/", separator))
    {
      String nextPath = childPath.substring(0, separator - 1);
      uint16_t newPrinted = SPIFFSPrintDirectory(nextPath.c_str(), level + 1);

      for (uint16_t i = 0; i < newPrinted; i++)
        child = root.openNextFile();

      printed += newPrinted;
    }
    else
    {
      String fileName = childPath.substring(separator);
      for (uint8_t i = 0; i < level + 1; i++)
        info.print("  ");
      // i == level - 1 ? info.print("| ") : info.print("  ");
      info.println(fileName.substring(String(rootPath).length() + 1));
      printed++;
    }

    child = root.openNextFile();
  }

  root.close();

  return printed;
}

fs::File SPIFFSGetFile(const char *filename, const char *mode)
{
  fs::File file = SPIFFS.open(filename, mode);
  if (!file)
  {
    error.printf("[SPIFFS] Failed to open file %s\n", filename);
  }
  return file;
}

bool SPIFFSFileExists(const char *filename)
{
  return SPIFFS.exists(filename);
}

uint32_t SPIFFSGetTotalSpace()
{
  return SPIFFS.totalBytes();
}

uint32_t SPIFFSGetFreeSpace()
{
  return SPIFFS.totalBytes() - SPIFFS.usedBytes();
}

bool SPIFFSDeleteFile(const char *filename)
{
  info.printf("[SPIFFS] Deleting file %s\n", filename);
  if (SPIFFS.remove(filename))
  {
    info.println("[SPIFFS] \tFile successfully deleted");
    return true;
  }
  else
  {
    error.println("[SPIFFS] \tFailed to delete file");
  }
  return false;
}

uint32_t SPIFFSGetFileSize(const char *filename)
{
  File file = SPIFFS.open(filename, "r");
  if (!file)
  {
    error.println("[SPIFFS] Failed to open file for reading");
    return 0;
  }

  uint32_t size = file.size();
  file.close();
  return size;
}

char *SPIFFSGetFileContent(const char *filename)
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

void SPIFFSWriteFileContent(const char *filename, const char *content)
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
  uint8_t *buffer = (uint8_t *)malloc(length + 1);
  if (!buffer)
  {
    error.println("[SPIFFS] \tFailed to allocate memory");
    file.close();
    return nullptr;
  }

  size_t read = file.read((uint8_t *)buffer, length);
  if (read != length)
  {
    error.println("[SPIFFS] \tFailed to read file");
    free(buffer);
    file.close();
    return nullptr;
  }

  buffer[length] = '\0';
  return (char *)buffer;
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