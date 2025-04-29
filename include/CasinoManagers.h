#ifndef __CASINOMANAGERS_H__
#define __CASINOMANAGERS_H__

#include "../lib/include/OutputManager.h"
#include "../lib/include/InputManager.h"
#include "../lib/include/Replication.h"
#include "CasinoCSV.h"

class CasinoOutputManager : public OutputManager
{
public:
    CasinoOutputManager() : OutputManager() {}
    CasinoOutputManager(const std::string &path) : OutputManager(path) {}

    void init() override
    {
        std::string path = getCurrentReplicationPath();

        auto writer1 = std::make_shared<CasinoCSVWriter>(path + "ruleta_red.csv");
        auto writer2 = std::make_shared<CasinoCSVWriter>(path + "ruleta_alt.csv");
        auto writer3 = std::make_shared<CasinoCSVWriter>(path + "automat.csv");
        auto writer4 = std::make_shared<CasinoCSVWriter>(path + "blackjack_con.csv");
        auto writer5 = std::make_shared<CasinoCSVWriter>(path + "blackjack_agg.csv");

        registerWriter(writer1);
        registerWriter(writer2);
        registerWriter(writer3);
        registerWriter(writer4);
        registerWriter(writer5);
    }

    std::shared_ptr<CasinoCSVWriter> getWriterRR() { return getWriter<CasinoCSVWriter>(0); }
    std::shared_ptr<CasinoCSVWriter> getWriterRA() { return getWriter<CasinoCSVWriter>(1); }
    std::shared_ptr<CasinoCSVWriter> getWriterA() { return getWriter<CasinoCSVWriter>(2); }
    std::shared_ptr<CasinoCSVWriter> getWriterBC() { return getWriter<CasinoCSVWriter>(3); }
    std::shared_ptr<CasinoCSVWriter> getWriterBA() { return getWriter<CasinoCSVWriter>(4); }

    void writeResults(std::vector<double> results)
    {
        getWriterRR()->write(results[0]);
        getWriterRA()->write(results[1]);
        getWriterA()->write(results[2]);
        getWriterBC()->write(results[3]);
        getWriterBA()->write(results[4]);
    }
};

class CasinoReplication : public Replication
{
public:
    CasinoReplication() : Replication() {}
    CasinoReplication(const std::string &name) : Replication(name) {}

    void init() override
    {
        std::string path = getBasePath();

        auto reader1 = std::make_shared<CasinoCSVReader>(path + "ruleta_red.csv");
        auto reader2 = std::make_shared<CasinoCSVReader>(path + "ruleta_alt.csv");
        auto reader3 = std::make_shared<CasinoCSVReader>(path + "automat.csv");
        auto reader4 = std::make_shared<CasinoCSVReader>(path + "blackjack_con.csv");
        auto reader5 = std::make_shared<CasinoCSVReader>(path + "blackjack_agg.csv");

        registerReader(reader1);
        registerReader(reader2);
        registerReader(reader3);
        registerReader(reader4);
        registerReader(reader5);
    }
};

class CasinoInputManager : public InputManager<CasinoReplication>
{
public:
    CasinoInputManager() = default;
    CasinoInputManager(const std::string &path) : InputManager(path) {}
};
#endif // __CASINOMANAGERS_H__