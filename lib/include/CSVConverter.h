#pragma once

#include "Converter.h"
#include <string>
#include <vector>

/**
 * @brief Abstract class for converting data to and from CSV format.
 * 
 * This class extends the `Converter` interface to provide a specialized
 * conversion mechanism for transforming objects of type `T` to CSV-formatted
 * strings and vice versa.
 * 
 * @tparam T The type of the data to be converted.
 */
template <typename T>
class CSVConverter : public Converter<T, std::string> // O is std::string
{
public:
    /**
     * @brief Defines the output type as a CSV-formatted string.
     */
    using OutputType = std::string;

    /**
     * @brief Converts an object of type `T` into a CSV-formatted string.
     * 
     * This function must be implemented by derived classes to specify
     * the exact format of conversion.
     * 
     * @param data The object to convert.
     * @return A CSV-formatted string representation of `data`.
     */
    virtual OutputType convert(const T &data) override = 0;

    /**
     * @brief Converts a CSV-formatted string back into an object of type `T`.
     * 
     * This function must be implemented by derived classes to specify
     * the exact parsing logic.
     * 
     * @param data The CSV-formatted string.
     * @return An object of type `T` parsed from the string.
     */
    virtual T convert(const std::string &data) override = 0; // Fixed type
};