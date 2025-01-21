#include "main.h"

void setup()
{
  SerialSetup();

  // uncomment to set Wifi SSID and password for the first time
  // setNvsValue("wifi_ssid", "<your_wifi_ssid>");
  // setNvsValue("wifi_password", "<your_wifi_pass>");

  SpiffsSetup();
  WiFiSetup();
}

void loop()
{
  delay(2000);

  char *response = executeGetRequest("https://raw.githubusercontent.com/nikoo5/AmzCube/refs/heads/master/platformio.ini", "/certs/DigiCert_CA1.crt");

  if (response != nullptr)
  {
    info.println("Response:");
    info.println(response);
    free(response);
  }

  delay(60000);
}
