#pragma once

#include <string>

using namespace std;

struct Card {
    string suit;
    int rank;
};

// Node for link list
struct CardNode {
    Card data;
    CardNode* next;
    
    CardNode(Card c) {
        data = c;
        next = nullptr;
    }
};

class Deck {
public:
    Deck();
    ~Deck();  // Destructor to clean up linked list
    
    void printCard(const Card& card);
    void printDeck();
    int size();
    bool isEmpty();
    void shuffle();
    Card dealCard();
    void deckRefresh();
    void addCard(Card card);  // Add card to end of deck
    
private:
    CardNode* head;     // Head of linked list
    int deckSize;       // Track size
    
    // Helper functions
    void clearDeck();
};
