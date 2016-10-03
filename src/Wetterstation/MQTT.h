WiFiClient wlanClient;
PubSubClient mqtt_client(wlanClient);

// MQTT Callback
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

boolean mqtt_connect(void) {
  int counter = 0;

  mqtt_client.setServer(mqtt_server, 1883);
  //mqtt_client.setCallback(callback);
  
  mqtt_client.connect("Wetterstation");
  Serial.print("MQTT Connection: ");
  while (!mqtt_client.connected()) 
  {
    delay(500);
    Serial.print(".");
    counter++;
    if (counter > 15 ) {
      Serial.println("Verbindung konnte nicht aufgebaut werden!");
      return false;
    }
  }
  Serial.println ("OK");
  return true;
}


void mqtt_daten_senden(String aTemp, String iTemp, String ASpannung, String lFeuchte, String lDruck, String Wind, String(min_wind), String(max_wind), String sleepT, boolean write2ThingSpeak) {
  ThingSpeak.begin(wlanClient);
  String t_punkt = taupunkt(aTemp.toFloat(), lFeuchte.toFloat());
  String chill = WindChill(aTemp.toFloat(), Wind.toFloat());

  mqtt_client.publish("/openHAB/Wetterstation/Temperatur1", aTemp.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/wsTemp", iTemp.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/Akkuspannung", ASpannung.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/Luftfeuchte", lFeuchte.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/Luftdruck", lDruck.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/Wind", Wind.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/minWind", min_wind.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/maxWind", max_wind.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/WindChill", chill.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/Taupunkt", t_punkt.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/SleepTime", sleepT.c_str());
  mqtt_client.publish("/openHAB/Wetterstation/RSSI", WLAN_RSSI.c_str());
  mqtt_client.loop();

  if (write2ThingSpeak == true) {
    ThingSpeak.setField(1,aTemp.c_str());
    ThingSpeak.setField(2,lFeuchte.c_str());
    ThingSpeak.setField(3,lDruck.c_str());
    ThingSpeak.setField(4,Wind.c_str());
    ThingSpeak.setField(5,t_punkt.c_str());
    ThingSpeak.setField(6,ASpannung.c_str());
    ThingSpeak.writeFields(ChannelNumber, WriteAPIKey);
  }
  delay(250);
  mqtt_client.disconnect();
}


