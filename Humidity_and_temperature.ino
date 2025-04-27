#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

const char *ssid = "Optus_508297";
const char *password = "heezegytes22umL";
const char *server = "api.thingspeak.com";
WiFiClient client;

unsigned long myChannelNumber = 2936793;
const char *myWriteAPIKey = "RM4NH65UDV0BCAVV";

DHT dht(2, DHT22);  // DHT11 sensor on pin 2

void setup() {
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);
}

void loop() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  ThingSpeak.setField(1, temp);   // Field 1 for temperature
  ThingSpeak.setField(2, humidity); // Field 2 for humidity

  int status = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (status == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Failed to send data to ThingSpeak");
  }

  delay(60000);  // Send data every 60 seconds
}
