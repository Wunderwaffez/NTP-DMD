#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define SOFTAP_SSID "NTP"
#define SOFTAP_PASSWORD "87654321"
#define SERVER_PORT 2000

const char *ssid     = "bugs";
const char *password = "04546412889";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 10800, 60000);

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(IPAddress(192, 168, 35, 35), IPAddress(192, 168, 35, 35), IPAddress(255, 255, 255, 0));
  WiFi.softAP(SOFTAP_SSID, SOFTAP_PASSWORD, 6, 0);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);

  ArduinoOTA.begin();
  server.begin();
  timeClient.begin();
  
}

void loop() {
  timeClient.update();

  
}
