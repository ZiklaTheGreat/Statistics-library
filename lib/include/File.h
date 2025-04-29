#pragma once

#include <string>

/**
 * @brief Abstract base class representing a file interface.
 * 
 * This class provides a generic interface for handling files, allowing derived classes
 * to implement specific file operations.
 * 
 */
class File
{
public:
    /**
     * @brief Default constructor.
     */
    File() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~File() noexcept = default;

    /**
     * @brief Opens a file with the specified filename.
     * 
     * @param file The path to the file that should be opened.
     */
    virtual void open(const std::string_view file) = 0;

    /**
     * @brief Closes the currently open file.
     */
    virtual void close() noexcept = 0;
};