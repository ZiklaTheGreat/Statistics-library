#pragma once

#include "File.h"
#include <fstream>

/**
 * @brief Abstract class for handling input file operations.
 * 
 * This class extends the `File` interface and provides a base for reading data from files.
 * Derived classes must implement the `read` function to define specific reading behavior.
 * 
 * @tparam O The type of data that will be read from the file.
 */
template <typename O>
class FileIn : public File
{
protected:
    std::ifstream inFile; ///< Input file stream for reading data.

public:
    /**
     * @brief Default constructor.
     */
    FileIn() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~FileIn() = default;

    FileIn(const FileIn&) = delete;
    FileIn& operator=(const FileIn&) = delete;
    FileIn(FileIn&&) = default;
    FileIn& operator=(FileIn&&) = default;

    /**
     * @brief Reads data from the input file.
     * 
     * This function must be implemented by derived classes to specify how data is read.
     * 
     * @return The data read from the file.
     */
    virtual O read() = 0;
};