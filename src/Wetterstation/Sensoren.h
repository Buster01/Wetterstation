 // Library für BMP085
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Library für Dallas 18B20
#include <DallasTemperature.h>
#include <OneWire.h>
OneWire oneWire(DS18B20_PIN);          
DallasTemperature DS18B20(&oneWire);

//Library für DHT 22
#include <DHT.h>
DHT dht(DHTPIN, DHTTYPE);

void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
}
  
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
      while(1);
    }
}

String get_ds1820_aussentemp() {
  //Serial.println("Lese Aussentemperatur Sensor aus!");
  String temp = "";
    
  DS18B20.requestTemperatures();
  temp = String(DS18B20.getTempCByIndex(1));
  temp.trim();
  return temp;
  }

String get_ds1820_innentemp() {
  //Serial.println("Lese Innentemperatur Sensor aus!");
  String temp = "";
    
  DS18B20.requestTemperatures();
  temp = String(DS18B20.getTempCByIndex(0));
  temp.trim();
  return temp;
  }

String get_bmp180_druck() {
  //Serial.println("Lese Luftdruck Sensor aus!");
  sensors_event_t event;
  float bmp_druck = 0.00;
  String druck = "";
  
  bmp.getEvent(&event);
  if (event.pressure)
  {
    // Luftdruck lesen und umwandeln in string
    bmp_druck = event.pressure;
    druck = String(bmp_druck);
    druck.trim();
    return druck;
  }
}

String get_dht_feuchte(){
  //Serial.println("Lese Luftfeuchte Sensor aus!");
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
  Serial.println("Lese Windsensor Sensor aus!");
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



