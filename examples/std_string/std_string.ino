#include <string>
#include "esp32-psram.h"

// Alias for a std::string that stores its dynamic buffer in PSRAM
using StringPSRAM = std::basic_string<char, std::char_traits<char>, AllocatorPSRAM<char>>;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  StringPSRAM s;  // construct with PSRAM allocator
  s.reserve(1024);  // force an allocation
  s = "Hello from PSRAM string. ";
  s += "Appending more data...";

  Serial.println(s.c_str());
  Serial.printf("Size: %u, Capacity: %u\n", (unsigned)s.size(), (unsigned)s.capacity());

  // Verify location of underlying buffer
  const char* buf = s.data();
  if (esp_ptr_external_ram(buf)) {
    Serial.println("String storage is in PSRAM.");
  } else {
    Serial.println("String storage is NOT in PSRAM.");
  }
}

void loop() {
  // Nothing here
}