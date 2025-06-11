#pragma once

#include <Arduino.h>
#include <map>
#include "InMemoryFile.h"
extern "C" {
#include "esp32/himem.h"
}

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
 * @class HIMEMClass
 * @brief Class for managing files stored in ESP32's High Memory (HIMEM)
 * 
 * This class provides an interface similar to SD.h for managing files
 * that are stored in HIMEM memory (beyond the 4MB boundary) rather than on an SD card.
 * HIMEM offers larger storage capacity but slightly slower access than regular PSRAM.
 */
class HIMEMClass {
private:
    bool initialized = false;
    std::map<std::string, VectorHIMEM<uint8_t>> fileData;
    
public:
    /**
     * @brief Initialize the HIMEM filesystem
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
     * @brief Check if a file exists in HIMEM
     * @param filename Name of the file to check
     * @return true if the file exists, false otherwise
     */
    bool exists(const char* filename) {
        if (!initialized) return false;
        return fileData.find(std::string(filename)) != fileData.end();
    }
    
    /**
     * @brief Open a file in HIMEM
     * @param filename Name of the file to open
     * @param mode Mode to open the file in (FILE_READ, FILE_WRITE, etc.)
     * @return A FileHIMEM object for the opened file
     */
    FileHIMEM open(const char* filename, uint8_t mode) {
        if (!initialized) {
            FileHIMEM emptyFile;
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
            // File exists, create a new FileHIMEM pointing to existing data
            FileHIMEM file;
            file.setVector(&(it->second));
            file.setName(filename);
            file.open(fileMode);
            return file;
        } else if (mode != FILE_READ) {
            // File doesn't exist, create it for writing or appending
            fileData[filenameStr] = VectorHIMEM<uint8_t>(); // Create empty vector in the map
            FileHIMEM file;
            file.setVector(&(fileData[filenameStr]));
            file.setName(filename);
            file.open(fileMode);
            return file;
        } else {
            // File doesn't exist and mode is READ
            FileHIMEM emptyFile;
            return emptyFile;
        }
    }
    
    /**
     * @brief Remove a file from HIMEM
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
     * @brief Get total space (returns available HIMEM)
     * @return Total HIMEM size in bytes
     */
    uint64_t totalBytes() {
        return esp_himem_get_phys_size();
    }

    /**
     * @brief Get free space (returns free HIMEM)
     * @return Free HIMEM size in bytes
     */
    uint64_t freeBytes() {
        return esp_himem_get_free_size();
    }
};

/**
 * @brief Global instance of HIMEMClass for easy access
 */
static HIMEMClass HIMEM;

} // namespace esp32_psram