// REQUIRES the following Arduino libraries:
#include <WiFi.h>
#include "DHT.h"

// Initialize DHT sensor.
#define DHTTYPE DHT11
uint8_t DHTPin = 27;
DHT dht(DHTPin, DHTTYPE);

int dustPin = 35; // dust sensor - Wemos A0 pin
int ledPin = 12;    

float voltsMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

const char* ssid     = "FD-82";
const char* password = "Zxcvbnm@123";
const char* host = "192.168.43.4";

void setup()
{ 
    Serial.begin(115200);

    Serial.println("Setup Dust Sensor Pinmode");
    pinMode(ledPin,OUTPUT);
      
    Serial.println("Setup DHT11 Pinmode");
    pinMode(DHTPin, INPUT);
    dht.begin();
    
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}


void loop()
{
  // read temp and humidity from dht11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if(isnan(temperature) || isnan(humidity)){
    Serial.println("Failed to read DHT11");
  }else{
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
    delay(5000);
  }

  digitalWrite(ledPin,LOW); // power on the LED
  delayMicroseconds(280);
 
  voltsMeasured = analogRead(dustPin); // read the dust value
 
  delayMicroseconds(40);
  digitalWrite(ledPin,HIGH); // turn the LED off
  delayMicroseconds(9680);
 
  //measure your 5v and change below
  calcVoltage = voltsMeasured * (3.3 / 1024.0);
  dustDensity = 0.17 * calcVoltage - 0.1;
  Serial.println("GP2Y1010AU0F readings"); 
  Serial.print("Raw Signal Value = ");
  Serial.println(voltsMeasured); 
  Serial.print("Voltage = ");
  Serial.println(calcVoltage);
  Serial.print("Dust Density = ");
  Serial.println(dustDensity); // mg/m3
  Serial.println("");
  delay(1000);
  
  Serial.print("connecting to ");
  Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // This will send the request to the server
 client.print(String("GET http://192.168.43.4/air_quality/DHT11.php?") + 
                          ("&temperature=") + temperature +
                          ("&humidity=") + humidity +
                          ("&raw_measurement=") + voltsMeasured +
                          ("&dust_density=") + dustDensity +
                          " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        
    }

    Serial.println();
    Serial.println("closing connection");
}
