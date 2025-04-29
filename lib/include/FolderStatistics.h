#pragma once

#include "Statistics.h"
#include "StatisticsManager.h"
#include <memory>
#include <string>

/**
 * @brief Represents statistics for a specific folder.
 */
class FolderStatistics {
public:
    std::string name; ///< Name of the folder.
    std::string path; ///< Path to the folder.
    std::unique_ptr<StatisticsManager> statistics; ///< A unique pointer to the statistics manager.

    /**
     * @brief Constructor for FolderStatistics.
     * 
     * @param folderName The name of the folder.
     * @param folderPath The path to the folder.
     * @param stats A unique pointer to the StatisticsManager object.
     */
    FolderStatistics(std::string_view folderName, std::string_view folderPath, 
                   std::unique_ptr<StatisticsManager> stats)
        : name(std::move(folderName)), path(std::move(folderPath)), 
          statistics(std::move(stats)) {}
};
