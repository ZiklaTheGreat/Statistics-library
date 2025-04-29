#include "../include/Casino.h"

#include "../include/CasinoBinManagers.h"
#include "../include/CasinoBinStatistics.h"
#include "../include/CasinoBinStatisticsGraph.h"

#include "../lib/include/FolderStatistics.h"
#include "../lib/include/StatisticsManager.h"

#include "../lib/include/BinFileOut.h"
#include "../lib/include/BinFileIn.h"

/**
 * @brief Main entry point for the Casino simulation program.
 * 
 * This program performs the following steps:
 * 1. Runs one replication of a casino simulation.
 * 2. Stores the simulation results in binary files using `CasinoBinOutputManager`.
 * 3. Sets up a statistics manager (`StatisticsManager`) to analyze the stored results.
 * 4. Registers the output folder for analysis via `FolderStatistics`.
 * 5. Launches the GUI using `PresenterManager` to display the results.
 */
int main()
{
    std::string path = "/home/martin/results/casinobin/";

    // Initialize the output manager with the target folder for binary output
    CasinoBinOutputManager casinoOutputMnanager(path);
    casinoOutputMnanager.setName("Replication"); // Replications will be named "ReplicationX"

    // Run 1 replication and save its results
    for (size_t i = 0; i < 1; i++)
    {
        casinoOutputMnanager.newReplication();               // Create a new folder for this replication
        casinoOutputMnanager.writeResults(simulateCasino()); // Write simulation results to files
    }

    // Close all file writers to finalize data
    casinoOutputMnanager.closeAllWriters();

    // Create a statistics manager and register custom casino statistics
    StatisticsManager statManager;
    statManager.addStatistics<CasinoBinStatistics>("CasinoBinStats");

    // Link the simulation output folder with the statistics manager
    std::vector<FolderStatistics> folders;
    folders.push_back(FolderStatistics(
        "Casino",                                        // Name displayed in the GUI
        path,                // Path to simulation data
        std::make_unique<StatisticsManager>(statManager) // Associated statistics manager
    ));

    // Get the singleton GUI manager instance
    auto *manager = PresenterManager::getInstance();

    // Register the folder and its stats manager with the GUI
    manager->setFolderStatisticsList(std::move(folders));

    // Launch the graphical interface
    manager->run();

    return 0;
}
