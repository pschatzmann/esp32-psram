#include "esp32-psram.h"

// Example data structure
struct SensorData {
  float temperature;
  float humidity;
  uint32_t timestamp;
  
  // For printing
  void print() {
    Serial.printf("Time: %lu, Temp: %.1f°C, Humidity: %.1f%%\n", 
                 timestamp, temperature, humidity);
  }
};

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(100);
  
  Serial.println("ESP32 Typed Ring Buffer Example");
  Serial.println("===============================");
  
  // Create ring buffer for sensor data in PSRAM or HIMEM with capacity for 20 readings
  TypedRingBufferPSRAM<SensorData> dataBuffer(20);
  //TypedRingBufferHIMEM<SensorData> dataBuffer(20);
  
  // Fill buffer with simulated sensor readings
  Serial.println("Adding sensor data to buffer...");
  for (int i = 0; i < 15; i++) {
    SensorData data = {
      .temperature = 20.0f + random(100) / 10.0f,
      .humidity = 40.0f + random(200) / 10.0f,
      .timestamp = millis()
    };
    
    dataBuffer.push(data);
    delay(100); // Small delay to get different timestamps
  }
  
  Serial.printf("Buffer contains %d items (capacity: %d)\n", 
               dataBuffer.available(), dataBuffer.capacity());
  
  // Read all data from the buffer
  Serial.println("\nReading all sensor data:");
  SensorData data;
  while (dataBuffer.pop(data)) {
    data.print();
  }
  
  Serial.println("\nBuffer is now empty");
  
  // Demonstrate overwrite behavior
  Serial.println("\nFilling buffer to capacity and then some (with overwrite)...");
  for (int i = 0; i < 25; i++) {
    SensorData data = {
      .temperature = 25.0f + i * 0.1f,
      .humidity = 50.0f + i * 0.2f,
      .timestamp = millis()
    };
    
    bool overwritten = dataBuffer.pushOverwrite(data);
    if (overwritten) {
      Serial.printf("Item %d caused oldest data to be overwritten\n", i);
    }
    delay(50);
  }
  
  // Peek at data without removing
  Serial.println("\nPeeking at first 3 items without removing them:");
  for (int i = 0; i < 3; i++) {
    if (dataBuffer.peekAt(i, data)) {
      Serial.printf("Item %d: ", i);
      data.print();
    }
  }
  
  // Verify buffer state
  Serial.printf("\nBuffer still contains %d items\n", dataBuffer.available());
  
  // Clear the buffer
  dataBuffer.clear();
  Serial.printf("After clearing, buffer contains %d items\n", dataBuffer.available());
}

void loop() {
  // Nothing here
}