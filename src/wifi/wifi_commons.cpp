#include "wifi_commons.h"

WiFiMulti wifiMulti;

void WiFiSetup()
{
    char *wifi_ssid = getNvsValue("wifi_ssid");
    char *wifi_password = getNvsValue("wifi_password");

    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(wifi_ssid, wifi_password);

    info.print("[WiFi] Waiting for WiFi to connect");
    while ((wifiMulti.run() != WL_CONNECTED))
    {
        info.print(".");
    }
    info.printf("\n[WiFi] \tConnected to SSID: %s\n", wifi_ssid);

    setNtpClock();
}

bool setNtpClock()
{
    info.println("[WiFi] Setting clock using NTP");
    info.print("[WiFi] \tWaiting for NTP time sync");

    configTime(0, 0, "pool.ntp.org");

    time_t nowSecs = time(nullptr);
    while (nowSecs < 8 * 3600 * 2)
    {
        delay(500);
        info.print(".");
        yield();
        nowSecs = time(nullptr);
    }

    info.println();
    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);
    info.print("[WiFi] \tCurrent time: ");
    info.println(asctime(&timeinfo));

    return true;
}

char *executeGetRequest(const char *url, const char *caCertificate)
{
    info.printf("[WiFi] Executing GET request to: %s\n", url);

    WiFiClientSecure *client = new WiFiClientSecure;
    if (client)
    {
        const char *cert = getFileFromSPIFFS(caCertificate);
        if (cert)
        {
            client->setCACert(cert);

            {
                HTTPClient http;
                if (http.begin(*client, url))
                {
                    int httpCode = http.GET();

                    info.printf("[WiFi] \tHTTP code: %d\n", httpCode);
                    if (httpCode > 0)
                    {
                        if (httpCode == HTTP_CODE_OK)
                        {
                            info.println("[WiFi] \tRequest successful");
                            String content = http.getString().c_str();
                            return strdup(content.c_str());
                        }
                    }

                    http.end();
                }
                else
                {
                    error.println("[WiFi] \tFailed to connect");
                }
            }
        }
        else
        {
            error.println("[WiFi] \tFailed to set CA certificate");
        }

        delete client;
    }
    else
    {
        error.println("[WiFi] \tFailed to create client");
    }

    return nullptr;
}