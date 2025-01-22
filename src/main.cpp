#include "main.h"

void setup()
{
  SerialSetup();

  // uncomment to set Wifi SSID and password for the first time
  // NVSSetValue("wifi_ssid", "<your_wifi_ssid>");
  // NVSSetValue("wifi_password", "<your_wifi_pass>");

  SPIFFSSetup();
  WiFiSetup();
  TFTSetup();
  GifSetup();
}

void loop()
{
  if (!SPIFFSFileExists("/images/gifs/eyes.gif"))
  {
    char *response = WiFiExecuteGetRequest("https://raw.githubusercontent.com/nikoo5/AmzCube/refs/heads/master/web/images/gifs/eyes.gif", "/certs/DigiCert_CA1.crt");

    if (response != nullptr)
    {
      SPIFFSWriteFileContent("/images/gifs/eyes.gif", response);
    }
  }
  else
  {
    if (!TFTDrawAnimatedGif("/images/gifs/eyes.gif", 0, 0))
      delay(5000);
  }
}
