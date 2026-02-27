#pragma once
#include <string>

struct Card {
    std::string suit;
    int rank;
};

struct CardNode {
    Card data;
    CardNode* next;

    CardNode(Card c) {
        data = c;
        next = nullptr;
    }
};