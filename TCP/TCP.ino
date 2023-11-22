#include <Arduino.h>
#include "WiFi.h"

#define WIFI_SSID "NEO"
#define WIFI_PASSWORD "54332232"


const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

const int port = 10000;
WiFiServer server(port);

String translateEncryptionType(wifi_auth_mode_t encryptionType)
{

  switch (encryptionType)
  {
  case (WIFI_AUTH_OPEN):
    return "Open";
  case (WIFI_AUTH_WEP):
    return "WEP";
  case (WIFI_AUTH_WPA_PSK):
    return "WPA_PSK";
  case (WIFI_AUTH_WPA2_PSK):
    return "WPA2_PSK";
  case (WIFI_AUTH_WPA_WPA2_PSK):
    return "WPA_WPA2_PSK";
  case (WIFI_AUTH_WPA2_ENTERPRISE):
    return "WPA2_ENTERPRISE";
  default:  
    return "UNKNOWN";
  }
  return "UNKNOWN";
}

void scanNetworks()
{

  int numberOfNetworks = WiFi.scanNetworks();

  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);

  for (int i = 0; i < numberOfNetworks; i++)
  {

    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));

    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));

    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));

    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");
  }
}

void connectToNetwork()
{
  WiFi.begin(ssid, password);
  Serial.println("!!!");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }

  Serial.println("Connected to network");
}

void handleTCPClient() {
  WiFiClient client = server.available();
  int errorCount = 0;
  if (client)
  {
    Serial.println("New client");
    
    while (client.connected())
    {
      uint8_t data[4196];
      errorCount = 0;
      if (client.available())
      {
        int len = client.read(data, 4196);
        if (len < 4196)
        {
          data[len] = '\0';
        }
        else
        {
          data[4196] = '\0';
        }
        for (int i = 2; i < 4196; i+= 2) {
          if(data[i-1] != 208 && data[i] != 176) {
            errorCount++;
          }
        }
        Serial.println(errorCount);
        Serial.print("client sent: ");
        Serial.println((char*)data);
      }
    }
  }
}

void setup()
{

  Serial.begin(115200);

  scanNetworks();
  connectToNetwork();

  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop()
{
  handleTCPClient();
}