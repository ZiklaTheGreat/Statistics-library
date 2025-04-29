#pragma once

#include "InputManager.h"
#include <armadillo>
#include <string>
#include <vector>

// Forward declaration
class PresenterManager;

/**
 * @brief Interface for statistics processing.
 * 
 * Defines methods for processing replication data, managing file paths, and setting up presenters.
 */
class IStatistics {
public:
    virtual ~IStatistics() = default;

    /**
     * @brief Processes all replications in the dataset.
     */
    virtual void processAllReplications() = 0;

    /**
     * @brief Processes a single replication at the specified index.
     * 
     * @param index The index of the replication to process.
     */
    virtual void processReplication(size_t index) = 0;

    /**
     * @brief Clears all processed data.
     */
    virtual void clearData() = 0;

    /**
     * @brief Sets the base path for file operations.
     * 
     * @param basePath The base directory path for file access.
     */
    virtual void setBasePath(std::string_view basePath) = 0;

    /**
     * @brief Loads data from the specified folders.
     * 
     * @param folderNames List of folder names to load data from.
     */
    virtual void loadFolders(const std::vector<std::string>& folderNames) = 0;
    
    /**
     * @brief Sets up presenters for visualizing the statistics.
     * 
     * @param manager The PresenterManager to configure presenters with.
     */
    virtual void setupPresenters(PresenterManager* manager) = 0;
};

/**
 * @brief Base class for statistics that use a specific input manager type.
 * 
 * This class provides common functionality for statistics processing using an input manager.
 * 
 * @tparam IM The type of input manager, constrained to support replication data.
 */
template <typename IM>
requires ReplicationType<typename IM::ReplicationType>
class Statistics : public IStatistics {
private:
    IM inputManager;      ///< Input manager for handling replication data.
    std::string basePath; ///< Base directory path for file operations.

public:
    Statistics() = default;
    virtual ~Statistics() = default;

    /**
     * @brief Sets the base path for file operations.
     * 
     * Updates the base path in both the statistics object and the input manager.
     * 
     * @param path The base directory path.
     */
    void setBasePath(std::string_view path) override {
        basePath = path;
        inputManager.setBasePath(path);
    }

    /**
     * @brief Loads data from the specified folders using the input manager.
     * 
     * @param folderNames List of folder names to load data from.
     */
    void loadFolders(const std::vector<std::string>& folderNames) override {
        inputManager.loadReplications(folderNames);
    }

    /**
     * @brief Processes all replications by iterating over them.
     */
    void processAllReplications() override {
        // Fix: use getReplications().size() instead of getReplicationCount()
        for (size_t i = 0; i < inputManager.getReplications().size(); i++) {
            processReplication(i);
        }
    }

    /**
     * @brief Processes a single replication at the specified index.
     * 
     * Must be implemented by derived classes.
     * 
     * @param index The index of the replication to process.
     */
    virtual void processReplication(size_t index) override = 0;

    /**
     * @brief Clears all processed data.
     * 
     * Must be implemented by derived classes.
     */
    virtual void clearData() override = 0;
    
    /**
     * @brief Sets up presenters for visualizing the statistics.
     * 
     * Provides a default empty implementation that can be overridden.
     * 
     * @param manager The PresenterManager to configure presenters with.
     * @throws std::invalid_argument If the manager is nullptr.
     */
    virtual void setupPresenters(PresenterManager* manager) override {
        if (!manager) {
            throw std::invalid_argument("PresenterManager cannot be nullptr");
        }
        // Default empty implementation
    }

    /**
     * @brief Returns a reference to the input manager.
     * 
     * @return The input manager used by this statistics object.
     */
    IM& getInputManager() {
        return inputManager;
    }
};