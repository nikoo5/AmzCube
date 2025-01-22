#include "main.h"

String playGif = "";

void setup()
{
  SerialSetup();
  TFTSetup();

  TFTDrawText("Initializing...\n", 0, 0);

  // uncomment to set Wifi SSID and password for the first time
  // NVSSetValue(NVSKeys::WIFI_SSID, "<your_wifi_ssid>");
  // NVSSetValue(NVSKeys::WIFI_PASSWORD, "<your_wifi_pass>");

  TFTDrawText("  Setting up SPIFFS... ");
  SPIFFSSetup();
  TFTDrawText("OK\n  Setting up WiFi... ");
  WiFiSetup();
  TFTDrawText("OK\n  Setting up GIF... ");
  GifSetup();
  TFTDrawText("OK\n");

  TFTDrawText("All systems ready!\n");
  delay(2000);
  TFTFillScreen(TFT_BLACK);

  char *initGif = NVSGetValue(NVSKeys::INIT_GIF);
  if (initGif != nullptr)
  {
    playGif = String(initGif);
    free(initGif);
  }
}

void loop()
{
  if (Serial.available())
  {
    String rcv = Serial.readStringUntil('\n');
    rcv.trim();

    String command = rcv.substring(0, rcv.indexOf(" "));
    String arg1 = rcv.substring(rcv.indexOf(" ") + 1);
    String arg2 = arg1.substring(arg1.indexOf(" ") + 1);

    if (arg1.indexOf(" ") != -1)
      arg1 = arg1.substring(0, arg1.indexOf(" "));

    info.printf("[MAIN] Received Command: %s\n", command.c_str());
    if (arg1 != "")
      info.printf("[MAIN] Received Argument: %s\n", arg1.c_str());

    if (command == "read" && arg1 != "")
    {
      char *content = SPIFFSGetFileContent(arg1.c_str());
      if (content != nullptr)
      {
        info.println(content);
        free(content);
      }
    }
    else if (command == "space" && arg1 != "")
    {
      if (arg1 == "total")
        info.printf("[MAIN] Total space: %d\n", SPIFFSGetTotalSpace());
      else if (arg1 == "free")
        info.printf("[MAIN] Free space: %d\n", SPIFFSGetFreeSpace());
    }
    else if (command == "delete" && arg1 != "")
    {
      if (SPIFFSFileExists(arg1.c_str()))
        SPIFFSDeleteFile(arg1.c_str());
    }
    else if (command == "list" && arg1 != "")
    {
      SPIFFSPrintDirectory(arg1.c_str(), 0);
    }
    else if (command == "gif" && arg1 != "")
    {
      if (arg2 != "always")
      {
        int replay = arg2 != "" ? arg2.toInt() : 1;
        for (int i = 0; i < replay; i++)
          TFTDrawAnimatedGif(arg1.c_str(), 0, 0);
        TFTFillScreen(TFT_BLACK);
      }
      else
      {
        playGif = arg1;
        NVSSetValue(NVSKeys::INIT_GIF, playGif.c_str());
      }
    }
    else if (command == "download" && arg1 != "" && arg2 != "")
    {
      bool status = WiFiDownloadFile(arg1.c_str(), "/certs/DigiCert_CA1.crt", arg2.c_str());
    }
  }

  if (playGif != "")
    TFTDrawAnimatedGif(playGif.c_str(), 0, 0);

  delay(1000);
}

// Example commands
// read /test.txt
// download https://raw.githubusercontent.com/nikoo5/AmzCube/refs/heads/master/web/images/gifs/rocket.gif /output.gif
// gif /output.gif 3