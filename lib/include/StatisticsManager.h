#pragma once

#include "Statistics.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <vector>

/**
 * @brief Manages a collection of statistics objects.
 * 
 * This class provides functionality to add, retrieve, remove, and process statistics,
 * storing them in a map with their names as keys.
 */
class StatisticsManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<IStatistics>> statisticsMap; ///< Map of statistics objects, keyed by their names.

public:
    StatisticsManager() = default;
    virtual ~StatisticsManager() = default;

    /**
     * @brief Adds a new statistics object to the manager.
     * 
     * Creates a new statistics object of type `S` with the given arguments and stores it under the specified name.
     * 
     * @tparam S The type of the statistics object, must derive from `IStatistics`.
     * @tparam Args Variadic template for constructor arguments of `S`.
     * @param name The name under which the statistics object will be stored.
     * @param args Arguments to forward to the constructor of `S`.
     * @return A shared pointer to the created statistics object.
     * @throws std::runtime_error If a statistics object with the given name already exists.
     */
    template <typename S, typename... Args>
        requires std::is_base_of_v<IStatistics, S>
    std::shared_ptr<S> addStatistics(const std::string &name, Args &&...args)
    {
        if (statisticsMap.contains(name))
        {
            throw std::runtime_error("Statistics with name '" + name + "' already exists.");
        }
        auto stats = std::make_shared<S>(std::forward<Args>(args)...);
        statisticsMap[name] = stats;
        return stats;
    }

    /**
     * @brief Retrieves a statistics object by its name.
     * 
     * @param name The name of the statistics object to retrieve.
     * @return A shared pointer to the statistics object.
     * @throws std::runtime_error If no statistics object with the given name exists.
     */
    std::shared_ptr<IStatistics> getStatistics(const std::string &name) const
    {
        auto it = statisticsMap.find(name);
        if (it == statisticsMap.end())
        {
            throw std::runtime_error("Statistics with name '" + name + "' not found.");
        }
        return it->second;
    }

    /**
     * @brief Removes a statistics object by its name.
     * 
     * @param name The name of the statistics object to remove.
     * @throws std::runtime_error If no statistics object with the given name exists.
     */
    void removeStatistics(const std::string &name)
    {
        if (statisticsMap.erase(name) == 0)
        {
            throw std::runtime_error("Statistics with name '" + name + "' not found.");
        }
    }

    /**
     * @brief Lists all stored statistics to an output stream.
     * 
     * @param out The output stream to write the list of statistics to (defaults to std::cout).
     */
    void listStatistics(std::ostream& out = std::cout) const {
        out << "Stored Statistics:\n";
        for (const auto& [name, _] : statisticsMap) {
            out << " - " << name << "\n";
        }
    }

    /**
     * @brief Removes all statistics from the manager.
     */
    void clearStatistics() { statisticsMap.clear(); }

    /**
     * @brief Returns the number of stored statistics.
     * 
     * @return The number of statistics currently stored.
     */
    size_t count() const { return statisticsMap.size(); }

    /**
     * @brief Get names of all available statistics
     * @return Vector of statistic names
     */
    std::vector<std::string> getStatisticsNames() const {
        std::vector<std::string> names;
        for (const auto& pair : statisticsMap) {
            names.push_back(pair.first);
        }
        return names;
    }

    /**
     * @brief Process a specific statistics by name
     * @param name The name of the statistics to process
     * @return true if successful, false otherwise
     */
    bool processStatistics(const std::string& name) {
        auto it = statisticsMap.find(name);
        if (it != statisticsMap.end()) {
            it->second->processAllReplications();
            return true;
        }
        return false;
    }
};