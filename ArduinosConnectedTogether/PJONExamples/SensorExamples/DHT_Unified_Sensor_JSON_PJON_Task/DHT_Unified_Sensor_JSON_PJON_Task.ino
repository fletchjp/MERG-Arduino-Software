// DHT_Unified_Sensor_JSON_PJON_Task.ino
// Using JSON and PJON to transmit data.
// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// Add task management
// THIS WORKS. I did not expect this to work at all.

#include <TaskManagerIO.h>

// Add serialization using the ArduinoJSON library.
// Also add PJON to transmit the data.

#include <PJONSoftwareBitBang.h>

PJONSoftwareBitBang bus(45);

/// error_handler  
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if(code == PJON_CONNECTION_LOST) {
    Serial.print("Connection lost with device id ");
    Serial.println(bus.packets[data].content[0], DEC);
    // Avoid Serial interference during test flushing
    Serial.flush();
  }
  if(code == PJON_PACKETS_BUFFER_FULL) {
    Serial.print("Packet buffer is full, has now a length of ");
    Serial.println(data, DEC);
    Serial.println("Possible wrong bus configuration!");
    Serial.println("higher PJON_MAX_PACKETS if necessary.");
    Serial.flush();
  }
  if(code == PJON_CONTENT_TOO_LONG) {
    Serial.print("Content is too long, length: ");
    Serial.println(data);
    Serial.flush();
  }
};

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'X') {
    Serial.println("BLINK");
    // Avoid Serial interference during test flushing
    Serial.flush();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
  } else { 
    for (byte i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println(" ");
    Serial.flush();
  }
};

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <ArduinoJson.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)
#define COUNT 6 // This value does not seem to be used. 6 is the default value.
#define TEMP_ID 1 // ID no for temperature measurement. Default -1.
#define HUMIDITY_ID 2 // ID for humidity measuerment. Default -1.

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

//DHT_Unified dht(DHTPIN, DHTTYPE);
DHT_Unified dht(DHTPIN, DHTTYPE, COUNT, TEMP_ID, HUMIDITY_ID);

uint32_t delayMS;

#define ARRAY_SIZE 60
#define DATA_ARRAY_SIZE 30

void setup() {
  Serial.begin(115200);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example with JSON and PJON"));
  // JSON document to describe the sensor
  JsonDocument temperature_sensor;

  // Print temperature sensor details.
  sensor_t sensor;
  String buffer;
  char output_array[ARRAY_SIZE];
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  temperature_sensor["name"] = "Temperature Sensor";
  temperature_sensor["type"] = sensor.name;
  temperature_sensor["version"] = sensor.version;
  serializeJson(temperature_sensor, Serial);
  size_t count = serializeJson(temperature_sensor, buffer);
  size_t array_count = serializeJson(temperature_sensor, output_array);
  Serial.println("");
  Serial.print("Size of temperature_sensor data ");
  Serial.println(measureJson(temperature_sensor));
  Serial.println(buffer);
  Serial.print("sizeof(buffer) reports ");
  Serial.println(sizeof(buffer)); //This is 6.
  Serial.print("buffer.length() reports ");
  Serial.println(buffer.length()); //This is 6 which is the number of items.
  Serial.print("count of data written to buffer = ");
  Serial.println(count);
  Serial.println(output_array);
  Serial.print("count of data written to output_array = ");
  Serial.println(array_count);
  Serial.println(F("------------------------------------"));

  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));

  Serial.flush();
    pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Initialize LED 13 to be off

  bus.strategy.set_pin(12);
  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.begin();
  bus.send_repeatedly(44, "X", 1, 1000000); // Send B to device 44 every second

  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  //taskManager.scheduleFixedRate(5000,task_measure_environment); // old style
  taskManager.schedule(repeatSeconds(5),task_measure_environment);

}
 
bool new_data;
char data_array[DATA_ARRAY_SIZE];


void task_measure_environment() {
  // Delay between measurements.
  //delay(delayMS);
  JsonDocument json_data;
  //char data_array[DATA_ARRAY_SIZE];
  new_data = false;
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    Serial.flush();
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    json_data["temp"] = event.temperature;
    new_data = true;
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    Serial.flush();
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    json_data["humid"] = event.relative_humidity;
    new_data = true;
  }
  if (new_data) {
    serializeJson(json_data, Serial);
    Serial.println("");
    size_t data_array_count = serializeJson(json_data, data_array);
    Serial.println(data_array_count);
    Serial.flush();
      bus.send(40, &data_array, DATA_ARRAY_SIZE);
      Serial.println("data_array sent");
      Serial.flush();    
  }
}


void loop() {
  taskManager.runLoop();
  bus.update();
  /*{
    if(new_data) {
      bus.send(44, &data_array, DATA_ARRAY_SIZE);
      Serial.println("data_array sent");
      Serial.flush();    
    }
  } */
  bus.receive(1000);
}
