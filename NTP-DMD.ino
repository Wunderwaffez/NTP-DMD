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

#include "DHT.h"
#include <DS18B20.h>


#include <SPI.h>
#include <DMDESP.h>
#include <fonts/angka6x13.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/ElektronMart5x6.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DMDESP dmd(2, 1);

const char *ssid     = "bugs";
const char *password = "04546412889";

String humid;
String sicaklik;
String temp;

int last_second = 0;
int last_second2 = 0;

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP, "europe.pool.ntp.org", 10800, 300000);
DHT dht(DHTPIN, DHTTYPE);
DS18B20 ds(4);

uint8_t address[] = {40, 250, 31, 218, 4, 0, 0, 52};

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
  dht.begin();

  ds.select(address);

  dmd.setBrightness(1);
  dmd.setFont(SystemFont5x7);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    ESP.restart();
  }
  Serial.println(WiFi.localIP());

  int h = dht.readHumidity();
  humid = "%" + String(h);

  float t = ds.getTempC();
  temp = String(t, 3);

}

void loop() {
  dmd.loop();

  ArduinoOTA.handle();

  dmd.setFont(SystemFont5x7);

  if (millis() / 1000 % 2 == 0)
  {
    dmd.drawChar(14, 4, ':');
  }
  else
  {
    dmd.drawChar(14, 4, ' ');
  }

  if (millis() - last_second2 > 300000) {

    last_second2 = millis();

    dmd.clear();

    int h = dht.readHumidity();
    humid = "%" + String(h);

    float t = ds.getTempC();
    temp = String(t, 3);

  }

  if (millis() - last_second > 1000) {
    last_second = millis();
    ntp.update();

    int saat = ntp.getHours();
    int dakika = ntp.getMinutes();
    String saat0;
    String dakika0;

    dakika0 = String(dakika);
    if (dakika < 10) {
      dakika0 = '0' + dakika0;
    }
    else dakika0 = dakika0;

    saat0 = String (saat);
    if (saat < 10) {
      saat0 = ("0") + (saat0);
    }
    else {
      saat0 = saat0;
    }

    dmd.setFont(angka6x13);

    dmd.drawText(0, 0, String(saat0));
    dmd.drawText(19, 0, String(dakika0));

    dmd.setFont(ElektronMart5x6);

    //dmd.drawLine(33, 0, 33, 16);
    
    dmd.setPixel(56,0,1);
    dmd.setPixel(56,1,1);
    dmd.setPixel(56,2,1);

    dmd.setPixel(57,2,1);
    dmd.setPixel(57,0,1);
    
    dmd.setPixel(58,0,1);
    dmd.setPixel(58,1,1);
    dmd.setPixel(58,2,1);

    
/* -----------------------------
 
    dmd.setPixel(53,9,1);
    
    dmd.setPixel(52,10,1);
    dmd.setPixel(53,10,1);
    dmd.setPixel(54,10,1);
    
    dmd.setPixel(52,11,1);
    dmd.setPixel(53,11,1);
    dmd.setPixel(54,11,1);
    
    dmd.setPixel(52,12,1);
    dmd.setPixel(53,12,1);
    dmd.setPixel(54,12,1);
    
    dmd.setPixel(52,13,1);
    dmd.setPixel(53,13,1);
    dmd.setPixel(54,13,1);
    
    dmd.setPixel(52,14,1);
    dmd.setPixel(53,14,1);
    dmd.setPixel(54,14,1);

    dmd.setPixel(51,12,1);
    dmd.setPixel(55,12,1);
    
    dmd.setPixel(51,13,1);
    dmd.setPixel(55,13,1);

*/

    dmd.drawText(35, 0, temp, 0, 4);
    dmd.drawText(35, 8, humid);
    dmd.drawText(60, 0, "C");


  }
}
