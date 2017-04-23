
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  WLAN_RSSI = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(WLAN_RSSI);
  Serial.println(" dBm");
}

boolean connect_wlan(unsigned long beginn) {
  WiFi.hostname (host_name);
  int counter = 0;
  int status = WL_IDLE_STATUS;
  
  Serial.print("WLAN SSID: ");
  Serial.println(ssid);
  
  status = WiFi.begin(ssid, wlan_passwd);
  while ( status != WL_CONNECTED) {
      counter++;
      status = WiFi.status();
      delay(1000);
      Serial.print(".");
      if (counter > 15) {
        Serial.println("fehlerhaft!");
        return false;
      }
  }
  printWifiStatus();
  Serial.println("OK");
  return true;
}

