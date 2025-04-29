#pragma once

#include "File.h"
#include <fstream>

/**
 * @brief Abstract class for handling output file operations.
 * 
 * This class extends the `File` interface and provides a base for writing data to files.
 * Derived classes must implement the `write` function to define specific writing behavior.
 * 
 * @tparam O The type of data that will be written to the file.
 */
template <typename O>
class FileOut : public File
{
protected:
    std::ofstream outFile; ///< Output file stream for writing data.

public:
    /**
     * @brief Default constructor.
     */
    FileOut() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~FileOut() = default;

    FileOut(const FileOut&) = delete;
    FileOut& operator=(const FileOut&) = delete;
    FileOut(FileOut&&) = default;
    FileOut& operator=(FileOut&&) = default;

    /**
     * @brief Writes data to the output file.
     * 
     * This function must be implemented by derived classes to specify how data is written.
     * 
     * @param data The data to be written to the file.
     */
    virtual void write(const O &data) = 0;
};