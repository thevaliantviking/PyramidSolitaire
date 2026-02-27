#pragma once
#include "Card.h"
#include "Deck.h"

class Pyramid : public Deck {
public:
    Pyramid();
    void mainMenu();
    void printBoard();
    void clearBoard();
    void newGame();
    void currentPlay();
    bool isExposed(int row, int col);
    bool makeMove(int r1, int c1, int r2, int c2);
    bool removeKing(int r, int c);
    bool hasValidMoves();
    void drawFromStock();

private:
    Card board[7][7];  // 7 rows for pyramid (1+2+3+4+5+6+7 = 28 cards)
    CardNode* stockPile;   // Stock pile as linked list
    Card currentCard;      // Current card drawn from stock
    bool hasDrawnCard;     // test if card is currently drawn
    int stockSize;

    void dealPyramid();
    void addToStock(Card card);
    Card drawFromStockList();
};
