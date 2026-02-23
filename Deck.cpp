#include "Deck.h"
#include <iostream>
#include <random>

using namespace std;

const string SUIT[] = {"♥", "♦", "♧", "♤"};
const int RANK[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

Deck::Deck() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            Card card;
            card.suit = SUIT[i];
            card.rank = RANK[j];
            cards.push_back(card);
        }
    }
};

void Deck::printCard(const Card &card) {
    switch (card.rank) {
        case 1: cout << " A"; break;
        case 10: cout << "10"; break;
        case 11: cout << " J"; break;
        case 12: cout << " Q"; break;
        case 13: cout << " K"; break;
        default: cout << " " << card.rank; break;
    }
    cout << card.suit;
};

void Deck::printDeck() {
    for (const Card& card : cards) {
        printCard(card);
    }
    cout << endl;
};

int Deck::size() {
    return cards.size();
};

bool Deck::isEmpty() {
    return cards.empty();
}

void Deck::shuffle() {
    random_device rd;
    mt19937 gen(rd());

    for (int i = cards.size() - 1; i > 0; i--) {
        int j = gen() % (i + 1);
        swap(cards[i], cards[j]);
    }
};

Card Deck::dealCard() {
    Card card;
    card = cards.back();
    cards.pop_back();
    return card;
};

void Deck::deckRefresh() {
    cards.clear();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            Card card;
            card.suit = SUIT[i];
            card.rank = RANK[j];
            cards.push_back(card);
        }
    }
    shuffle();
}

