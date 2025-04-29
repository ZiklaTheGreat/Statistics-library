#pragma once

#include "FileOut.h"
#include <cstdint>
#include <fstream>
#include <vector>
#include <format>

/**
 * @brief Class for writing binary data to a file.
 * 
 * This class inherits from `FileOut` and implements writing operations for binary files,
 * storing data as a vector of bytes.
 */
class BinFileOut : public FileOut<std::vector<uint8_t>>
{
public:
    /**
     * @brief Opens a binary file for writing.
     * 
     * @param file The path to the binary file to open.
     * @throws std::runtime_error If the file cannot be opened.
     */
    void open(const std::string_view file) override
    {
        close();
        outFile.open(file.data(), std::ios::binary | std::ios::in);
        if (!outFile)
        {
            throw std::runtime_error(std::format("Failed to open file: {}", file));
        }
    }

    /**
     * @brief Closes the currently open binary file.
     */
    void close() noexcept override
    {
        if (outFile.is_open())
        {
            outFile.close();
        }
    }

    /**
     * @brief Writes binary data to the file.
     * 
     * Writes the size of the data (4 bytes, little-endian) followed by the actual data content.
     * 
     * @param data The vector of bytes to write to the file.
     * @throws std::runtime_error If the file is not open for writing.
     */
    void write(const std::vector<uint8_t>& data) override {
        if (!outFile) {
            throw std::runtime_error("Failed to open file for writing");
        }
    
        // Write size in a portable way (little-endian, fixed 4 bytes)
        uint32_t dataSize = static_cast<uint32_t>(data.size()); // Použijeme 4-bajtový typ
        for (size_t i = 0; i < sizeof(dataSize); ++i) {
            uint8_t byte = static_cast<uint8_t>(dataSize >> (i * 8));
            outFile.write(reinterpret_cast<const char*>(&byte), 1);
        }
    
        // Write the actual data
        outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
};