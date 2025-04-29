#pragma once

#include "FolderStatistics.h"
#include "InputManager.h"
#include "Presenter.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <filesystem>
#include <string>
#include <functional>
#include <algorithm> // For std::sort

/**
 * @brief A singleton class that manages the presentation and interaction with the UI.
 * It handles the creation of presenters, folder selection, and rendering of content using OpenGL and ImGui.
 */
class PresenterManager
{
private:
    static std::unique_ptr<PresenterManager> instance;  ///< Singleton instance of the PresenterManager
    std::vector<std::shared_ptr<Presenter>> presenters; ///< List of presenters (text, table, graph, etc.)
    GLFWwindow *window;                                 ///< The OpenGL window for rendering using GLFW

    std::vector<FolderStatistics> folderStatisticsList; ///< List of folder statistics (replication data)
    int selectedFolderIndex = -1;                       ///< Index of the currently selected folder statistics

    std::string currentPath;              ///< The current file system path being used
    std::vector<std::string> folderNames; ///< List of folder names available for selection
    std::vector<bool> folderSelections;   ///< Boolean list to track the selection of folders
    bool showFolderBrowser = false;       ///< Flag to toggle visibility of the folder browser
    bool showResults = false;             ///< Flag to toggle visibility of the results

    bool showStatisticsSelector = false;      ///< Flag to toggle visibility of statistics selection
    std::vector<std::string> statisticsNames; ///< Names of available statistics
    std::vector<bool> statisticsSelections;   ///< Boolean list to track selected statistics

    /**
     * @brief Initializes the OpenGL context and the ImGui UI system.
     * Throws an exception if initialization fails.
     */
    void initOpenGL()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    window = glfwCreateWindow(1280, 720, "Statistics GUI", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    float dpi_scale = 1.0f;
    glfwGetWindowContentScale(window, &dpi_scale, nullptr);
    if (dpi_scale > 0.0f)
    {
        io.FontGlobalScale = dpi_scale;
        ImGui::GetStyle().ScaleAllSizes(dpi_scale);
    }
    else
    {
        io.FontGlobalScale = 2.0f;
        ImGui::GetStyle().ScaleAllSizes(2.0f);
    }

    io.Fonts->AddFontDefault();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

    /**
     * @brief Cleans up OpenGL and ImGui resources.
     */
    void cleanup()
    {
        if (ImGui::GetCurrentContext())
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
        if (window)
        {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        glfwTerminate();
    }

    /**
     * @brief Scans the folder at the specified path and updates the list of folder names and their selection status.
     * @param path The path to scan for folders.
     */
    void scanFolders(std::string_view path) {
        folderNames.clear();
        folderSelections.clear();

        try {
            std::vector<std::filesystem::directory_entry> entries;
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (entry.is_directory()) {
                    entries.push_back(entry);
                }
            }

            std::sort(entries.begin(), entries.end(),
                      [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
                          return a.path().filename().string() < b.path().filename().string();
                      });

            folderNames.reserve(entries.size());
            folderSelections.reserve(entries.size());
            for (const auto& entry : entries) {
                folderNames.push_back(entry.path().filename().string());
                folderSelections.push_back(false);
            }
        } catch (const std::exception& e) {
            std::cerr << std::format("Error scanning folders: {}", e.what()) << "\n";
        }
    }

    /**
     * @brief Processes the folders that have been selected by the user and updates the results.
     */
    void processSelectedFolders()
    {
        if (selectedFolderIndex < 0 || selectedFolderIndex >= static_cast<int>(folderStatisticsList.size()))
            return;

        std::vector<std::string> selectedFolders;
        for (size_t i = 0; i < folderNames.size(); i++)
        {
            if (folderSelections[i])
            {
                selectedFolders.push_back(folderNames[i]);
            }
        }

        std::vector<std::string> selectedStats;
        for (size_t i = 0; i < statisticsNames.size(); i++)
        {
            if (statisticsSelections[i])
            {
                selectedStats.push_back(statisticsNames[i]);
            }
        }

        auto &selectedFolderStats = folderStatisticsList[selectedFolderIndex];

        // Only process if we have both folders and statistics selected
        if (!selectedFolders.empty() && !selectedStats.empty() && selectedFolderStats.statistics)
        {
            clearPresenters();

            // Process each selected statistic
            for (const auto &statName : selectedStats)
            {
                try
                {
                    // Get the specific statistics object
                    auto statObj = selectedFolderStats.statistics->getStatistics(statName);

                    // Clear previous data for this statistics
                    statObj->clearData();

                    // Set base path and load folders for this statistics
                    statObj->setBasePath(selectedFolderStats.path);
                    statObj->loadFolders(selectedFolders);

                    // Process this statistics
                    statObj->processAllReplications();

                    // Use the new setupPresenters method directly on the statistics object
                    statObj->setupPresenters(this);
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error processing statistics " << statName << ": " << e.what() << std::endl;
                }
            }

            showResults = true;
        }
    }

    /**
     * @brief Displays the UI for folder statistics selection.
     */
    void showFolderStatisticsSelection()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("Folder Statistics Selection");
    // Zvyšok kódu zostáva nezmenený
    for (size_t i = 0; i < folderStatisticsList.size(); ++i)
    {
        if (ImGui::Selectable(folderStatisticsList[i].name.c_str(), selectedFolderIndex == static_cast<int>(i)))
        {
            selectedFolderIndex = static_cast<int>(i);
            showFolderBrowser = true;
            showStatisticsSelector = true;
            scanFolders(folderStatisticsList[i].path);

            statisticsNames.clear();
            statisticsSelections.clear();
            FolderStatistics &selectedStats = folderStatisticsList[selectedFolderIndex];
            if (selectedStats.statistics)
            {
                for (const auto &statName : selectedStats.statistics->getStatisticsNames())
                {
                    statisticsNames.push_back(statName);
                    statisticsSelections.push_back(false);
                }
            }
        }
    }
    ImGui::End();
}

    /**
     * @brief Displays the folder browser UI for selecting folders to process.
     */
    void showFolderBrowserF()
{
    if (!showFolderBrowser || selectedFolderIndex == -1)
        return;

    ImGui::SetNextWindowPos(ImVec2(10, 150), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    ImGui::Begin("Folder Browser", &showFolderBrowser);
    // Zvyšok kódu zostáva nezmenený
    FolderStatistics &selectedStats = folderStatisticsList[selectedFolderIndex];
    ImGui::Text("Selected Statistics: %s", selectedStats.name.c_str());
    ImGui::Text("Path: %s", selectedStats.path.c_str());

    ImGui::BeginChild("FolderList", ImVec2(0, 250), true);
    for (size_t i = 0; i < folderNames.size(); i++)
    {
        bool selected = folderSelections[i];
        if (ImGui::Checkbox(folderNames[i].c_str(), &selected))
        {
            folderSelections[i] = selected;
        }
    }
    ImGui::EndChild();

    if (ImGui::Button("Select All"))
    {
        for (size_t i = 0; i < folderSelections.size(); i++)
        {
            folderSelections[i] = true;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Deselect All"))
    {
        for (size_t i = 0; i < folderSelections.size(); i++)
        {
            folderSelections[i] = false;
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Process Selected"))
    {
        processSelectedFolders();
    }
    ImGui::End();
}

    /**
     * @brief Displays the statistics selector UI
     */
    void showStatisticsSelectorWindow()
    {
        if (!showStatisticsSelector || selectedFolderIndex == -1)
            return;
    
        ImGui::SetNextWindowPos(ImVec2(10, 500), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
        ImGui::Begin("Select Statistics to Process", &showStatisticsSelector);
        // Zvyšok kódu zostáva nezmenený
        FolderStatistics &selectedStats = folderStatisticsList[selectedFolderIndex];
        ImGui::Text("Available Statistics for: %s", selectedStats.name.c_str());
    
        ImGui::BeginChild("StatsList", ImVec2(0, 150), true);
        for (size_t i = 0; i < statisticsNames.size(); i++)
        {
            bool selected = statisticsSelections[i];
            if (ImGui::Checkbox(statisticsNames[i].c_str(), &selected))
            {
                statisticsSelections[i] = selected;
            }
        }
        ImGui::EndChild();
    
        if (ImGui::Button("Select All"))
        {
            for (size_t i = 0; i < statisticsSelections.size(); i++)
            {
                statisticsSelections[i] = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Deselect All"))
        {
            for (size_t i = 0; i < statisticsSelections.size(); i++)
            {
                statisticsSelections[i] = false;
            }
        }
        ImGui::End();
    }

public:
    /**
     * @brief Destructor that ensures cleanup of OpenGL and ImGui resources.
     */
    ~PresenterManager()
    {
        cleanup();
    }

    /**
     * @brief Gets the singleton instance of the PresenterManager.
     * @return The singleton instance of the PresenterManager.
     */
    static PresenterManager *getInstance()
    {
        if (!instance)
        {
            instance = std::make_unique<PresenterManager>();
        }
        return instance.get();
    }

    /**
     * @brief Creates and adds a new TextPresenter to the presenters list.
     * @return A shared pointer to the newly created TextPresenter.
     */
    std::shared_ptr<TextPresenter> createTextPresenter()
    {
        auto presenter = std::make_shared<TextPresenter>();
        presenters.push_back(presenter);
        return presenter;
    }

    /**
     * @brief Creates and adds a new TablePresenter to the presenters list.
     * @return A shared pointer to the newly created TablePresenter.
     */
    std::shared_ptr<TablePresenter> createTablePresenter()
    {
        auto presenter = std::make_shared<TablePresenter>();
        presenters.push_back(presenter);
        return presenter;
    }

    /**
     * @brief Creates and adds a new GraphPresenter to the presenters list.
     * @return A shared pointer to the newly created GraphPresenter.
     */
    std::shared_ptr<GraphPresenter> createGraphPresenter()
    {
        auto presenter = std::make_shared<GraphPresenter>();
        presenters.push_back(presenter);
        return presenter;
    }

    void setPresenters(std::vector<std::shared_ptr<Presenter>> newPresenters) noexcept {
        presenters = std::move(newPresenters);
    }

    void setFolderStatisticsList(std::vector<FolderStatistics> folders) noexcept {
        folderStatisticsList = std::move(folders);
        selectedFolderIndex = -1;
        showFolderBrowser = false;
    }

    void clearPresenters() noexcept {
        presenters.clear();
    }

    /**
     * @brief Runs the main loop of the application, rendering the UI and handling user interactions.
     */
    void run()
    {
        initOpenGL();

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            showFolderStatisticsSelection();
            showFolderBrowserF();
            showStatisticsSelectorWindow();

            if (showResults)
            {
                for (const auto &presenter : presenters)
                {
                    presenter->show();
                }
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }

        cleanup();
    }
};

// Initialize the static instance pointer to nullptr
std::unique_ptr<PresenterManager> PresenterManager::instance = nullptr;
