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

        bool isEmpty();
        void shuffle();
        Card dealCard();
        void deckRefresh();

    private:
        vector<Card> cards;
        vector<Card> discard;
};