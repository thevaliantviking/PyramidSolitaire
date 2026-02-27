#include "Pyramid.h"
#include <iostream>
using namespace std;

Pyramid::Pyramid() {
    stockPile = nullptr;
    hasDrawnCard = false;
    stockSize = 0;
    clearBoard();
}

void Pyramid::mainMenu() {
    char c;
    cout << "\n\t\tWelcome to the Pyramid!" << endl;
    cout << "\tA Game of Patience (solitaire)" << endl;
    cout << "______________________________________" << endl;
    cout << "To start a new game enter 'n'" << endl;
    cout << "To print current deck enter 'd'" << endl;
    cout << "To shuffle current deck enter 's'" << endl;
    cout << "To quit game enter 'q'" << endl;
    cout << "\nPlease make a selection: ";
    cin >> c;
    if (c == 'N' || c == 'n') {
        newGame();
    } else if (c == 'D' || c == 'd') {
        printDeck();
        mainMenu();
    } else if (c == 'S' || c == 's') {
        shuffle();
        cout << "Current deck shuffled. To view enter 'd'." << endl;
        mainMenu();
    } else if (c == 'Q' || c == 'q') {
        return;
    } else {
        cout << "Invalid selection. Please try again!" << endl;
        mainMenu();
    }
}

void Pyramid::clearBoard() {
    Card clear;
    clear.rank = 0;
    clear.suit = "";
    
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            board[i][j] = clear;
        }
    }
    
    // Clear stock pile
    CardNode* current = stockPile;
    while (current != nullptr) {
        CardNode* next = current->next;
        delete current;
        current = next;
    }
    stockPile = nullptr;
    stockSize = 0;

    currentCard.rank = 0;
    currentCard.suit = "";
    hasDrawnCard = false;
}

void Pyramid::dealPyramid() {
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col <= row; col++) {
            board[row][col] = dealCard();
        }
    }
}

void Pyramid::addToStock(Card card) {
    CardNode* newNode = new CardNode(card);
    newNode->next = stockPile;
    stockPile = newNode;
    stockSize++;
}

Card Pyramid::drawFromStockList() {
    if (stockPile == nullptr) {
        Card empty;
        empty.rank = 0;
        empty.suit = "";
        return empty;
    }
    
    CardNode* temp = stockPile;
    Card card = temp->data;
    stockPile = stockPile->next;
    delete temp;
    stockSize--;
    
    return card;
}

void Pyramid::printBoard() {
    
    // Print pyramid with proper spacing
    for (int i = 0; i < 7; i++) {
        cout << (i+1) << "\t";
        // spacing to center
        for (int space = 0; space < (7 - i - 1) * 2; space++) {
            cout << " ";
        }

        for (int j = 0; j <= i; j++) {
            if (board[i][j].rank == 0) {
                cout << "    ";
            } else {
                printCard(board[i][j]);
            }
        }
        cout << endl;
    }
    
    cout << "\nStock pile: " << stockSize << " cards remaining" << endl;
    
    if (hasDrawnCard) {
        cout << "Current card: ";
        printCard(currentCard);
        cout << endl;
    } else {
        cout << "Current card: (none - press 'd' to draw)" << endl;
    }

}

void Pyramid::newGame() {
    deckRefresh();
    clearBoard();
    
    cout << "\n______________________________________________" << endl;
    cout << "\t\t  CONQUER THE PYRAMID!!!\n";
    cout << "______________________________________________\n" << endl;

    dealPyramid();
    
    //Cards to stock pile
    while (!isEmpty()) {
        addToStock(dealCard());
    }

    currentPlay();
}

bool Pyramid::isExposed(int row, int col) {
    // Check if card is exposed (no cards covering it)
    if (board[row][col].rank == 0) {
        return false;  // No card there
    }
    
    if (row == 6) {
        return true;  // Bottom row always exposed
    }
    
    // Check if both children are removed
    bool leftChildGone = (board[row + 1][col].rank == 0);
    bool rightChildGone = (board[row + 1][col + 1].rank == 0);
    
    return leftChildGone && rightChildGone;
}

bool Pyramid::makeMove(int r1, int c1, int r2, int c2) {
    --r1;
    --c1;
    --r2;
    --c2;
    if (r1 < 0 || r1 >= 7 || c1 < 0 || c1 >= 7 ||
        r2 < 0 || r2 >= 7 || c2 < 0 || c2 >= 7) {
        return false;
    }
    
    // Check if cards exist and are exposed
    if (!isExposed(r1, c1) || !isExposed(r2, c2)) {
        cout << "One or both cards are not exposed!" << endl;
        return false;
    }
    
    // Check if they add up to 13
    int sum = board[r1][c1].rank + board[r2][c2].rank;
    if (sum == 13) {
        cout << "Match! Removing cards." << endl;
        board[r1][c1].rank = 0;
        board[r2][c2].rank = 0;
        return true;
    } else {
        cout << "Cards don't add up to 13 (sum = " << sum << ")." << endl;
        return false;
    }
}

bool Pyramid::removeKing(int r, int c) {
    --r;
    --c;
    if (r < 0 || r >= 7 || c < 0 || c >= 7) {
        return false;
    }
    
    if (!isExposed(r, c)) {
        cout << "Card is not exposed!" << endl;
        return false;
    }
    
    if (board[r][c].rank == 13) {
        cout << "King removed!" << endl;
        board[r][c].rank = 0;
        return true;
    } else {
        cout << "That's not a King!" << endl;
        return false;
    }
}

bool Pyramid::hasValidMoves() {
    // Check if any valid moves exist in pyramid
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j <= i; j++) {
            if (board[i][j].rank != 0 && isExposed(i, j)) {
                // Check for Kings
                if (board[i][j].rank == 13) {
                    return true;
                }
                
                // Check for pairs
                for (int k = 0; k < 7; k++) {
                    for (int l = 0; l <= k; l++) {
                        if (board[k][l].rank != 0 && isExposed(k, l)) {
                            if (i != k || j != l) {  // Different card
                                if (board[i][j].rank + board[k][l].rank == 13) {
                                    return true;
                                }
                            }
                        }
                    }
                }
                
                // Check with current card from stock
                if (hasDrawnCard && board[i][j].rank + currentCard.rank == 13) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

void Pyramid::drawFromStock() {
    if (stockSize == 0) {
        cout << "Stock pile is empty!" << endl;
        return;
    }
    
    if (hasDrawnCard) {
        cout << "You already have a card drawn. Use it or pass your turn first." << endl;
        return;
    }
    
    currentCard = drawFromStockList();
    hasDrawnCard = true;
    cout << "Drew: ";
    printCard(currentCard);
    cout << endl;
}

void Pyramid::currentPlay() {
    bool gameOver = false;
    
    while (!gameOver) {
        printBoard();
        
        // Check win condition
        bool pyramidClear = true;
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j <= i; j++) {
                if (board[i][j].rank != 0) {
                    pyramidClear = false;
                    break;
                }
            }
            if (!pyramidClear) break;
        }

        //Won
        if (pyramidClear) {
            cout << "\n*** CONGRATULATIONS! YOU WIN! ***" << endl;
            cout << "You've cleared the pyramid!\n" << endl;
            mainMenu();
            return;
        }
        
        // Lost
        if (stockSize == 0 && !hasDrawnCard && !hasValidMoves()) {
            cout << "\n*** GAME OVER ***" << endl;
            cout << "No more moves available!\n" << endl;
            mainMenu();
            return;
        }
        
        cout << "\nCommands:" << endl;
        cout << "  p <row1> <col1> <row2> <col2> - Remove pair" << endl;
        cout << "  k <row> <col> - Remove King" << endl;
        cout << "  s - Remove king from current stock card" << endl;
        cout << "  m <row> <col> - Match with current stock card" << endl;
        cout << "  d - Draw from stock" << endl;
        cout << "  n - Discard current card and draw new" << endl;
        cout << "  q - Quit to menu" << endl;
        cout << "\nEnter command: ";
        
        char command;
        cin >> command;
        
        if (command == 'q' || command == 'Q') {
            mainMenu();
            return;
        } else if (command == 'd' || command == 'D') {
            drawFromStock();
        } else if (command == 'n' || command == 'N') {
            if (hasDrawnCard) {
                cout << "Discarded ";
                printCard(currentCard);
                cout << endl;
                hasDrawnCard = false;
                currentCard.rank = 0;
            }
            drawFromStock();
        } else if (command == 'p' || command == 'P') {
            int r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            makeMove(r1, c1, r2, c2);
        } else if (command == 'k' || command == 'K') {
            int r, c;
            cin >> r >> c;
            removeKing(r, c);
        } else if (command == 's' || command == 'S') {
            if (!hasDrawnCard) {
                cout << "No card drawn from stock!" << endl;
            } else if (currentCard.rank == 13) {
                cout << "King removed!" << endl;
                hasDrawnCard = false;
                currentCard.rank = 0;
            } else {
                cout << "Current stock card is not a King!" << endl;
            }
        } else if (command == 'm' || command == 'M') {
            if (!hasDrawnCard) {
                cout << "No card drawn from stock!" << endl;
            } else {
                int r, c;
                cin >> r >> c;
                --r;
                --c;
                if (r < 0 || r >= 7 || c < 0 || c >= 7) {
                    cout << "Invalid position!" << endl;
                } else if (!isExposed(r, c)) {
                    cout << "Card is not exposed!" << endl;
                } else if (board[r][c].rank + currentCard.rank == 13) {
                    cout << "Match! Removing cards." << endl;
                    board[r][c].rank = 0;
                    hasDrawnCard = false;
                    currentCard.rank = 0;
                } else {
                    cout << "Cards don't add up to 13!" << endl;
                }
            }
        } else {
            cout << "Invalid command!" << endl;
        }
    }
}
