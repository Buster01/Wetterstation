 // Library für BMP085
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Library für Dallas 18B20
OneWire oneWire(DS18B20_PIN);          
DallasTemperature DS18B20(&oneWire);

//Library für DHT 22
DHT dht(DHTPIN, DHTTYPE);
  
void detect_sensors(void){
    // Sensoren 
    Serial.println("Suche Sensoren!");
    dht.begin(); // DHT 22 Sensor
    delay(3000);
    DS18B20.begin(); // Dallas 18b20
    delay(2000);
  
    // dallas Sensor Init
    Serial.print("Anzahl Dallas Temperatur Sensoren: "); Serial.println(DS18B20.getDeviceCount()); 
    DS18B20.setResolution(TEMPERATURE_PRECISION);
    Serial.print ("Setzte DS1B20 Temperatur Praezision auf ");
    Serial.print (TEMPERATURE_PRECISION);
    Serial.println ("Bit");
     
    if(!bmp.begin()) {
      Serial.print("Ooops, kein BMP085 gefunden ... Pruefe die Verkabelung oder die I2C Adresse!");
    }
}

String get_ds1820_aussentemp() {
  String temp = "";
    
  DS18B20.requestTemperatures();
  temp = String(DS18B20.getTempCByIndex(1));
  temp.trim();
  return temp;
  }

String get_ds1820_innentemp() {
  String temp = "";
    
  DS18B20.requestTemperatures();
  temp = String(DS18B20.getTempCByIndex(0));
  temp.trim();
  return temp;
  }

String get_bmp180_druck() {
  sensors_event_t event;
  float bmp_druck = 0.00;
  String druck = "";
  
  bmp.getEvent(&event);
  if (event.pressure)
  {
    bmp_druck = event.pressure;
    druck = String(bmp_druck);
    druck.trim();
    return druck;
  }
}

String get_dht_feuchte(){
  String feuchte = "";
  feuchte = String(dht.readHumidity());
  feuchte.trim();
  return feuchte;
}

String taupunkt(float temp, float feuchte){
  String taupunkt = "";
  float a = 17.62;
  float b = 243.12;
  float taupunktTemp = (a * temp ) / (b + temp) + log(feuchte/100);
  float p = (b * taupunktTemp) / (a - taupunktTemp);
  taupunkt = String(p);
  taupunkt.trim();
  return taupunkt;
}

float Spannung_berechnen(int sensor_wert) {
  
  float R1 = 10000.0; // Widerstandswert R1 (20k) 
  float R2 = 30000.0; // Widerstandswert R2 (20K)
  float spannung = 0.0;
  float value = 0.0;

  value = (sensor_wert * 3.2) / 1024;
  spannung = value /(R2 / (R1 + R2));
  return spannung;
}

void windpulscallback() {
  pulses++;
}


float calculateSpeed(int intervall) {
  Serial.println("Starte Messung der Windgeschwindigkeit!");
  unsigned long start;
  unsigned long end;
  unsigned long diff;
  float current_speed;

  attachInterrupt(WindSensorPin, windpulscallback, RISING);

  // Messung starten
  pulses=0;
  start=micros();
  delay(intervall); // in milliseconds
  end=micros();
  detachInterrupt(WindSensorPin);
  
  diff=end-start;
  current_speed=pulses * 1000.0f *(intervall) / 6.0f / diff;
  Serial.print  ("Windgeschwindigkeit: ");
  Serial.print  (current_speed);
  Serial.println(" km/h");
  return current_speed;
}

String WindChill (double temperatur, double wind) {
  float chill;

  if (temperatur < 10) {
    chill= 13.12 +(0.6215 * temperatur) - (11.37 * pow(wind, 0.16)) + (0.3965 * temperatur * pow(wind, 0.16));
  }
  else {
    chill = temperatur;
  }
  return String(chill);
}



