#include "wifi_commons.h"

WiFiMulti wifiMulti;

void WiFiSetup()
{
    char *wifi_ssid = NVSGetValue(NVS_KEY_WIFI_SSID);
    char *wifi_password = NVSGetValue(NVS_KEY_WIFI_PASSWORD);

    if (!wifi_ssid || !wifi_password)
    {
        error.println("[WiFi] SSID or password not found in NVS");
        return;
    }

    info.printf("[WiFi] Connecting to %s", wifi_ssid);

    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(wifi_ssid, wifi_password);

    while ((wifiMulti.run() != WL_CONNECTED))
    {
        info.print(".");
    }
    info.println("\n[WiFi] \tSuccessfully connected");

    WiFiSetNtpClock();
}

bool WiFiSetNtpClock()
{
    info.println("[WiFi] Setting clock using NTP");
    info.print("[WiFi] \tWaiting for NTP time sync");

    configTime(0, 0, "pool.ntp.org");

    time_t nowSecs = time(nullptr);

    uint8_t timeout = 20;
    while (nowSecs < 8 * 3600 * 2)
    {
        delay(500);
        info.print(".");
        yield();
        nowSecs = time(nullptr);

        if (timeout-- == 0)
        {
            error.println("\n[WiFi] \tFailed to obtain NTP time");
            return false;
        }
    }

    info.println();
    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);
    info.print("[WiFi] \tSuccessfully obtained NTP time: ");
    info.println(asctime(&timeinfo));

    return true;
}

char *WiFiExecuteGetRequest(const char *url, const char *caCertificate)
{
    info.printf("[WiFi] Executing GET request to: %s\n", url);

    WiFiClientSecure *client = new WiFiClientSecure;
    if (client)
    {
        const char *cert = SPIFFSGetFileContent(caCertificate);
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

bool WiFiDownloadFile(const char *url, const char *caCertificate, const char *filePath)
{
    info.printf("[WiFi] Downloading file from: %s\n", url);
    bool returnValue = false;

    WiFiClientSecure *client = new WiFiClientSecure;
    if (client)
    {
        const char *cert = SPIFFSGetFileContent(caCertificate);
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

                            uint32_t contentLength = http.getSize();
                            uint32_t SPIFFSFreeSpace = SPIFFSGetFreeSpace();

                            bool download = contentLength < SPIFFSFreeSpace;

                            if (!download && SPIFFSFileExists(filePath))
                            {
                                download = contentLength < SPIFFSFreeSpace + SPIFFSGetFileSize(filePath);
                                if (download)
                                {
                                    SPIFFSDeleteFile(filePath);
                                }
                            }

                            if (!download)
                            {
                                error.printf("[WiFi] \tNot enough space to download file (Required: %d, Available: %d)\n", contentLength, SPIFFSFreeSpace);
                            }
                            else
                            {
                                File file = SPIFFS.open(filePath, FILE_WRITE);
                                if (file)
                                {
                                    info.printf("[WiFi] \tWriting to file: %s\n", filePath);
                                    http.writeToStream(&file);
                                    file.close();
                                    returnValue = true;

                                    info.println("[WiFi] \tFile downloaded successfully");
                                }
                            }
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

    return returnValue;
}