// #ifndef __CASINOPRESENTER_H__
// #define __CASINOPRESENTER_H__

// #include "../lib/include/PresenterManager.h"
// #include "CasinoStatistics.h"
// #include <sstream>
// #include <iomanip>

// class CasinoPresenter : public IStatisticsPresenter {
//     private:
//         PresenterManager* presenterManager;
    
//     public:
//         CasinoPresenter(PresenterManager* manager) : presenterManager(manager) {}
    
//         void setupPresenters(const IStatistics& data) override {
//             const CasinoStatistics* stats = dynamic_cast<const CasinoStatistics*>(&data);
//             if (!stats) {
//                 throw std::runtime_error("Invalid statistics type");
//             }
    
//             // Text presentation (unchanged)
//             auto textPresenter = presenterManager->createTextPresenter();
//             std::ostringstream textStream;
//             textStream << std::fixed << std::setprecision(2);
//             textStream << "Ruleta AR: " << stats->getMean(0) * 100 << "%\n"
//                       << "Ruleta ALT: " << stats->getMean(1) * 100 << "%\n"
//                       << "Automaty: " << stats->getMean(2) * 100 << "%\n"
//                       << "Blackjack con: " << stats->getMean(3) * 100 << "%\n"
//                       << "Blackjack agg: " << stats->getMean(4) * 100 << "%";
//             textPresenter->setText(textStream.str());
    
//             // Table presentation (unchanged)
//             auto tablePresenter = presenterManager->createTablePresenter();
//             tablePresenter->addRow({"Metric", "Win Rate"});
            
//             std::ostringstream valueStream;
//             valueStream << std::fixed << std::setprecision(2);
//             valueStream.str(""); valueStream << stats->getMean(0) * 100 << "%";
//             tablePresenter->addRow({"Ruleta AR", valueStream.str()});
//             valueStream.str(""); valueStream << stats->getMean(1) * 100 << "%";
//             tablePresenter->addRow({"Ruleta ALT", valueStream.str()});
//             valueStream.str(""); valueStream << stats->getMean(2) * 100 << "%";
//             tablePresenter->addRow({"Automaty", valueStream.str()});
//             valueStream.str(""); valueStream << stats->getMean(3) * 100 << "%";
//             tablePresenter->addRow({"Blackjack con", valueStream.str()});
//             valueStream.str(""); valueStream << stats->getMean(4) * 100 << "%";
//             tablePresenter->addRow({"Blackjack agg", valueStream.str()});
    
//             // Enhanced Graph presentation
//             auto graphPresenter = presenterManager->createGraphPresenter();
//             std::vector<float> graphData = {
//                 static_cast<float>(stats->getMean(0) * 100),
//                 static_cast<float>(stats->getMean(1) * 100),
//                 static_cast<float>(stats->getMean(2) * 100),
//                 static_cast<float>(stats->getMean(3) * 100),
//                 static_cast<float>(stats->getMean(4) * 100)
//             };
            
//             graphPresenter->setData(graphData);
//             graphPresenter->setLabels({"Ruleta AR", "Ruleta ALT", "Automaty", "Blackjack con", "Blackjack agg"});
//             graphPresenter->setTitle("Casino Game Win Rates");
//             graphPresenter->setSize(ImVec2(0, 300)); // Width 0 means auto-width
//             graphPresenter->setScale(0.0f, 100.0f); // Scale from 0% to 100%
//         }
//     };

// #endif // __CASINOPRESENTER_H__