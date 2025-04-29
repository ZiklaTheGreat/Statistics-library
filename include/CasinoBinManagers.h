#pragma once

#include "../lib/include/OutputManager.h"
#include "../lib/include/InputManager.h"
#include "../lib/include/Replication.h"
#include "CasinoBin.h"

/**
 * @brief Manages binary output writers for different casino simulation results.
 */
class CasinoBinOutputManager : public OutputManager
{
public:
    CasinoBinOutputManager() : OutputManager() {}
    CasinoBinOutputManager(const std::string &path) : OutputManager(path) {}

    /**
     * @brief Initializes writers for each type of simulation result.
     */
    void init() override
    {
        std::string path = getCurrentReplicationPath();

        auto writer1 = std::make_shared<CasinoBinWriter>(path + "ruleta_red.csv");
        auto writer2 = std::make_shared<CasinoBinWriter>(path + "ruleta_alt.csv");
        auto writer3 = std::make_shared<CasinoBinWriter>(path + "automat.csv");
        auto writer4 = std::make_shared<CasinoBinWriter>(path + "blackjack_con.csv");
        auto writer5 = std::make_shared<CasinoBinWriter>(path + "blackjack_agg.csv");

        registerWriter(writer1);
        registerWriter(writer2);
        registerWriter(writer3);
        registerWriter(writer4);
        registerWriter(writer5);
    }

    // Getters for individual simulation result writers
    std::shared_ptr<CasinoBinWriter> getWriterRR() { return getWriter<CasinoBinWriter>(0); }
    std::shared_ptr<CasinoBinWriter> getWriterRA() { return getWriter<CasinoBinWriter>(1); }
    std::shared_ptr<CasinoBinWriter> getWriterA() { return getWriter<CasinoBinWriter>(2); }
    std::shared_ptr<CasinoBinWriter> getWriterBC() { return getWriter<CasinoBinWriter>(3); }
    std::shared_ptr<CasinoBinWriter> getWriterBA() { return getWriter<CasinoBinWriter>(4); }

    /**
     * @brief Writes a vector of simulation results to the corresponding writers.
     * @param results A vector of 5 double values from the simulation.
     */
    void writeResults(std::vector<double> results)
    {
        getWriterRR()->write(results[0]);
        getWriterRA()->write(results[1]);
        getWriterA()->write(results[2]);
        getWriterBC()->write(results[3]);
        getWriterBA()->write(results[4]);
    }
};

/**
 * @brief Manages binary input readers for loading simulation results.
 */
class CasinoBinReplication : public Replication
{
public:
    CasinoBinReplication() : Replication() {}
    CasinoBinReplication(const std::string &name) : Replication(name) {}

    /**
     * @brief Initializes readers for each type of simulation result.
     */
    void init() override
    {
        std::string path = getBasePath();

        auto reader1 = std::make_shared<CasinoBinReader>(path + "ruleta_red.csv");
        auto reader2 = std::make_shared<CasinoBinReader>(path + "ruleta_alt.csv");
        auto reader3 = std::make_shared<CasinoBinReader>(path + "automat.csv");
        auto reader4 = std::make_shared<CasinoBinReader>(path + "blackjack_con.csv");
        auto reader5 = std::make_shared<CasinoBinReader>(path + "blackjack_agg.csv");

        registerReader(reader1);
        registerReader(reader2);
        registerReader(reader3);
        registerReader(reader4);
        registerReader(reader5);
    }
};

/**
 * @brief Input manager for handling multiple CasinoBinReplication instances.
 */
class CasinoBinInputManager : public InputManager<CasinoBinReplication>
{
public:
    CasinoBinInputManager() = default;
    CasinoBinInputManager(const std::string &path) : InputManager(path) {}
};
