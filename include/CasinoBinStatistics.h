#pragma once

#include "../lib/include/Statistics.h"
#include "CasinoBinManagers.h"
#include "../lib/include/PresenterManager.h"
#include <sstream>
#include <iomanip>

/**
 * @brief Collects and processes statistical data from binary casino simulation results.
 */
class CasinoBinStatistics : public Statistics<CasinoBinInputManager>
{
private:
    std::vector<arma::vec> armaVecs; ///< Stores aggregated results for each game.

public:
    CasinoBinStatistics() : Statistics<CasinoBinInputManager>()
    {
        armaVecs.resize(5, arma::vec());
    }

    /**
     * @brief Processes a single replication by reading data and appending it to internal vectors.
     * @param index Index of the replication to process.
     */
    void processReplication(size_t index) override
    {
        auto rep = getInputManager().getReplication(index);

        for (size_t i = 0; i < rep->getReaderCount(); i++)
        {
            auto reader = rep->getReader<CasinoBinReader>(i);
            reader->load();
            auto &data = reader->getData();

            arma::vec convertedData(data.size());
            for (size_t j = 0; j < data.size(); j++)
            {
                convertedData(j) = data[j] ? *data[j] : 0.0;
            }

            if (i < armaVecs.size())
            {
                armaVecs[i] = arma::join_cols(armaVecs[i], convertedData);
            }

            reader->flush();
        }
    }

    /**
     * @brief Clears all stored statistical data and loaded replications.
     */
    void clearData() override
    {
        for (auto &vec : armaVecs)
        {
            vec.clear();
        }

        getInputManager().clearReplications();
    }

    /**
     * @brief Returns the mean value for a given game index.
     * @param index Index of the game (0–4).
     * @return Mean of the recorded results, or 0.0 if empty or out of range.
     */
    double getMean(size_t index) const
    {
        if (index >= armaVecs.size() || armaVecs[index].empty())
        {
            return 0.0;
        }
        return arma::mean(armaVecs[index]);
    }

    /**
     * @brief Prepares and registers presenters for visualizing simulation statistics.
     * @param manager Presenter manager used to handle visualization outputs.
     */
    void setupPresenters(PresenterManager *manager) override
    {
        // Text presentation
        auto textPresenter = manager->createTextPresenter();
        std::ostringstream textStream;
        textStream << std::fixed << std::setprecision(2);
        textStream << "Ruleta AR: " << getMean(0) * 100 << "%\n"
                   << "Ruleta ALT: " << getMean(1) * 100 << "%\n"
                   << "Automaty: " << getMean(2) * 100 << "%\n"
                   << "Blackjack con: " << getMean(3) * 100 << "%\n"
                   << "Blackjack agg: " << getMean(4) * 100 << "%";
        textPresenter->setText(textStream.str());

        // Table presentation
        auto tablePresenter = manager->createTablePresenter();
        tablePresenter->addRow({"Game", "Win Rate"});

        std::ostringstream valueStream;
        valueStream << std::fixed << std::setprecision(2);

        valueStream.str("");
        valueStream << getMean(0) * 100 << "%";
        tablePresenter->addRow({"Ruleta AR", valueStream.str()});

        valueStream.str("");
        valueStream << getMean(1) * 100 << "%";
        tablePresenter->addRow({"Ruleta ALT", valueStream.str()});

        valueStream.str("");
        valueStream << getMean(2) * 100 << "%";
        tablePresenter->addRow({"Automaty", valueStream.str()});

        valueStream.str("");
        valueStream << getMean(3) * 100 << "%";
        tablePresenter->addRow({"Blackjack con", valueStream.str()});

        valueStream.str("");
        valueStream << getMean(4) * 100 << "%";
        tablePresenter->addRow({"Blackjack agg", valueStream.str()});

        // Graph presentation
        auto graphPresenter = manager->createGraphPresenter();
        std::vector<float> graphData = {
            static_cast<float>(getMean(0) * 100),
            static_cast<float>(getMean(1) * 100),
            static_cast<float>(getMean(2) * 100),
            static_cast<float>(getMean(3) * 100),
            static_cast<float>(getMean(4) * 100)};
        graphPresenter->setData(graphData);
        graphPresenter->setLabels({"Ruleta AR", "Ruleta ALT", "Automaty", "Blackjack con", "Blackjack agg"});
        graphPresenter->setTitle("Casino Game Win Rates");
        graphPresenter->setSize(ImVec2(0, 300)); // Width 0 means auto-width
        graphPresenter->setScale(0.0f, 100.0f);  // Scale from 0% to 100%
    }
};
