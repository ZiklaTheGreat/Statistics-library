#pragma once

#include "FileOut.h"
#include <fstream>
#include <string>
#include <format>

/**
 * @brief A class for handling CSV file output operations.
 * 
 * This class extends `FileOut<std::string>` and provides methods for writing 
 * data to a CSV file. It ensures proper file management, including opening, 
 * closing, and writing operations.
 */
class CSVFileOut : public FileOut<std::string>
{
public:
    /**
     * @brief Opens a CSV file for writing.
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
        outFile.open(file.data(), std::ios::binary | std::ios::in);
        if (!outFile)
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
        if (outFile.is_open())
        {
            outFile.close();
        }
    }

    /**
     * @brief Writes a line of CSV data to the file.
     * 
     * The function appends a newline character after writing the data.
     * Throws an exception if no file is currently open.
     * 
     * @param data The CSV-formatted string to write.
     * @throws std::runtime_error If no file is open for writing.
     */
    void write(const std::string &data) override
    {
        if (!outFile) // Check if the file is open
        {
            throw std::runtime_error("No file opened for writing");
        }

        outFile.write(data.data(), data.size()); // Write data to file
        outFile << std::endl;                    // Append a newline character
    }
};