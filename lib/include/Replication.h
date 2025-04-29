#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include "Reader.h"

/**
 * @brief Concept to define the required interface for reader classes.
 * 
 * This concept ensures that the class has methods for opening files,
 * closing files, reading data, loading data, and flushing data.
 * 
 * @tparam T The reader class that must meet the specified requirements.
 */
template <typename T>
concept ReaderConcept = requires(T t, const std::string &filename) {
    { t.open(filename) };  ///< Method to open a file.
    { t.close() };  ///< Method to close a file.
    { t.read() };  ///< Method to read data.
    { t.load() };  ///< Method to load data.
    { t.flush() };  ///< Method to flush the data.
};

/**
 * @brief Class representing a replication.
 * 
 * The `Replication` class manages a collection of readers and provides
 * the ability to initialize and interact with multiple data readers for the replication.
 */
class Replication
{
private:
    std::string name;                             ///< The name of the replication.
    std::string basePath;                         ///< The base path where replication data is stored.
    std::vector<std::shared_ptr<IReader>> readers; ///< A collection of reader objects.

public:
    /**
     * @brief Default constructor for Replication.
     */
    Replication() {}

    /**
     * @brief Constructor for Replication with a specific name.
     * 
     * @param name The name of the replication.
     */
    Replication(const std::string_view name) : name(name) {}

    /**
     * @brief Destructor for Replication.
     */
    virtual ~Replication() = default;

    /**
     * @brief Gets the base path for the replication data.
     * 
     * @return The base path as a string.
     */
    std::string getBasePath() const { return basePath; }

    /**
     * @brief Sets the base path for the replication data.
     * 
     * This method ensures that the base path ends with a forward slash ('/').
     * 
     * @param path The base path to set.
     */
    void setBasePath(const std::string &path)
    {
        basePath = path;
        if (!basePath.empty() && basePath.back() != '/')
        {
            basePath += '/';
        }
    }

    /**
     * @brief Registers a reader to be used in this replication.
     * 
     * This method adds a reader object to the list of readers managed by the Replication.
     * 
     * @tparam R The reader type that must meet the `ReaderConcept`.
     * @param reader The reader object to register.
     */
    template <ReaderConcept R>
    void registerReader(const std::shared_ptr<R> &reader)
    {
        readers.push_back(reader);
    }

    /**
     * @brief Retrieves a registered reader by its index.
     * 
     * This method returns a shared pointer to the reader of the specified type.
     * 
     * @tparam R The reader type to retrieve.
     * @param index The index of the reader to retrieve.
     * @return A shared pointer to the reader.
     * @throws std::out_of_range If the index is out of bounds.
     */
    template <ReaderConcept R>
    std::shared_ptr<R> getReader(int index) {
        if (index < 0 || index >= static_cast<int>(readers.size())) {
            throw std::out_of_range("Index out of range");
        }
        return std::static_pointer_cast<R>(readers[index]);
    }

    /**
     * @brief Gets the number of registered readers.
     * 
     * @return The number of readers in the collection.
     */
    size_t getReaderCount() const
    {
        return readers.size();
    }

    /**
     * @brief Gets the name of the replication.
     * 
     * @return The name of the replication as a string.
     */
    std::string getName() const
    {
        return name;
    }

    /**
     * @brief Sets the name of the replication.
     * 
     * @param newName The new name for the replication.
     */
    void setName(const std::string_view newName)
    {
        name = newName;
    }

    /**
     * @brief Abstract method to initialize the readers for this replication.
     * 
     * This method must be implemented by derived classes to initialize
     * the readers according to the specific needs of the replication.
     */
    virtual void init() = 0;
};

/**
 * @brief Concept to ensure a type is a derived class of `Replication`.
 * 
 * This concept ensures that a given type `T` is a subclass of the `Replication` class.
 * 
 * @tparam T The type to check.
 */
template <typename T>
concept ReplicationType = std::is_base_of_v<Replication, T>;