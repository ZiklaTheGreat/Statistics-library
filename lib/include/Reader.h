#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <format>

/**
 * @brief Abstract base class for all reader implementations.
 *
 * This class provides a common interface for different types of readers
 * that handle input operations.
 */
class IReader
{
public:
    virtual ~IReader() = default;
};

/**
 * @brief Concept defining a valid converter type.
 *
 * A valid converter must provide methods for converting input data to output format,
 * converting back from output format to input, and processing a vector of inputs.
 *
 * @tparam T The converter type.
 * @tparam O The output type.
 */
template <typename T, typename O>
concept ConverterType = requires(T converter, const O &output, const T &input, const std::vector<T> &vecInput) {
    { converter.convert(input) } -> std::same_as<O>;
    { converter.convert(output) } -> std::same_as<T>;
    { converter.convertVector(vecInput) } -> std::same_as<O>;
};

/**
 * @brief Generic reader class for handling input operations.
 *
 * This template class provides a mechanism for reading data of type `T`
 * from a file using a converter `C` and a file handler `F`.
 *
 * @tparam T The type of data being read.
 * @tparam C The converter class responsible for transforming input data.
 * @tparam F The file handling class that manages file operations.
 */
template <typename T, typename C, typename F>
class Reader : public IReader
{
private:
    C converter;                            ///< Converter instance for transforming data.
    F file;                                 ///< File handler instance for managing file operations.
    std::vector<std::unique_ptr<T>> data{}; ///< Container for storing read data.
    std::string path;                       ///< File path.
    bool isOpen = false;                    ///< Flag indicating whether the file is currently open.

public:
    Reader() = default;

    /**
     * @brief Constructs a Reader with a specified file path.
     *
     * @param pFile The file path to be used for reading.
     */
    Reader(const std::string &pFile) : path(pFile) {};

    /**
     * @brief Destructor that ensures the file is closed if open.
     */
    virtual ~Reader() override
    {
        if (isOpen)
        {
            close();
        }
    }

    /**
     * @brief Opens the specified file for reading.
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
    virtual void close()
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
     * @brief Reads a single data entry from the file.
     *
     * The data is read from the file and converted into the appropriate type.
     * If the file is not already open, it will be opened.
     *
     * @return A unique pointer to the read data.
     * @throws std::runtime_error If reading fails or the end of the file is reached.
     */
    virtual std::unique_ptr<T> read()
    {
        if (!isOpen)
        {
            open(path);
        }

        try
        {
            // Read data from the file - handle both string and binary data
            auto fileData = file.read();

            // Check if we reached end of file
            if constexpr (std::is_same_v<decltype(fileData), std::string>)
            {
                if (fileData.empty())
                {
                    throw std::runtime_error("End of file reached");
                }
            }
            else if constexpr (std::is_same_v<decltype(fileData), std::vector<uint8_t>>)
            {
                if (fileData.empty())
                {
                    throw std::runtime_error("End of file reached");
                }
            }

            // Convert and return the data
            return std::make_unique<T>(converter.convert(fileData));
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Failed to read/convert data: " + std::string(e.what()));
        }
    }

    /**
     * @brief Loads all data from the file into memory.
     *
     * Reads the entire file and stores the data in the internal container.
     * If the file is not already open, it will be opened before reading.
     *
     * @throws std::runtime_error If the file path is not set or an error occurs during reading.
     */
    void load()
    {
        if (!isOpen)
        {
            if (path.empty())
            {
                throw std::runtime_error("File path is not set");
            }
            open(path);
        }

        flush();

        try
        {
            while (true)
            {
                try
                {
                    auto item = read();
                    if (item)
                    {
                        data.push_back(std::move(item));
                    }
                }
                catch (const std::runtime_error &e)
                {
                    if (std::string(e.what()).find("End of file") != std::string::npos)
                    {
                        break;
                    }
                    std::cerr << "Error reading file: " << e.what() << std::endl;
                    break; // Prevent infinite loop
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error during load: " << e.what() << std::endl;
            throw;
        }

        close();
    }

    /**
     * @brief Retrieves the stored data.
     *
     * @return A constant reference to the vector of unique pointers to data objects.
     */
    const std::vector<std::unique_ptr<T>> &getData() const { return data; }

    /**
     * @brief Clears all stored data.
     *
     * Empties the internal storage and shrinks the container to optimize memory usage.
     */
    void flush()
    {
        data.clear();
        data.shrink_to_fit();
    }
};