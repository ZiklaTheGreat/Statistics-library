#pragma once

#include <string>
#include <ostream>
#include <iostream>
#include <vector>
#include <format>

/**
 * @brief Abstract base class for all writer implementations.
 * 
 * This class provides a common interface for different types of writers
 * that handle output operations. It enforces the implementation of a
 * `close()` method in derived classes.
 */
class IWriter
{
public:
    virtual ~IWriter() = default;

    /**
     * @brief Closes the writer, ensuring all resources are released.
     */
    virtual void close() = 0;
};

/**
 * @brief Generic writer class that handles data conversion and file writing.
 * 
 * This template class provides a flexible mechanism for writing data of type `T`
 * to a file using a converter `C` and a file handler `F`.
 * 
 * @tparam T The type of data to be written.
 * @tparam C The converter class responsible for transforming `T` into a writable format.
 * @tparam F The file handling class that manages the actual file operations.
 */
template <typename T, typename C, typename F>
class Writer : public IWriter
{
private:
    C converter;         ///< Converter instance for transforming data before writing.
    F file;              ///< File handler instance for managing file operations.
    std::string path;    ///< File path.
    bool isOpen = false; ///< Flag indicating whether the file is currently open.

public:
    using DataType = T; ///< Defines the data type handled by the writer.

    Writer() = default;

    /**
     * @brief Constructs a Writer with a specified file path.
     * 
     * @param pFile The file path to be used for writing.
     */
    Writer(const std::string &pFile) : path(pFile) {};

    /**
     * @brief Destructor that ensures the file is closed if open.
     */
    virtual ~Writer() override
    {
        if (isOpen)
        {
            close();
        }
    }

    /**
     * @brief Opens the specified file for writing.
     * 
     * If the file is already open, it remains open. Otherwise, an attempt
     * is made to open the file, and an exception is thrown if it fails.
     * 
     * @param pFile The file path to open.
     * @throws std::runtime_error If the file cannot be opened.
     */
    void open(std::string_view pFile)
    {
        try
        {
            file.open(pFile);
            isOpen = true;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::format("Failed to open file: {} ({})", pFile, e.what()));
        }
    }

    /**
     * @brief Closes the file.
     * 
     * Ensures that the file is properly closed, logging a warning
     * if an error occurs during the process.
     */
    void close() override
    {
        try
        {
            file.close();
            isOpen = false;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Warning: Failed to close file: " << e.what() << std::endl;
        }
    }

    /**
     * @brief Writes a single data entry to the file.
     * 
     * The data is first converted using the converter and then written
     * to the file. If the file is not already open, it will be opened.
     * 
     * @param data The data entry to write.
     * @throws std::runtime_error If the file path is not set.
     */
    void write(const T &data)
    {
        if (!isOpen)
        {
            if (path.empty())
            {
                throw std::runtime_error("File path is not set");
            }
            open(path);
        }

        file.write(converter.convert(data));
    }

    /**
     * @brief Writes a range of data entries to the file.
     * 
     * Each entry in the range is converted and written to the file.
     * If the file is not already open, it will be opened.
     * 
     * @tparam Range The type of the range containing data entries.
     * @param dataRange The range of data entries to write.
     * @throws std::runtime_error If the file path is not set.
     */
    template <std::ranges::range Range>
    void write(const Range &dataRange)
    {
        if (!isOpen)
        {
            if (path.empty())
            {
                throw std::runtime_error("File path is not set");
            }
            open(path);
        }

        std::ranges::for_each(dataRange, [this](const T &item) {
            file.write(converter.convert(item));
        });
    }
};