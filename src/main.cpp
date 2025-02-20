#include <Arduino.h>
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>

#define DHTTYPE DHT11
#define DHTPIN 4
DHT_Unified dht(DHTPIN,DHTTYPE);

WiFiClient wificlient;

const char* ssid = "ELEM 6688";
const char* password = "Duyhung3286!";

unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "1UMIE8UX09NRRP3K";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.println("\nConnecting to WiFi network ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to WiFi Network");
  Serial.println("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  dht.begin();
  ThingSpeak.begin(wificlient);
}

void loop() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }else 
  {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    int x = ThingSpeak.writeField(myChannelNumber,2,event.temperature,myWriteAPIKey);
    if (x == 200)
    {
      Serial.println("Channel update successfull!");
    }else
    {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }
  delay(1000);
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Humidity: "));
  }else 
  {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    int y = ThingSpeak.writeField(myChannelNumber,1,event.relative_humidity,myWriteAPIKey);
    if (y == 200)
    {
      Serial.println("Channel update successfull!");
    }else
    {
      Serial.println("Problem updating channel. HTTP error code " + String(y));
    }
  }
  delay(1000);
}
