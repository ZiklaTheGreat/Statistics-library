#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random> // Add this for modern random number generation

/**
 * @brief Simulates a simple roulette game with different betting strategies.
 */
class Ruleta
{
public:
    enum Strategy
    {
        ALWAYS_RED, ///< Always bets on red.
        ALTERNATE   ///< Alternates between red and black.
    };

    /**
     * @brief Constructs the roulette game with a chosen strategy.
     */
    Ruleta(Strategy strat) : strategy(strat), lastBetRed(true), gen(rd()), dist(0, 1) {}

    /**
     * @brief Simulates one round of the game.
     * @return True if the player's bet matches the outcome.
     */
    bool play()
    {
        bool winningRed = dist(gen); // 50% chance of red or black
        bool playerBetRed = (strategy == ALWAYS_RED) ? true : lastBetRed;
        lastBetRed = !lastBetRed; // Alternate bet for ALTERNATE strategy
        return playerBetRed == winningRed;
    }

private:
    Strategy strategy;
    bool lastBetRed;
    static std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;
};

// Static member definition
std::random_device Ruleta::rd;

/**
 * @brief Simulates a slot machine with a configurable win probability.
 */
class Automaty
{
public:
    /**
     * @brief Constructs the slot machine with a given win probability.
     */
    Automaty(double winProb = 0.2) : winProbability(winProb), gen(rd()), dist(0.0, 1.0) {}

    /**
     * @brief Simulates one spin of the slot machine.
     * @return True if the player wins.
     */
    bool play()
    {
        return dist(gen) < winProbability; // 20% chance to win
    }

private:
    double winProbability;
    static std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;
};

// Static member definition
std::random_device Automaty::rd;

/**
 * @brief Simulates a simple Blackjack game with player strategies.
 */
class Blackjack
{
public:
    enum Strategy
    {
        CONSERVATIVE, ///< Stops hitting after reaching 12.
        AGGRESSIVE    ///< Hits until reaching at least 17.
    };

    /**
     * @brief Constructs the Blackjack game with a given strategy.
     */
    Blackjack(Strategy strat) : strategy(strat)
    {
        resetDeck();
    }

    /**
     * @brief Simulates one game of Blackjack.
     * @return True if the player wins.
     */
    bool play()
    {
        std::vector<int> playerHand, dealerHand;

        // Initial two cards
        playerHand.push_back(drawCard());
        playerHand.push_back(drawCard());
        dealerHand.push_back(drawCard());
        dealerHand.push_back(drawCard());

        int playerScore = getBestScore(playerHand);
        int dealerScore = getBestScore(dealerHand);

        // Player strategy execution
        while (true)
        {
            if (strategy == CONSERVATIVE && playerScore >= 12)
                break;
            if (strategy == AGGRESSIVE && playerScore >= 17)
                break;

            playerHand.push_back(drawCard());
            playerScore = getBestScore(playerHand);

            if (playerScore > 21)
                return false; // Player busts
        }

        // Dealer hits until reaching at least 17
        while (dealerScore < 17)
        {
            dealerHand.push_back(drawCard());
            dealerScore = getBestScore(dealerHand);
        }

        // Determine winner
        if (playerScore > 21)
            return false;
        if (dealerScore > 21)
            return true;
        return playerScore > dealerScore;
    }

private:
    Strategy strategy;
    std::vector<int> deck;

    /**
     * @brief Resets and shuffles the deck.
     */
    void resetDeck()
    {
        deck.clear();
        for (int i = 0; i < 4; i++)
        {
            for (int j = 2; j <= 10; j++)
                deck.push_back(j); // Cards 2-10
            deck.push_back(11);    // Ace
            deck.push_back(10);    // J
            deck.push_back(10);    // Q
            deck.push_back(10);    // K
        }

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::shuffle(deck.begin(), deck.end(), gen);
    }

    /**
     * @brief Draws a card from the deck.
     * @return The card value.
     */
    int drawCard()
    {
        if (deck.empty())
            resetDeck();
        int card = deck.back();
        deck.pop_back();
        return card;
    }

    /**
     * @brief Calculates the best score for a given hand, accounting for Aces.
     * @param hand The vector of card values.
     * @return The best possible hand value without busting.
     */
    int getBestScore(std::vector<int> hand)
    {
        int score = 0;
        int aceCount = 0;

        for (int card : hand)
        {
            score += card;
            if (card == 11)
                aceCount++;
        }

        while (score > 21 && aceCount > 0)
        {
            score -= 10; // Convert Ace from 11 to 1
            aceCount--;
        }

        return score;
    }
};

/**
 * @brief Simulates 100 players playing different casino games and collects win rates.
 * @return A vector of win rates for each game/strategy.
 */
std::vector<double> simulateCasino() {
    int numPlayers = 100;
    std::vector<double> results(5, 0.0);

    for (int i = 0; i < numPlayers; i++) {
        results[0] += Ruleta(Ruleta::ALWAYS_RED).play() ? 1.0 : 0.0;
        results[1] += Ruleta(Ruleta::ALTERNATE).play() ? 1.0 : 0.0;
        results[2] += Automaty().play() ? 1.0 : 0.0;
        results[3] += Blackjack(Blackjack::CONSERVATIVE).play() ? 1.0 : 0.0;
        results[4] += Blackjack(Blackjack::AGGRESSIVE).play() ? 1.0 : 0.0;
    }

    // Convert counts to percentages
    for (auto& result : results) {
        result /= numPlayers;
    }

    return results;
}
