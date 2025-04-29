// #pragma once

// #include "../lib/include/Statistics.h"
// #include "CasinoBinManagers.h"
// #include "../lib/include/PresenterManager.h"
// #include <sstream>
// #include <iomanip>

// /**
//  * @brief Specializes in producing graphical representations of casino simulation results.
//  */
// class CasinoBinStatisticsGraph : public Statistics<CasinoBinInputManager>
// {
// private:
//     std::vector<arma::vec> armaVecs; ///< Aggregated results for each game type.

// public:
//     /**
//      * @brief Constructs the statistics object and allocates storage for 5 game result vectors.
//      */
//     CasinoBinStatisticsGraph() : Statistics<CasinoBinInputManager>()
//     {
//         armaVecs.resize(5, arma::vec());
//     }

//     /**
//      * @brief Processes a single replication and appends the results to internal vectors.
//      * @param index Index of the replication to process.
//      */
//     void processReplication(size_t index) override
//     {
//         auto rep = getInputManager().getReplication(index);

//         for (size_t i = 0; i < rep->getReaderCount(); i++)
//         {
//             auto reader = rep->getReader<CasinoBinReader>(i);
//             reader->load();
//             auto &data = reader->getData();

//             arma::vec convertedData(data.size());
//             for (size_t j = 0; j < data.size(); j++)
//             {
//                 convertedData(j) = data[j] ? *data[j] : 0.0;
//             }

//             if (i < armaVecs.size())
//             {
//                 armaVecs[i] = arma::join_cols(armaVecs[i], convertedData);
//             }

//             reader->flush();
//         }
//     }

//     /**
//      * @brief Clears all collected data and removes loaded replications.
//      */
//     void clearData() override
//     {
//         for (auto &vec : armaVecs)
//         {
//             vec.clear();
//         }

//         getInputManager().clearReplications();
//     }

//     /**
//      * @brief Computes the mean win rate for a specific game.
//      * @param index Index of the game (0–4).
//      * @return Mean value as a double, or 0.0 if index is invalid or data is empty.
//      */
//     double getMean(size_t index) const
//     {
//         if (index >= armaVecs.size() || armaVecs[index].empty())
//         {
//             return 0.0;
//         }
//         return arma::mean(armaVecs[index]);
//     }

//     /**
//      * @brief Registers a graph presenter to visualize win rates from all casino games.
//      * @param manager Presenter manager responsible for creating UI components.
//      */
//     void setupPresenters(PresenterManager *manager) override
//     {
//         auto graphPresenter = manager->createGraphPresenter();
//         std::vector<float> graphData = {
//             static_cast<float>(getMean(0) * 100),
//             static_cast<float>(getMean(1) * 100),
//             static_cast<float>(getMean(2) * 100),
//             static_cast<float>(getMean(3) * 100),
//             static_cast<float>(getMean(4) * 100)};

//         graphPresenter->setData(graphData);
//         graphPresenter->setLabels({"Ruleta AR", "Ruleta ALT", "Automaty", "Blackjack con", "Blackjack agg"});
//         graphPresenter->setTitle("Casino Game Win Rates");
//     }
// };
