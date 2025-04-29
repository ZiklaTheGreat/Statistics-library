#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <format>
#include "Writer.h"

/**
 * @brief Concept to define the required interface for writer classes.
 * 
 * This concept ensures that the class has methods for writing data,
 * opening and closing files, as expected by the OutputManager class.
 * 
 * @tparam T The writer class that must meet the specified requirements.
 */
template <typename T>
concept WriterConcept = requires(T t, const typename T::DataType &data, const std::string &filename) {
    { t.write(data) };  ///< Method to write data.
    { t.open(filename) }; ///< Method to open a file.
    { t.close() };  ///< Method to close a file.
};

/**
 * @brief Class responsible for managing multiple writers and organizing output file handling.
 * 
 * The OutputManager is used to manage a collection of writer objects and perform operations such as
 * creating new replication directories, registering writers, and closing all writers.
 */
class OutputManager
{
private:
    std::vector<std::shared_ptr<IWriter>> writers; ///< A collection of registered writer objects.
    std::string basePath; ///< Base path where output files are stored.
    std::string replicationName; ///< Name of the current replication.
    std::string currentReplicationName; ///< Name of the current replication (if different).
    std::string currentReplicationPath; ///< Path for the current replication.
    int counter{1}; ///< Counter for generating unique replication names.

public:
    /**
     * @brief Default constructor for OutputManager.
     */
    OutputManager() {}

    /**
     * @brief Constructor that initializes the OutputManager with a base path.
     * 
     * @param path The base path for storing output files.
     */
    OutputManager(const std::string &path) { setBasePath(path); }

    /**
     * @brief Destructor for OutputManager.
     */
    virtual ~OutputManager() = default;

    /**
     * @brief Gets the base path for output files.
     * 
     * @return The base path as a string.
     */
    std::string getBasePath() const { return basePath; }

    /**
     * @brief Sets the base path for output files.
     * 
     * This method ensures that the base path ends with a forward slash ('/').
     * 
     * @param path The base path to set.
     */
    void setBasePath(std::string_view path) {
        basePath = path;
        if (!basePath.empty() && basePath.back() != '/') {
            basePath = std::format("{}/", basePath);
        }
    }

    /**
     * @brief Gets the current replication name.
     * 
     * @return The current replication name as a string.
     */
    std::string getName() const { return replicationName; }

    /**
     * @brief Sets the current replication name.
     * 
     * @param name The name for the current replication.
     */
    void setName(const std::string &name)
    {
        replicationName = name;
    }

    /**
     * @brief Gets the current replication name (with suffix).
     * 
     * @return The current replication name (with suffix).
     */
    std::string getCurrentReplicationName() const { return currentReplicationName; }

    /**
     * @brief Sets the current replication name (with suffix).
     * 
     * @param name The name for the current replication with suffix.
     */
    void setCurrentReplicationName(std::string_view name)
    {
        currentReplicationName = name;
    }

    /**
     * @brief Gets the current replication path.
     * 
     * @return The path for the current replication as a string.
     */
    std::string getCurrentReplicationPath() const { return currentReplicationPath; }

    /**
     * @brief Sets the current replication path.
     * 
     * @param path The path for the current replication.
     */
    void setCurrentReplicationPath(std::string_view path)
    {
        currentReplicationPath = path;
    }

    /**
     * @brief Registers a writer to be used in this output manager.
     * 
     * This method adds a writer object to the list of writers managed by the OutputManager.
     * 
     * @tparam W The writer type that must meet the `WriterConcept`.
     * @param writer The writer object to register.
     */
    template <WriterConcept W>
    void registerWriter(std::shared_ptr<W> &writer)
    {
        writers.push_back(std::move(writer));
    }

    /**
     * @brief Retrieves a registered writer by its index.
     * 
     * This method returns a shared pointer to the writer of the specified type.
     * 
     * @tparam W The writer type to retrieve.
     * @param index The index of the writer to retrieve.
     * @return A shared pointer to the writer.
     * @throws std::out_of_range If the index is out of bounds.
     */
    template <WriterConcept W>
    std::shared_ptr<W> getWriter(int index)
    {
        if (index < 0 || index >= static_cast<int>(writers.size()))
        {
            throw std::out_of_range("Index out of range");
        }
        return std::static_pointer_cast<W>(writers[index]);
    }

    /**
     * @brief Closes all registered writers.
     * 
     * This method ensures that all writers are closed, if they are open.
     */
    void closeAllWriters() noexcept {
        for (auto &writer : writers) {
            writer->close();
        }
    }

    /**
     * @brief Starts a new replication process with a generated name.
     * 
     * This method creates a new directory for the replication, incrementing a counter
     * for the replication name. It then calls the `init` method to initialize the writers.
     */
    void newReplication()
    {
        closeAllWriters();
        writers.clear();

        setCurrentReplicationName(getName() + std::to_string(counter));
        setCurrentReplicationPath(getBasePath() + currentReplicationName + "/");
        counter++;

        if (!std::filesystem::exists(currentReplicationPath))
        {
            if (!std::filesystem::create_directory(currentReplicationPath))
            {
                throw std::runtime_error("Failed to create directory: " + currentReplicationPath);
            }
        }

        init();
    }

    /**
     * @brief Abstract method for initializing the writers for the new replication.
     * 
     * This method must be implemented by derived classes to initialize the writers
     * according to the specific needs of the replication.
     */
    virtual void init() = 0;
};
