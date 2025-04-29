#pragma once

#include "FileIn.h"
#include <fstream>
#include <vector>
#include <cstdint>
#include <format>

/**
 * @brief Class for reading binary data from a file.
 * 
 * This class inherits from `FileIn` and implements reading operations for binary files,
 * returning data as a vector of bytes.
 */
class BinFileIn : public FileIn<std::vector<uint8_t>>
{
public:
    /**
     * @brief Opens a binary file for reading.
     * 
     * @param file The path to the binary file to open.
     * @throws std::runtime_error If the file cannot be opened.
     */
    void open(std::string_view file) override
    {
        close();
        inFile.open(file.data(), std::ios::binary | std::ios::in);
        if (!inFile)
        {
            throw std::runtime_error(std::format("Failed to open file: {}", file));
        }
    }

    /**
     * @brief Closes the currently open binary file.
     */
    void close() noexcept override
    {
        if (inFile.is_open())
        {
            inFile.close();
        }
    }

    /**
     * @brief Reads binary data from the file.
     * 
     * Reads the size of the data (4 bytes, little-endian) followed by the actual data content.
     * 
     * @return A vector of bytes containing the read data.
     * @throws std::runtime_error If the file is not open or reading fails.
     */
    std::vector<uint8_t> read() override
    {
        if (!inFile)
        {
            throw std::runtime_error("No file opened for reading");
        }

        // Read size in a portable way (little-endian, fixed 4 bytes)
        uint32_t dataSize = 0;
        for (size_t i = 0; i < sizeof(dataSize); ++i)
        {
            uint8_t byte;
            inFile.read(reinterpret_cast<char *>(&byte), 1);
            if (inFile.eof())
            {
                return {};
            }
            if (inFile.fail())
            {
                throw std::runtime_error("Failed to read data size");
            }
            dataSize |= static_cast<uint32_t>(byte) << (i * 8);
        }

        // Create a buffer of the appropriate size
        std::vector<uint8_t> buffer;
        buffer.resize(dataSize); // Použijeme resize namiesto konštruktora

        // Read the data
        inFile.read(reinterpret_cast<char *>(buffer.data()), dataSize);
        if (inFile.fail() && !inFile.eof())
        {
            throw std::runtime_error("Failed to read data content");
        }
        return buffer;
    }
};