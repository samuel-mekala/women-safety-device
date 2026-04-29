#include <SoftwareSerial.h>
#include <DHT.h>
#define DHTPIN D1           // Pin connected to the DHT sensor
#define DHTTYPE DHT11  // Change to DHT22 if you're using the DHT22 sensor
SoftwareSerial bluetoothSerial(D2, D3);  // RX, TX pins for the Bluetooth module
DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(9600);                 // Initialize the serial monitor
  dht.begin();                        // Initialize the DHT sensor
  bluetoothSerial.begin(9600);        // Initialize the Bluetooth module
}

void loop() {
  dht.begin();
  float temperature = dht.readTemperature(); 
  delay(500); // Read temperature from the DHT sensor
  float humidity = dht.readHumidity();   
  delay(500);     // Read humidity from the DHT sensor

  //if (!isnan(temperature) && !isnan(humidity)) {  // Check if the sensor readings are valid
    Serial.println("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.print(" %");


    bluetoothSerial.println("Temperature: ");
    bluetoothSerial.println(temperature);
    bluetoothSerial.println(" °C, Humidity: ");
    bluetoothSerial.println(humidity);
    bluetoothSerial.println(" %");
  // }

  delay(5000);  
}
