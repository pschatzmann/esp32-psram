#include <vector>
#include "esp32-psram.h"

void setup() {
  Serial.begin(115200);
  while (!Serial);

  size_t freeBefore = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);

  std::vector<int, AllocatorPSRAM<int>> v; // uses PSRAM allocator
  v.reserve(5000);                  // force allocation in PSRAM

  for (int i = 0; i < 5000; ++i) {
    v.push_back(i);
  }

  Serial.printf("Vector size=%u capacity=%u\n", (unsigned)v.size(), (unsigned)v.capacity());

  if (v.data() && esp_ptr_external_ram(v.data())) {
    Serial.println("Vector storage is in PSRAM.");
  } else {
    Serial.println("Vector storage NOT in PSRAM.");
  }

}

void loop() {
}