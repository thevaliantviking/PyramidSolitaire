#pragma once

#include "Card.h"

class Deck {
public:
    Deck();
    ~Deck();
    
    void printCard(const Card& card);
    void printDeck();
    int size();
    bool isEmpty();
    void shuffle();
    Card dealCard();
    void deckRefresh();
    void addCard(Card card);
    
private:
    CardNode* head;
    int deckSize;

    void clearDeck();
};
