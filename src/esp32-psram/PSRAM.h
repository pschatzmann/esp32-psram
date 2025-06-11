#pragma once

#include <Arduino.h>
#include <map>
#include "VectorFile.h"

// Define Arduino file mode constants if not already defined
#ifndef FILE_READ
#define FILE_READ 0
#endif
#ifndef FILE_WRITE
#define FILE_WRITE 1
#endif
#ifndef FILE_APPEND
#define FILE_APPEND 2
#endif

namespace esp32_psram {

/**
 * @class PSRAMClass
 * @brief Class for managing files stored in ESP32's PSRAM
 * 
 * This class provides an interface similar to SD.h for managing files
 * that are stored in PSRAM memory rather than on an SD card.
 */
class PSRAMClass {
private:
    bool initialized = false;
    std::map<std::string, VectorPSRAM<uint8_t>> fileData;
    
public:
    /**
     * @brief Initialize the PSRAM filesystem
     * @return true if initialization was successful, false otherwise
     */
    bool begin() {
        if (esp_himem_get_free_size() > 0) {
            initialized = true;
            return true;
        }
        initialized = false;
        return false;
    }
    
    /**
     * @brief Check if a file exists in PSRAM
     * @param filename Name of the file to check
     * @return true if the file exists, false otherwise
     */
    bool exists(const char* filename) {
        if (!initialized) return false;
        return fileData.find(std::string(filename)) != fileData.end();
    }
    
    /**
     * @brief Open a file in PSRAM
     * @param filename Name of the file to open
     * @param mode Mode to open the file in (FILE_READ, FILE_WRITE, etc.)
     * @return A FilePSRAM object for the opened file
     */
    FilePSRAM open(const char* filename, uint8_t mode) {
        if (!initialized) {
            FilePSRAM emptyFile;
            return emptyFile;
        }
        
        // Convert Arduino file modes to our enum
        FileMode fileMode;
        if (mode == FILE_READ) {
            fileMode = FileMode::READ;
        } else if (mode == FILE_WRITE) {
            fileMode = FileMode::WRITE;
        } else if (mode == FILE_APPEND) {
            fileMode = FileMode::APPEND;
        } else {
            fileMode = FileMode::READ_WRITE;
        }
        
        std::string filenameStr(filename);
        auto it = fileData.find(filenameStr);
        
        if (it != fileData.end()) {
            // File exists, create a new FilePSRAM pointing to existing data
            FilePSRAM file;
            file.setVector(&(it->second));
            file.setName(filename);
            file.open(fileMode);
            return file;
        } else if (mode != FILE_READ) {
            // File doesn't exist, create it for writing or appending
            fileData[filenameStr] = VectorPSRAM<uint8_t>(); // Create empty vector in the map
            FilePSRAM file;
            file.setVector(&(fileData[filenameStr]));
            file.setName(filename);
            file.open(fileMode);
            return file;
        } else {
            // File doesn't exist and mode is READ
            FilePSRAM emptyFile;
            return emptyFile;
        }
    }
    
    /**
     * @brief Remove a file from PSRAM
     * @param filename Name of the file to remove
     * @return true if the file was removed, false otherwise
     */
    bool remove(const char* filename) {
        if (!initialized) return false;
        
        std::string filenameStr(filename);
        auto it = fileData.find(filenameStr);
        
        if (it != fileData.end()) {
            fileData.erase(it);
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief Create a directory (no-op for compatibility)
     * @param dirname Name of the directory
     * @return Always returns true for compatibility
     */
    bool mkdir(const char* dirname) {
        // No directories in this implementation, just for compatibility
        return true;
    }
    
    /**
     * @brief Remove a directory (no-op for compatibility)
     * @param dirname Name of the directory
     * @return Always returns true for compatibility
     */
    bool rmdir(const char* dirname) {
        // No directories in this implementation, just for compatibility
        return true;
    }

    /**
     * @brief Get total space (returns available PSRAM)
     * @return Total PSRAM size in bytes
     */
    uint64_t totalBytes() {
        return esp_himem_get_phys_size();
    }

    /**
     * @brief Get free space (returns free PSRAM)
     * @return Free PSRAM size in bytes
     */
    uint64_t freeBytes() {
        return esp_himem_get_free_size();
    }
};

/**
 * @brief Global instance of PSRAMClass for easy access
 */
static PSRAMClass PSRAM;

} // namespace esp32_psram