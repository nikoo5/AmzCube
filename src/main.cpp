#include "main.h"

void setup()
{
  SerialSetup();

  // uncomment to set Wifi SSID and password for the first time
  // setNvsValue("wifi_ssid", "<your_wifi_ssid>");
  // setNvsValue("wifi_password", "<your_wifi_pass>");

  SpiffsSetup();
  WiFiSetup();
  TFTSetup();

  TFTFillScreen(TFT_GREEN);
}

void loop()
{
  if (!SpiffsFileExists("/images/gifs/eyes.gif"))
  {
    char *response = executeGetRequest("https://raw.githubusercontent.com/nikoo5/AmzCube/refs/heads/master/web/images/gifs/eyes.gif", "/certs/DigiCert_CA1.crt");

    if (response != nullptr)
    {
      SpiffsWriteFileContent("/images/gifs/eyes.gif", response);
    }
  }
  else
  {
  }
}
