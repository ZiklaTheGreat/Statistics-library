#pragma once

#include "Converter.h"
#include <vector>
#include <cstdint>

/**
 * @brief Abstract class for binary data conversion.
 * 
 * This class inherits from `Converter` and specializes in converting between a type `T`
 * and a binary representation as a vector of bytes (`std::vector<uint8_t>`).
 * 
 * @tparam T The type to convert to/from binary data.
 */
template <typename T>
class BinConverter : public Converter<T, std::vector<uint8_t>> // Použi std::vector<uint8_t> ako OutputType
{
public:
    using OutputType = std::vector<uint8_t>;

    /**
     * @brief Converts data of type `T` to a binary representation.
     * 
     * Must be implemented by derived classes to define the conversion logic.
     * 
     * @param data The input data of type `T`.
     * @return A vector of bytes representing the binary data.
     */
    OutputType convert(const T &data) override = 0;

    /**
     * @brief Converts binary data back to type `T`.
     * 
     * Must be implemented by derived classes to define the conversion logic.
     * 
     * @param data The binary data as a vector of bytes.
     * @return The converted data of type `T`.
     */
    T convert(const OutputType &data) override = 0;
};