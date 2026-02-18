#pragma once
#include "Deck.h"

class Pyramid : public Deck {
public:
    Pyramid();
    void mainMenu();
    void printBoard();
    void clearBoard();
    void newGame();
    void currentPlay();


private:
    Card board[8][13];
};
