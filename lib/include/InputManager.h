#pragma once

#include "Replication.h"
#include <vector>
#include <filesystem>
#include <memory>
#include <iostream>
#include <algorithm>
#include <format>

/**
 * @brief InputManager class for managing replications and their readers.
 * 
 * The `InputManager` class handles loading, sorting, and accessing replication data. It provides various 
 * methods to load replications from a specified base path or from a specific replication directory. 
 * It also supports batch loading and sorting of the replications based on their names.
 * 
 * @tparam R The replication type, which must inherit from Replication.
 */
template <ReplicationType R>
class InputManager
{
public:
    using ReplicationType = R; ///< Alias for the replication type.

private:
    std::vector<std::shared_ptr<R>> replications; ///< List of loaded replications.
    std::string basePath;                          ///< Base path where replication data is located.

public:
    /**
     * @brief Default constructor for InputManager.
     */
    InputManager() = default;

    /**
     * @brief Constructor that sets the base path for the input manager.
     * 
     * @param path The base path for loading replications.
     */
    InputManager(const std::string &path) { setBasePath(path); }

    /**
     * @brief Destructor for InputManager.
     */
    virtual ~InputManager() = default;

    /**
     * @brief Gets the list of loaded replications.
     * 
     * @return A constant reference to the vector of loaded replications.
     */
    const std::vector<std::shared_ptr<R>> &getReplications() const
    {
        return replications;
    }

    /**
     * @brief Gets the base path for loading replications.
     * 
     * @return The base path as a string.
     */
    std::string getBasePath() const { return basePath; }

    /**
     * @brief Sets the base path for loading replications.
     * 
     * Ensures that the base path ends with a forward slash ('/').
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
     * @brief Loads all replications from the base path.
     * 
     * Iterates over the directories in the base path and initializes the corresponding replications.
     */
    void loadReplications()
    {
        int c = 1;
        for (const auto &entry : std::filesystem::directory_iterator(basePath))
        {
            if (entry.is_directory())
            {
                std::string folderName = entry.path().filename().string();
                c++;
                auto replication = std::make_shared<R>(folderName);
                replication->setBasePath(basePath + folderName + "/");
                replication->setName(folderName);
                replication->init();
                replications.push_back(replication);
            }
        }
        sortReplications();
    }

    /**
     * @brief Loads a specific replication by name.
     * 
     * @param name The name of the replication to load.
     * @throws std::runtime_error If the directory does not exist.
     */
    void loadSpecificReplication(std::string_view name)
    {
        std::string fullPath = std::format("{}{}", basePath, name);
        if (!std::filesystem::exists(fullPath) || !std::filesystem::is_directory(fullPath))
        {
            throw std::runtime_error(std::format("Directory not found: {}", fullPath));
        }

        auto replication = std::make_shared<R>(name.data());
        replication->setBasePath(fullPath + "/");
        replication->setName(name);
        replication->init();
        replications.push_back(replication);
    }

    /**
     * @brief Loads a batch of replications based on a specified range.
     * 
     * @param start The starting index (inclusive).
     * @param end The ending index (inclusive).
     * @throws std::invalid_argument If the end value is less than the start value.
     */
    void loadBatch(int start, int end)
    {
        if (end < start)
        {
            throw std::invalid_argument("End value must be greater than or equal to start value");
        }

        int count = 0;
        for (const auto &entry : std::filesystem::directory_iterator(basePath))
        {
            if (entry.is_directory())
            {
                std::string folderName = entry.path().filename().string();

                // Extract number from folder name
                size_t numStart = folderName.find_last_not_of("0123456789") + 1;
                try
                {
                    int num = std::stoi(folderName.substr(numStart));
                    if (num >= start && num <= end)
                    {
                        auto replication = std::make_shared<R>(folderName);
                        replication->setBasePath(basePath + folderName + "/");
                        replication->setName(folderName);
                        replication->init();
                        replications.push_back(replication);
                        count++;
                    }
                }
                catch (...)
                {
                    // Skip folders that don’t have a number at the end
                    throw std::runtime_error("Failed processing file: " + folderName);
                }
            }
        }
    }

    /**
     * @brief Loads specific replications from a list of folder names.
     * 
     * @param folders A vector of folder names to load.
     */
    void loadReplications(const std::vector<std::string> folders)
    {
        for (const auto &folder : folders)
        {
            loadSpecificReplication(folder);
        }
    }

    /**
     * @brief Sorts the loaded replications based on their names.
     * 
     * The sorting is based on the numerical part of the folder name. If no number is found, 
     * it falls back to lexicographical sorting.
     */
    void sortReplications() {
        // Pomocná štruktúra na uloženie čísla a replikácie
        struct SortableReplication {
            std::shared_ptr<R> replication;
            int number = -1; // -1 pre replikácie bez čísla
            std::string name;
        };
    
        std::vector<SortableReplication> sortable;
        sortable.reserve(replications.size());
    
        for (const auto &rep : replications) {
            std::string name = rep->getName();
            int num = -1;
            size_t numStart = name.find_last_not_of("0123456789") + 1;
            try {
                num = std::stoi(name.substr(numStart));
            } catch (...) {
                // Bez čísla, použije sa name
            }
            sortable.push_back({rep, num, name});
        }
    
        std::sort(sortable.begin(), sortable.end(),
                  [](const SortableReplication &a, const SortableReplication &b) {
                      if (a.number != -1 && b.number != -1) {
                          return a.number < b.number;
                      }
                      return a.name < b.name;
                  });
    
        // Preusporiadať replikácie
        replications.clear();
        for (const auto &item : sortable) {
            replications.push_back(item.replication);
        }
    }

    /**
     * @brief Retrieves a replication by index.
     * 
     * @param index The index of the replication to retrieve.
     * @return A shared pointer to the requested replication.
     * @throws std::out_of_range If the index is out of bounds.
     */
    std::shared_ptr<R> getReplication(int index) const
    {
        if (index < 0 || index >= static_cast<int>(replications.size()))
        {
            throw std::out_of_range("Index out of range");
        }
        return replications[index];
    }

    /**
     * @brief Retrieves a replication by name.
     * 
     * @param name The name of the replication to retrieve.
     * @return A shared pointer to the requested replication.
     * @throws std::runtime_error If the replication is not found.
     */
    std::shared_ptr<R> getReplication(std::string_view name) const
    {
        for (const auto &replication : replications)
        {
            if (replication->getName() == name)
            {
                return replication;
            }
        }
        throw std::runtime_error("Replication with the given name not found");
    }

    /**
     * @brief Clears the list of loaded replications.
     */
    void clearReplications()
    {
        replications.clear();
    }
};

/**
 * @brief Concept to ensure a type is derived from InputManager.
 * 
 * This concept ensures that the type `T` is a subclass of `InputManager` and that it
 * follows the correct interface.
 * 
 * @tparam T The type to check.
 */
template <typename R>
concept InputManagerType = std::is_base_of_v<InputManager<typename R::ReplicationType>, R>;