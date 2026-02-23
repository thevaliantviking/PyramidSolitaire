#include "Pyramid.h"
#include <iostream>
using namespace std;

Pyramid::Pyramid() {
    clearBoard();
}

void Pyramid::mainMenu() {
    char c;
    cout << "\n\t\tWelcome to the Pyramid!" << endl;
    cout << "\tA Game of Patience (solitaire)" << endl;
    cout << "______________________________________" << endl;
    cout << "To start a new game enter 'n' or 'N'" << endl;
    cout << "To quit enter 'q' or 'Q'" << endl;
    cout << "\nPlease make a selection: ";
    cin >> c;
    if (c == 'N' || c == 'n') {
        newGame();
    } else if (c == 'Q' || c == 'q') {
        return;
    } else {
        cout << "Invalid selection. Please try again!" << endl;
        mainMenu();
    }
}

void Pyramid::printBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 13; j++) {
            if (board[i][j].rank == 0) {
                cout << "   ";
            } else {
                printCard(board[i][j]);
            }
        }
        cout << endl;
    }
}

void Pyramid::clearBoard() {
    Card clear = {};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 13; j++) {
            board[i][j] = clear;
        }
    }
}

void Pyramid::newGame() {
    deckRefresh();
    clearBoard();
    cout << "\n______________________________________________" << endl;
    cout << "\t\t  CONQUER THE PYRAMID!!!\n\n";
    board[0][6] = dealCard();
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 13; j++) {
            if (board[i][j].rank != 0) {
                board[i + 1][j + 1] = dealCard();
                board[i + 1][j - 1] = dealCard();
            }
        }
    }
    vector<Card> pile;
    vector<Card> discard;
    Card currentCard;
    while (!isEmpty()) {

        currentCard = dealCard();
    }
    printBoard();
    currentPlay();
}

void Pyramid::currentPlay() {
    int round = 1;

   while (size() - i != -1) {

    }




    if (round > 3) {
        cout << "GAME OVER" << endl;
        cout << "Better luck next time!!!\n" << endl;
        mainMenu();
    }

    if (cards.isEmpty() && discard.isEmpty()) {

    }
}

