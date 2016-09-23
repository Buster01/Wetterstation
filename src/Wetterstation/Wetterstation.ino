const char* ssid = "SSID";  // SSID deines WLAN Netzwerkes
const char* wlan_passwd = "PassW0rd"; // Passwort deines WLAN Netzwerkes

const char* mqtt_server = "MQTT-Server"; // MQTT Server

unsigned long ChannelNumber = 123; // Thingspeak Channel Nummer
const char * WriteAPIKey = "API-KEY"; // Thingspeak Write API -Key
#define write2TS true // Bei true werden die Daten auch zu Thingspeak geschrieben, bei false nicht

#define AkkuPin A0
#define WindSensorPin 2
#define DHTPIN 1
#define DS18B20_PIN 0
#define ResetPIN  7

#define DHTTYPE DHT22
#define TEMPERATURE_PRECISION 12


unsigned long pulses;
String WLAN_RSSI = "";
unsigned long beginn = 0;

#include "ThingSpeak.h"
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
  float wind_geschw = calculateSpeed(wind_mess_intervall);
  
  String ldruck = get_bmp180_druck();
  String atemp = get_ds1820_aussentemp();
  String itemp = get_ds1820_innentemp();
  
  int sensorValue = analogRead(AkkuPin); // AkkuSpannung messen
  float aspannung = Spannung_berechnen(sensorValue);
  Serial.println("Akku Sensor Wert:" + String(sensorValue));
  Serial.println("Akkkuspannunng: " + String(aspannung) + " V");
  
  String lfeuchte = get_dht_feuchte();
  
  if (!connect_wlan(beginn)) 
  {
    Serial.println("Keine WLAN Verbindung moeglich! Reset!");
    rtc_sleep(sleepTime(aspannung, beginn));
  }
  else {
    if (!mqtt_connect()) {
      Serial.println("keine Verbindung! Reset!");
      rtc_sleep(sleepTime(aspannung, beginn));
    }
    else {
      mqtt_daten_senden(String(atemp),String(itemp),String(aspannung), String(lfeuchte),String(ldruck), String(wind_geschw), String(sleepTime(Spannung_berechnen(sensorValue), beginn)), write2TS);
    }
    Serial.println("Arbeit fuer " + String( (millis()-beginn) / 1000) + " Sekunden"); 
    sleep(sleepTime(aspannung, beginn));
  }
  // Reset wegen stabilität
  hw_reset();
}

