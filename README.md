# ESP32-PSRAM Library

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue.svg)](https://www.arduino.cc/reference/en/libraries/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A comprehensive memory management library for ESP32 microcontrollers, providing efficient data structures that leverage __PSRAM__ and __HIMEM__ for expanded storage capabilities.

## Overview

The ESP32 can only address 4MB of PSRAM directly, so the remaining 4MB (HIMEM) are usually unsued!

ESP32-PSRAM is a C++ library that helps you manage large amounts of data on ESP32 devices by utilizing the full external PSRAM (SPI RAM) supporting the high memory (HIMEM) regions. This allows your applications to work with much larger datasets than would be possible using just internal RAM. 


- **Memory-Efficient Data Structures**:
  - `VectorPSRAM`: Vector implementation that automatically stores data in PSRAM
  - `VectorHIMEM`: Vector implementation for extremely large datasets using ESP32's high memory region
  
- **File System Abstractions**:
  - `FilePSRAM`: File-like interface backed by PSRAM
  - `FileHIMEM`: File-like interface backed by high memory
  - SD card-like API for familiar file operations
  
- **Streaming Data Handling**:
  - `RingBufferRAM`: Circular buffer implementation in RAM (Stream-based)
  - `RingBufferPSRAM`: Circular buffer implementation in PSRAM (Stream-based)
  - `RingBufferHIMEM`: Circular buffer implementation in high memory (Stream-based)
  - Fully compatible with Arduino's Stream class
  
- **Typed Ring Buffers**:
  - `TypedRingBufferRAM<T>`: Type-safe circular buffer for any data type using RAM
  - `TypedRingBufferPSRAM<T>`: PSRAM version for storing complex data structures
  - `TypedRingBufferHIMEM<T>`: High memory version for storing complex data structures
  - Optimized for struct/class storage with proper memory management

## Installation

### Arduino IDE

1. In Arduino IDE, go to **Sketch > Include Library > Manage Libraries...**
2. Search for "ESP32-PSRAM"
3. Click "Install"

### Manual Installation

1. Download the latest release from GitHub
2. Extract the ZIP file
3. Move the folder to your Arduino libraries directory (typically `~/Arduino/libraries/` on Linux/macOS or `Documents\Arduino\libraries\` on Windows)
4. Restart Arduino IDE

## Usage Examples

### Basic Vector Usage

```cpp
#include "ESP32-PSRAM.h"

void setup() {
  Serial.begin(115200);
  
  // Create a vector in PSRAM
  VectorPSRAM<int> psram_vector;
  
  // Add elements
  for(int i = 0; i < 1000; i++) {
    psram_vector.push_back(i);
  }
  
  Serial.printf("Vector size: %d\n", psram_vector.size());
  Serial.printf("First element: %d\n", psram_vector.front());
  Serial.printf("Last element: %d\n", psram_vector.back());
}
```

### File Operations

```cpp
#include "ESP32-PSRAM.h"

void setup() {
  Serial.begin(115200);
  
  // Initialize PSRAM file system
  if (!PSRAM.begin()) {
    Serial.println("PSRAM initialization failed!");
    return;
  }
  
  // Create and write to a file
  auto file = PSRAM.open("test.txt", FILE_WRITE);
  if (file) {
    file.println("Hello PSRAM!");
    file.println("This is a test file.");
    file.close();
    Serial.println("File written successfully");
  }
  
  // Read from the file
  file = PSRAM.open("test.txt", FILE_READ);
  if (file) {
    Serial.println("File contents:");
    while (file.available()) {
      Serial.write(file.read());
    }
    file.close();
  }
}
```

### Stream-Based Ring Buffer

```cpp
#include "ESP32-PSRAM.h"

void setup() {
  Serial.begin(115200);
  
  // Create a ring buffer in PSRAM with 1024 bytes capacity
  RingBufferStreamPSRAM buffer(1024);
  
  // Write data to the buffer
  buffer.print("Hello, Ring Buffer!");
  
  // Read and print the data
  while (buffer.available()) {
    Serial.write(buffer.read());
  }
}
```

### Typed Ring Buffer

```cpp
#include "ESP32-PSRAM.h"

// Define a custom data structure
struct SensorData {
  float temperature;
  float humidity;
  uint32_t timestamp;
};

void setup() {
  Serial.begin(115200);
  
  // Create a typed ring buffer for sensor data
  TypedRingBufferPSRAM<SensorData> dataBuffer(100);
  
  // Add some data
  SensorData reading = {25.4, 68.7, millis()};
  dataBuffer.push(reading);
  
  // Read data back
  SensorData result;
  if (dataBuffer.pop(result)) {
    Serial.printf("Temperature: %.1f°C, Humidity: %.1f%%\n", 
                 result.temperature, result.humidity);
  }
}
```

## API Reference

### Vector Classes

- `VectorPSRAM<T>`: Similar interface to std::vector but stores data in PSRAM
  - `push_back(value)`: Add element to the end
  - `pop_back()`: Remove the last element
  - `size()`: Get number of elements
  - `capacity()`: Get allocated storage capacity
  - `operator[]`: Access element by index
  - `clear()`: Remove all elements
  - `reserve(n)`: Reserve storage for n elements

- `VectorHIMEM<T>`: Similar interface to std::vector but stores data in high memory
  - Same interface as VectorPSRAM

### File Classes

- `PSRAM`: Global instance providing file operations in PSRAM
  - `begin()`: Initialize the PSRAM filesystem
  - `open(filename, mode)`: Open a file
  - `exists(filename)`: Check if a file exists
  - `remove(filename)`: Delete a file
  - `totalBytes()`: Get total PSRAM size
  - `freeBytes()`: Get free PSRAM available

- `HIMEM`: Global instance providing file operations in high memory
  - Same interface as PSRAM but uses high memory

- `FilePSRAM` and `FileHIMEM`: File interface
  - `read()`: Read a byte
  - `write(byte)`: Write a byte
  - `available()`: Check bytes available for reading
  - `seek(pos)`: Set position in file
  - `position()`: Get current position
  - `size()`: Get file size
  - `flush()`: Ensure all data is written
  - `close()`: Close the file

### Ring Buffer Classes

- `RingBufferPSRAM` and `RingBufferHIMEM`: Stream-based circular buffers
  - `available()`: Check bytes available for reading
  - `availableForWrite()`: Check space available for writing
  - `read()`: Read a byte
  - `peek()`: Look at next byte without consuming
  - `write(byte)`: Write a byte
  - `flush()`: Clear buffer
  - `size()`: Get total capacity
  - `used()`: Get number of bytes currently stored
  - `free()`: Get free space

- `TypedRingBufferPSRAM<T>` and `TypedRingBufferHIMEM<T>`: Type-safe circular buffers
  - `push(value)`: Add an element (fails if full)
  - `pushOverwrite(value)`: Add an element, overwriting oldest if full
  - `pop(value)`: Remove and return the oldest element
  - `peek(value)`: Look at oldest element without removing
  - `peekAt(index, value)`: Look at element at specified position
  - `clear()`: Remove all elements
  - `isEmpty()`: Check if buffer is empty
  - `isFull()`: Check if buffer is full
  - `available()`: Get number of elements stored
  - `availableForWrite()`: Get number of free slots
  - `capacity()`: Get total capacity

## Memory Performance Comparison

| Memory Type | Access Speed | Capacity | Use Case |
|-------------|--------------|----------|----------|
| RAM (DRAM)  | Very Fast    | ~200-300KB | Small, performance-critical data |
| PSRAM       | Fast         | Up to 4MB  | Medium-sized datasets, frequent access |
| HIMEM       | Medium       | Up to 8MB  | Large datasets, less frequent access |

## Hardware Compatibility

| ESP32 Board | PSRAM Size | HIMEM Support | Notes |
|-------------|------------|--------------|-------|
| ESP32       | Up to 4MB  | Yes          | Most common ESP32 modules |
| ESP32-S2    | Up to 2MB  | Limited      | Check board specifications |
| ESP32-S3    | Up to 8MB  | Yes          | Best memory capabilities |
| ESP32-C3    | None       | No           | Not compatible with this library |

## Troubleshooting

### PSRAM Not Detected

- Ensure PSRAM is enabled in your Arduino IDE board configuration:
  - Tools > PSRAM: "Enabled"

- Add this to your setup function to verify PSRAM:
  ```cpp
  if (psramFound()) {
    Serial.printf("PSRAM size: %d bytes\n", ESP.getPsramSize());
  } else {
    Serial.println("No PSRAM detected!");
  }
  ```

### Memory Allocation Failures

- When using large allocations, check available memory first:
  ```cpp
  Serial.printf("Free PSRAM: %d bytes\n", ESP.getFreePsram());
  ```

- Try reducing buffer sizes or using multiple smaller buffers

### Performance Issues

- HIMEM access is slower than PSRAM or RAM
- For frequently accessed data, prefer RAM > PSRAM > HIMEM
- Use the appropriate container for your use case

## Design Philosophy

The ESP32-PSRAM library is designed with these principles in mind:

1. **Familiar interfaces**: API design mimics standard C++ containers and Arduino libraries
2. **Memory efficiency**: Automatic use of appropriate memory regions
3. **Type safety**: Template-based to work with any data type
4. **Flexibility**: Configurable for different memory types and use cases
5. **Zero overhead**: Direct memory access with minimal abstraction layers


## License

MIT License

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgements

This library was inspired by the need for better memory management solutions on ESP32, especially for audio, image processing, and data logging applications.