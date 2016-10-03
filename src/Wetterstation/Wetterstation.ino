const char* ssid = "SSID";  // SSID deines WLAN Netzwerkes
const char* wlan_passwd = "PassW0rd"; // Passwort deines WLAN Netzwerkes

const char* mqtt_server = "MQTT-Server"; // MQTT Server

unsigned long ChannelNumber = 123; // Thingspeak Channel Nummer
const char * WriteAPIKey = "API-KEY"; // Thingspeak Write API -Key
#define write2TS true

#define AkkuPin A0
#define DS18B20_PIN 0
#define DHTPIN 1
#define WindSensorPin 4
#define ResetPIN  7

#define DHTTYPE DHT22
#define TEMPERATURE_PRECISION 12

unsigned long pulses;
String WLAN_RSSI = "";
unsigned long beginn = 0;

#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <DHT.h>
#include <ThingSpeak.h>


#include "MKR1000_WLAN.h"
#include "Funktionen.h"
#include "Sensoren.h"
#include "MQTT.h"

void setup() {
  beginn = millis();
  
  Serial.begin(115200);
  pinMode(WindSensorPin, INPUT_PULLUP);
  
  Serial.println(" ");
  Serial.println("Wetterstation startet!");
  
  //HW - reset 
  pinMode(ResetPIN, OUTPUT);
  detect_sensors(); // Sensoren finde
}

void loop() {
  int wind_mess_intervall = 5000;
  float max_wind = 0;
  float min_wind = 999;
  float ds_wind = 0;
  int wind_mess_wiederholung = 0;
  float wind_geschw = 0;
  int w = 0;

  int sensorValue = analogRead(AkkuPin); // AkkuSpannung messen
  float aspannung = Spannung_berechnen(sensorValue);
  Serial.println("Akkkuspannunng: " + String(aspannung) + " V");

  wind_mess_wiederholung = abs((sleepTime(aspannung, beginn) - 15) / 5) + 1;
  Serial.println("Windmesswiederholung: " + wind_mess_wiederholung);
  for (w = 1; w < wind_mess_wiederholung; w++) {
    wind_geschw = calculateSpeed(wind_mess_intervall);
    ds_wind = ds_wind + wind_geschw;
    if (wind_geschw > max_wind) {max_wind = wind_geschw;}
    if (wind_geschw < min_wind) {min_wind = wind_geschw;}
  }
  ds_wind = (ds_wind + wind_geschw) / w;

  String ldruck = get_bmp180_druck();
  String atemp = get_ds1820_aussentemp();
  String itemp = get_ds1820_innentemp();  
  String lfeuchte = get_dht_feuchte();

  if (!connect_wlan(beginn)) 
  {
    Serial.println("Keine WLAN Verbindung moeglich! Reset!");
    sleep(sleepTime(aspannung, beginn));
  }
  else {
    if (!mqtt_connect()) {
      Serial.println("keine Verbindung! Reset!");
      sleep(sleepTime(aspannung, beginn));
    }
    else {         
      mqtt_daten_senden(String(atemp),String(itemp),String(aspannung), String(lfeuchte),String(ldruck), String(ds_wind), String(min_wind), String(max_wind), String(sleepTime(Spannung_berechnen(sensorValue), beginn)), write2TS);
    }
    Serial.println("Arbeit fuer " + String( (millis() - beginn) / 1000) + " Sekunden"); 
    Serial.println("Schlafe fuer " + String(sleepTime(Spannung_berechnen(sensorValue), beginn)) + " Sekunden!");

    sleep(sleepTime(aspannung, beginn));
  }
  // Reset wegen stabilität
  hw_reset();
}

