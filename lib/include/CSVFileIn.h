#pragma once

#include "FileIn.h"
#include <fstream>
#include <string>
#include <format>

/**
 * @brief A class for reading data from a CSV file.
 * 
 * This class extends `FileIn<std::string>` and provides methods to open, close,
 * and read lines from a CSV file.
 */
class CSVFileIn : public FileIn<std::string>
{
public:
    /**
     * @brief Destructor that ensures the file is closed upon object destruction.
     */
    ~CSVFileIn() { close(); }

    /**
     * @brief Opens a CSV file for reading.
     * 
     * If a file is already open, it will be closed before opening a new one.
     * Throws an exception if the file cannot be opened.
     * 
     * @param file The path to the CSV file.
     * @throws std::runtime_error If the file cannot be opened.
     */
    void open(const std::string_view file) override
    {
        close();
        inFile.open(file.data(), std::ios::binary | std::ios::in);
        if (!inFile)
        {
            throw std::runtime_error(std::format("Failed to open file: {}", file));
        }
    }

    /**
     * @brief Closes the currently opened CSV file.
     * 
     * If no file is open, this function does nothing.
     */
    void close() noexcept override
    {
        if (inFile.is_open())
        {
            inFile.close();
        }
    }

    /**
     * @brief Reads a single line from the CSV file.
     * 
     * Reads the next line from the file and returns it as a string.
     * Throws an exception if the end of the file is reached or if an error occurs.
     * 
     * @return A string containing the next line from the file.
     * @throws std::runtime_error If the end of the file is reached or if reading fails.
     */
    std::string read() override
    {
        std::string line;
        if (!std::getline(inFile, line))
        {
            if (inFile.eof())
            {
                throw std::runtime_error("End of file reached");
            }
            throw std::runtime_error("Error reading file");
        }
        return line;
    }
};