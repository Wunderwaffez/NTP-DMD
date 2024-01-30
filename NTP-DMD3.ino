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
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "DHT.h"
#include <DS18B20.h>
// DS18B20_RT LIBRARY
#include <OneWire.h>


#include <SPI.h>
#include <DMDESP.h>
#include <fonts/angka6x13.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/ElektronMart5x6.h>
#include <fonts/ElektronMart6x16.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define ONE_WIRE_BUS 4

const char *ssid     = "wifi";
const char *password = "password";

String humid;
String sicaklik;
String temp;

String text1 = "";
String text2 = "";

ESP8266WebServer server(80);

int last_second = 0;
int last_second2 = 0;
int durum = 0;

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP, "europe.pool.ntp.org", 10800, 300000);
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DS18B20 ds(&oneWire);
DMDESP dmd(2, 1);


uint8_t address[] = {40, 250, 31, 218, 4, 0, 0, 52};

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP8266 NTP SAAT</h1>";
  html += "<form action='/submit' method='post'>";
  html += "Ust Satir : <input type='text' name='text1'><br>";
  html += "Alt Satir: <input type='text' name='text2'><br>";
  html += "<br>";
  html += "<input type='submit' value='GONDER'>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleSubmit() {
  if (server.hasArg("text1")) {
    text1 = server.arg("text1");
  }
  if (server.hasArg("text2")) {
    text2 = server.arg("text2");
  }

  durum++;

  dmd.clear();

  server.send(200, "text/plain", "Veri Gonderildi...");
}

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

  ds.requestTemperatures();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/submit", HTTP_POST, handleSubmit);
  server.begin();

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

  server.handleClient();

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

    durum = 0;
    
    ds.requestTemperatures();
    
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

    if (saat < 6) {dmd.setBrightness(0);} 
    else {dmd.setBrightness(1);} 

    dmd.setFont(ElektronMart6x16);

    dmd.drawText(0, 0, String(saat0));
    dmd.drawText(19, 0, String(dakika0));

    dmd.setFont(ElektronMart5x6);

    //dmd.drawLine(33, 0, 33, 16);

    if (durum == 0){ 

      dmd.setPixel(56,0,1);
      dmd.setPixel(56,1,1);
      dmd.setPixel(56,2,1);

      dmd.setPixel(57,2,1);
      dmd.setPixel(57,0,1);
    
      dmd.setPixel(58,0,1);
      dmd.setPixel(58,1,1);
      dmd.setPixel(58,2,1);

      dmd.drawText(35, 0, temp, 0, 4);
      dmd.drawText(35, 8, humid);
      dmd.drawText(60, 0, "C");

    }
    else {
      dmd.drawText(35, 0, text1);
      dmd.drawText(35, 8, text2);
    }


  }
}
