#pragma once

#include "imgui.h"
#include <vector>
#include <string>

/**
 * @brief Abstract base class for all presenters.
 *
 * This class defines the common interface for displaying data in different formats such as text, tables, and graphs.
 * Each subclass must implement the `show()` method to render their specific content.
 */
class Presenter
{
public:
    /**
     * @brief Abstract method to display the data.
     *
     * Each subclass of `Presenter` must override this method to implement their own rendering logic.
     * This method is responsible for rendering the content specific to the type of presenter (text, table, graph, etc.).
     */
    virtual void show() = 0;

    virtual ~Presenter() = default; ///< Virtual destructor to ensure proper cleanup of derived classes.
};

/**
 * @brief Presenter class that displays a block of text.
 *
 * This class is used for showing static text in a window using ImGui.
 */
class TextPresenter : public Presenter
{
private:
    std::string text; ///< The text content to be displayed in the window.

public:
    /**
     * @brief Sets the text to be displayed in the window.
     *
     * This method allows updating the text content that will be rendered by the `show()` method.
     *
     * @param newText The string to be shown in the window.
     */
    void setText(std::string_view newText) { text = newText; }

    /**
     * @brief Displays the text inside an ImGui window.
     *
     * This method renders the text using ImGui's text rendering system, with automatic wrapping if the text exceeds the window width.
     * It begins by creating a new window titled "Text View", and then renders the wrapped text.
     */
    void show() override
    {
        ImGui::Begin("Text View");              // Begin a new ImGui window with the title "Text View"
        ImGui::TextWrapped("%s", text.c_str()); // Display the text, wrapping it if necessary
        ImGui::End();                           // End the ImGui window
    }
};

/**
 * @brief Presenter class that displays data in a table format.
 *
 * This class renders a table where each cell can contain a string value. It is suitable for displaying structured data in rows and columns.
 */
class TablePresenter : public Presenter
{
private:
    std::vector<std::vector<std::string>> rows; ///< A 2D vector holding rows, where each row is a vector of strings representing cells.

public:
    /**
     * @brief Adds a row to the table.
     *
     * This method allows dynamically adding new rows to the table. Each row is a vector of strings, where each string is a cell in the row.
     *
     * @param row A vector containing the cell values for the new row.
     */
    void addRow(const std::vector<std::string> &row) { rows.push_back(row); }

    /**
     * @brief Displays the data in a table format inside an ImGui window.
     *
     * This method creates a table with the appropriate number of columns and populates it with rows and cells.
     * Each row is iterated, and for each cell in the row, its content is displayed in the corresponding column.
     */
    void show() override
    {
        ImGui::Begin("Table View"); // Begin a new ImGui window with the title "Table View"

        // Determine the number of columns (default to 1 if no rows exist)
        size_t columns = rows.empty() ? 1 : rows[0].size();
        if (ImGui::BeginTable("Table", columns)) // Create the table with the correct number of columns
        {
            // Loop through each row and render its cells
            for (const auto &row : rows)
            {
                ImGui::TableNextRow(); // Start a new row in the table
                // Loop through each cell in the row and render its content
                for (const auto &cell : row)
                {
                    ImGui::TableNextColumn();        // Move to the next column
                    ImGui::Text("%s", cell.c_str()); // Display the cell's text
                }
            }
            ImGui::EndTable(); // End the table
        }

        ImGui::End(); // Close the ImGui window
    }
};

/**
 * @brief Presenter class that displays data in a graph format (histogram).
 *
 * This class uses ImGui's plotting capabilities to render a histogram from the provided data values.
 */
class GraphPresenter : public Presenter
{
private:
    std::vector<float> values;
    std::vector<std::string> labels;
    ImVec2 graphSize = ImVec2(0, 400); // Default size (width 0 means auto-width)
    std::string title = "Data";
    float scaleMin = 0.0f;
    float scaleMax = 100.0f;

public:
    void setData(const std::vector<float> &newValues) { values = newValues; }
    void setSize(const ImVec2 &size) { graphSize = size; }
    void setScale(float min, float max)
    {
        scaleMin = min;
        scaleMax = max;
    }
    void setTitle(std::string_view newTitle) { title = newTitle; }
    void setLabels(const std::vector<std::string_view> &newLabels)
    {
        labels.clear();
        labels.reserve(newLabels.size());
        for (const auto &label : newLabels)
        {
            labels.emplace_back(label);
        }
    }

    void show() override
    {
        ImGui::Begin("Graph View", nullptr, ImGuiWindowFlags_NoScrollbar);

        // Make the graph fill the available width
        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        ImVec2 graphDisplaySize = ImVec2(availableSize.x, graphSize.y);

        if (!values.empty())
        {
            if (!labels.empty() && labels.size() == values.size())
            {
                // Custom bar graph with labels
                ImGui::PlotHistogram(
                    title.c_str(),
                    values.data(),
                    values.size(),
                    0,       // Start index
                    nullptr, // Overlay text
                    scaleMin,
                    scaleMax,
                    graphDisplaySize);

                // Add labels under each bar
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));
                float barWidth = availableSize.x / values.size();
                for (size_t i = 0; i < labels.size(); ++i)
                {
                    if (i > 0)
                        ImGui::SameLine();
                    ImGui::SetCursorPosX(i * barWidth + (barWidth - ImGui::CalcTextSize(labels[i].c_str()).x) * 0.5f);
                    ImGui::Text("%s", labels[i].c_str());
                }
                ImGui::PopStyleVar();
            }
            else
            {
                // Standard histogram if no labels
                ImGui::PlotHistogram(
                    title.c_str(),
                    values.data(),
                    values.size(),
                    0,       // Start index
                    nullptr, // Overlay text
                    scaleMin,
                    scaleMax,
                    graphDisplaySize);
            }
        }

        ImGui::End();
    }
};
