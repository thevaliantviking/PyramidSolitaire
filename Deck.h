#pragma once

#include <vector>
#include <string>

using namespace std;

struct Card{
    string suit;
    int rank;
};

class Deck {
    public:
        Deck();
        void printCard(const Card& card);
        void printDeck();
        int size();
        void shuffle();
        Card dealCard();
        void deckRefresh();
        Card

    private:
        vector<Card> cards;
};