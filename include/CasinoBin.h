#pragma once

#include "../lib/include/BinConverter.h"
#include "../lib/include/Writer.h"
#include "../lib/include/Reader.h"
#include "../lib/include/BinFileOut.h"
#include "../lib/include/BinFileIn.h"
#include <cstring> // For memcpy

/**
 * @brief Converts a double value to and from a binary format with a count prefix.
 * 
 * Format: [4 bytes count (uint32_t) | 8 bytes double]
 */
class CasinoBinConverter : public BinConverter<double>
{
public:
    /**
     * @brief Converts a double value to binary format.
     * @param data The double to convert.
     * @return A byte vector representing the binary data.
     */
    std::vector<uint8_t> convert(const double &data) override
    {
        // Create buffer: first 4 bytes for size (which is 1) followed by 8 bytes for the double
        std::vector<uint8_t> buffer(sizeof(uint32_t) + sizeof(double));
        
        // Write the count (always 1 for a single double)
        uint32_t count = 1;
        std::memcpy(buffer.data(), &count, sizeof(count));
        
        // Write the double value
        std::memcpy(buffer.data() + sizeof(uint32_t), &data, sizeof(data));
        
        return buffer;
    }

    /**
     * @brief Converts binary data back to a double.
     * @param data A byte vector representing the binary format.
     * @return The extracted double value.
     * @throws std::runtime_error if the format is invalid or count is not 1.
     */
    double convert(const std::vector<uint8_t> &data) override
    {
        // Check for minimum valid size (need at least 4 bytes for count and 8 bytes for double)
        if (data.size() < sizeof(uint32_t) + sizeof(double))
        {
            throw std::runtime_error("Invalid data size for double conversion");
        }
        
        // Read the count (should be 1)
        uint32_t count;
        std::memcpy(&count, data.data(), sizeof(count));
        
        if (count != 1)
        {
            throw std::runtime_error("Expected count of 1 for single double conversion");
        }
        
        // Read the double value
        double result;
        std::memcpy(&result, data.data() + sizeof(uint32_t), sizeof(result));
        
        return result;
    }
};

/**
 * @brief Writer class for serializing double values to a binary file using CasinoBinConverter.
 */
class CasinoBinWriter : public Writer<double, CasinoBinConverter, BinFileOut>
{
public:
    CasinoBinWriter() : Writer() {}
    CasinoBinWriter(const std::string &pFile) : Writer(pFile) {}
};

/**
 * @brief Reader class for deserializing double values from a binary file using CasinoBinConverter.
 */
class CasinoBinReader : public Reader<double, CasinoBinConverter, BinFileIn>
{
public:
    CasinoBinReader() : Reader() {}
    CasinoBinReader(const std::string &pFile) : Reader(pFile) {}
};
