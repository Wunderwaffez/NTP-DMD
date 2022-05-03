/* P10 MonoColor Hardware Connections:
            ------IDC16 IN------
  CS/GPIO15/D8  |1|   |2|  D0/GPIO16
            Gnd |3|   |4|  D6/GPIO12/MISO
            Gnd |5|   |6|  X
            Gnd |7|   |8|  D5/GPIO14/CLK
            Gnd |9|   |10| D3/GPIO0
            Gnd |11|  |12| D7/GPIO13/MOSI
            Gnd |13|  |14| X
            Gnd |15|  |16| X
            --------------------  */

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <SPI.h>
#include <DMDESP.h>
#include <fonts/angka6x13.h>
#include <fonts/SystemFont5x7.h>

DMDESP dmd(2,1);

const char *ssid     = "bugs";
const char *password = "04546412889";

int last_second = 0;

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP, "europe.pool.ntp.org", 10800, 300000);

void setup() {

  last_second = millis();
  
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  ArduinoOTA.setHostname("NTP");

  dmd.start();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  ArduinoOTA.begin();
  ntp.begin();
  
  dmd.setBrightness(1);
  dmd.setFont(SystemFont5x7);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    ESP.restart();
  }
    Serial.println(WiFi.localIP());

}

void loop() {
  dmd.loop();
  
  ArduinoOTA.handle();

  dmd.setFont(SystemFont5x7);
  
  if (millis() / 1000 % 2 == 0)
  {
    dmd.drawChar(14, 5, ':');
  }
  else
  {
    dmd.drawChar(14, 5, ' ');
  }

  if (millis() - last_second > 1000){
  last_second = millis();   
  ntp.update();
  
  int saat = ntp.getHours();
  int dakika = ntp.getMinutes();
  String saat0;
  String dakika0;
  
  dakika0 = String(dakika);
    if (dakika < 10) {dakika0 = '0' + dakika0;}
    else dakika0 = dakika0;

    saat0 = String (saat);
    if (saat < 10) {saat0 = ("0") + (saat0);}
    else {saat0 = saat0;}

  dmd.setFont(angka6x13);

  dmd.drawText(0, 0, String(saat0));
  dmd.drawText(19, 0, String(dakika0));

  dmd.setFont(SystemFont5x7);
  dmd.drawText(33, 0, "anky");
  
  }
}
