#include "Deck.h"
#include <iostream>
#include <random>
#include <vector>

using namespace std;

const string SUIT[] = {"♥", "♦", "♧", "♤"};
const int RANK[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

Deck::Deck() : head(nullptr), tail(nullptr), deckSize(0) {
    // Create all 52 cards and add to linked list
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            Card card;
            card.suit = SUIT[i];
            card.rank = RANK[j];
            addCard(card);
        }
    }
}

Deck::~Deck() {
    clearDeck();
}

void Deck::clearDeck() {
    CardNode* current = head;
    while (current != nullptr) {
        CardNode* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    deckSize = 0;
}

void Deck::addCard(Card card) {
    CardNode* newNode = new CardNode(card);
    
    if (head == nullptr) {
        // Empty list
        head = newNode;
        tail = newNode;
    } else {
        // Add to end
        tail->next = newNode;
        tail = newNode;
    }
    deckSize++;
}

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
}

void Deck::printDeck() {
    CardNode* current = head;
    while (current != nullptr) {
        printCard(current->data);
        cout << " ";
        current = current->next;
    }
    cout << endl;
}

int Deck::size() {
    return deckSize;
}

bool Deck::isEmpty() {
    return head == nullptr;
}

CardNode* Deck::getNodeAt(int index) {
    if (index < 0 || index >= deckSize) {
        return nullptr;
    }
    
    CardNode* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

void Deck::shuffle() {
    if (deckSize <= 1) return;
    
    // Convert linked list to vector for shuffling
    vector<Card> tempCards;
    CardNode* current = head;
    while (current != nullptr) {
        tempCards.push_back(current->data);
        current = current->next;
    }

    random_device rd;
    mt19937 gen(rd());
    
    for (int i = tempCards.size() - 1; i > 0; i--) {
        int j = gen() % (i + 1);
        swap(tempCards[i], tempCards[j]);
    }
    
    // rebuild linked list with shuffled cards
    clearDeck();
    for (const Card& card : tempCards) {
        addCard(card);
    }
}

Card Deck::dealCard() {
    if (isEmpty()) {
        Card empty;
        empty.rank = 0;
        empty.suit = "";
        return empty;
    }
    
    // Remove from end (tail)
    if (head == tail) {
        // Only one card
        Card card = head->data;
        delete head;
        head = nullptr;
        tail = nullptr;
        deckSize--;
        return card;
    }
    
    // Find second-to-last node
    CardNode* current = head;
    while (current->next != tail) {
        current = current->next;
    }
    
    Card card = tail->data;
    delete tail;
    tail = current;
    tail->next = nullptr;
    deckSize--;
    
    return card;
}

void Deck::deckRefresh() {
    clearDeck();
    
    // Recreate deck
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            Card card;
            card.suit = SUIT[i];
            card.rank = RANK[j];
            addCard(card);
        }
    }
    shuffle();
}
