int sleepTime (float ASpannung, unsigned long beginn) {
  unsigned long sleep;
  
  if (ASpannung >= 4) { sleep = 60; }
  if ((ASpannung < 4.0) && (ASpannung >= 3.9)) { sleep = 120;} 
  if ((ASpannung < 3.9) && (ASpannung >= 3.8)) { sleep = 180;}
  if ((ASpannung < 3.8) && (ASpannung >= 3.7)) { sleep = 240;}
  if ((ASpannung < 3.7) && (ASpannung >= 3.6)) { sleep = 300;}
  if ((ASpannung < 3.6) && (ASpannung >= 3.5)) { sleep = 360;}
  if ((ASpannung < 3.5) && (ASpannung >= 3.4)) { sleep = 420;}
  if ((ASpannung < 3.4) && (ASpannung >= 3.3)) { sleep = 480;}
  if ((ASpannung < 3.3) && (ASpannung >= 3.2)) { sleep = 600;}
  if (ASpannung < 3.2) { sleep = 900;}
  
  sleep = (sleep - ((millis()-beginn)/1000));
  
  if (sleep <= 10 ) {sleep = 10;}
  if (sleep > 900) {sleep = 900;}
  return (int)sleep;
}

void hw_reset(void) {
  // Hardware Reset benötigt eine Verbindung mit einem GPIO PIN und dem Reset Eingang
  Serial.println ("HW-Reset!");
  delay(250);
  digitalWrite(ResetPIN, HIGH); 
}

void sleep(int sleepTime) {
  byte minute =0;
  byte sekunde = 0;

  // WLAN abschalten
  WiFi.maxLowPowerMode();
  WiFi.end();
  delay(sleepTime * 1000);
}






