#pragma once

#include <vector>

/**
 * @brief An abstract class for data conversion between two types.
 * 
 * This template class provides an interface for converting data between
 * two different types, `T` and `O`. Implementing classes must define
 * the conversion logic.
 * 
 * @tparam T The input data type.
 * @tparam O The output data type.
 */
template <typename T, typename O>
class Converter
{
public:
    /**
     * @brief Default constructor.
     */
    Converter() = default;

    /**
     * @brief Virtual destructor to ensure proper cleanup in derived classes.
     */
    virtual ~Converter() = default;

    /**
     * @brief Converts data of type `T` to type `O`.
     * 
     * This function must be implemented by derived classes to define
     * the specific conversion logic.
     * 
     * @param data The input data of type `T`.
     * @return The converted data of type `O`.
     */
    virtual O convert(const T &data) = 0;

    /**
     * @brief Converts data of type `O` back to type `T`.
     * 
     * This function must be implemented by derived classes to define
     * the specific conversion logic.
     * 
     * @param data The input data of type `O`.
     * @return The converted data of type `T`.
     */
    virtual T convert(const O &data) = 0;
};