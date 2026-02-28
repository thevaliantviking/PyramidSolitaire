#include "Pyramid.h"
#include <iostream>
using namespace std;

Pyramid::Pyramid() {
    stockPile = nullptr;
    discardPile = nullptr;
    hasDrawnCard = false;
    stockSize = 0;
    discardSize = 0;
    clearBoard();

    //open the logfile
    logFile.open("csc375Solitaire.txt");
    if (logFile.is_open()) {
        logFile << "---- Pyramid Solitaire Game Started ----" << endl;
    }
}

Pyramid::~Pyramid() {
    if (logFile.is_open()) {
        logFile << "---- Game Ended ----" << endl;
        logFile.close();
    }
}

void Pyramid::logAction(const string& action) {
    if (logFile.is_open()) {
        logFile << action << endl;
        logFile.flush();
    }
}

void Pyramid::mainMenu() {
    char c;
    cout << "\n\t\tWelcome to the Pyramid!" << endl;
    cout << "\tA Game of Patience (solitaire)" << endl;
    cout << "______________________________________" << endl;
    cout << "To start a new game enter 'n'" << endl;
    cout << "To print current deck enter 'd'" << endl;
    cout << "To shuffle current deck enter 's'" << endl;
    cout << "To view instuctions enter 'i'" << endl;
    cout << "To quit game enter 'q'" << endl;
    cout << "\nPlease make a selection: ";
    cin >> c;
    if (c == 'N' || c == 'n') {
        logAction("User selected: New Game");
        newGame();
    } else if (c == 'D' || c == 'd') {
        logAction("User selected: Print Deck");
        if (isEmpty()) {
            deckRefresh();
        }
        printDeck();
        mainMenu();
    } else if (c == 'S' || c == 's') {
        logAction("User selected: Shuffle");
        shuffle();
        cout << "Current deck shuffled. To view enter 'd'." << endl;
        mainMenu();
    } else if (c == 'I' || c == 'i') {
        help();
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

    //discard pile
    current = discardPile;
    while (current != nullptr) {
        CardNode* next = current->next;
        delete current;
        current = next;
    }
    discardPile = nullptr;
    discardSize = 0;

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

void Pyramid::addToDiscard(Card card) {
    CardNode* newNode = new CardNode(card);
    newNode->next = discardPile;
    discardPile = newNode;
    discardSize++;
    logAction("Card added to discard pile (Rank: " + to_string(card.rank) +
              ", Discard size: " + to_string(discardSize) + ")");
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

Card Pyramid::drawFromDiscard() {
    if (discardPile == nullptr) {
        Card empty;
        empty.rank = 0;
        empty.suit = "";
        return empty;
    }

    CardNode* temp = discardPile;
    Card card = temp->data;
    discardPile = discardPile->next;
    delete temp;
    discardSize--;
    logAction("Card drawn from discard pile (Rank: " + to_string(card.rank) +
              ", Discard remaining: " + to_string(discardSize) + ")");

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
    cout << "Discard pile: " << discardSize << " cards" << endl;
    
    if (hasDrawnCard) {
        cout << "Current card: ";
        printCard(currentCard);
        cout << endl;
    } else {
        cout << "Current card: (none - press 'd' to draw)" << endl;
    }

}

void Pyramid::newGame() {
    logAction("\n--- NEW GAME STARTED ---");
    deckRefresh();
    clearBoard();
    
    cout << "\n______________________________________________" << endl;
    cout << "\t\t  CONQUER THE PYRAMID!!!\n";
    cout << "______________________________________________\n" << endl;
    if (!isEmpty()) {
        dealPyramid();
        logAction("Pyramid dealt - 28 cards placed");
    } else {
        cout << "There are no cards in your deck!" << endl;
        logAction("Pyramid not dealt - Deck Empty");
        mainMenu();
    }
    
    //add remaining cards to stock pile
    while (!isEmpty()) {
        addToStock(dealCard());
    }

    logAction("Stock pile created with " + to_string(stockSize) + " cards");

    currentPlay();
}

bool Pyramid::isExposed(int row, int col) {
    //check if card is exposed

    // no card
    if (board[row][col].rank == 0) {
        return false;
    }

    //bottom row always exposed
    if (row == 6) {
        return true;
    }
    
    //children check for exposed card
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
        logAction("INVALID MOVE: Position out of bounds");
        return false;
    }
    
    // Check if cards exist and are exposed
    if (!isExposed(r1, c1) || !isExposed(r2, c2)) {
        cout << "One or both cards are not exposed!" << endl;
        logAction("INVALID MOVE: Card(s) not exposed");
        return false;
    }
    
    // Check if they add up to 13
    int sum = board[r1][c1].rank + board[r2][c2].rank;
    if (sum == 13) {
        cout << "Match! Removing cards." << endl;
        logAction("SUCCESSFUL MOVE: Removed pair at (" + to_string(r1+1) + "," +
                  to_string(c1+1) + ") and (" + to_string(r2+1) + "," +
                  to_string(c2+1) + ") - Sum = 13");
        board[r1][c1].rank = 0;
        board[r2][c2].rank = 0;
        return true;
    } else {
        cout << "Cards don't add up to 13 (sum = " << sum << ")." << endl;
        logAction("INVALID MOVE: Sum = " + to_string(sum) + " (not 13)");
        return false;
    }
}

bool Pyramid::removeKing(int r, int c) {
    --r;
    --c;
    if (r < 0 || r >= 7 || c < 0 || c >= 7) {
        logAction("INVALID: King removal out of bounds");
        return false;
    }
    
    if (!isExposed(r, c)) {
        cout << "Card is not exposed!" << endl;
        logAction("INVALID: King at (" + to_string(r+1) + "," + to_string(c+1) + ") not exposed");
        return false;
    }
    
    if (board[r][c].rank == 13) {
        cout << "King removed!" << endl;
        logAction("SUCCESS: King removed at (" + to_string(r+1) + "," + to_string(c+1) + ")");
        board[r][c].rank = 0;
        return true;
    } else {
        cout << "That's not a King!" << endl;
        logAction("INVALID: Card at (" + to_string(r+1) + "," + to_string(c+1) + ") is not a King");
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
    if (hasDrawnCard) {
        cout << "You already have a card drawn. Use it or Discard and draw" << endl;
        logAction("DRAW FAILED: Card already drawn");
        return;
    }

    /* Try to draw from discard pile first
    if (discardSize > 0) {
        currentCard = drawFromDiscard();
        hasDrawnCard = true;
        cout << "Drew from discard: ";
        printCard(currentCard);
        cout << endl;
        return;
    }*/

    // If discard is empty, draw from stock
    if (stockSize == 0) {
        cout << "Both stock and discard piles are empty!" << endl;
        logAction("DRAW FAILED: Stock and discard piles empty");
        return;
    }

    currentCard = drawFromStockList();
    hasDrawnCard = true;
    cout << "Drew: ";
    printCard(currentCard);
    cout << endl;
    logAction("DRAW: Card drawn from stock (Rank: " + to_string(currentCard.rank) +
              ", Stock remaining: " + to_string(stockSize) + ")");
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
            logAction("\n*** GAME WON! Pyramid cleared! ***");
            mainMenu();
            return;
        }
        
        // Lost
        if (stockSize == 0 && !hasDrawnCard && !hasValidMoves()) {
            cout << "\n*** GAME OVER ***" << endl;
            cout << "No more moves available!\n" << endl;
            logAction("\n*** GAME LOST! No more valid moves! ***");
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
                addToDiscard(currentCard);
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
                logAction("SUCCESS: King removed from stock card");
                if (discardSize > 0) {
                    currentCard = drawFromDiscard();
                    hasDrawnCard = true;
                    cout << "Drew from discard: ";
                    printCard(currentCard);
                    cout << endl;
                } else {
                    hasDrawnCard = false;
                    currentCard.rank = 0;
                }
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
                    logAction("SUCCESSFUL MATCH: Stock card (Rank: " + to_string(currentCard.rank) +
                             ") matched with board at (" + to_string(r+1) + "," + to_string(c+1) + ")");
                    board[r][c].rank = 0;
                    if (discardSize > 0) {
                        currentCard = drawFromDiscard();
                        hasDrawnCard = true;
                        cout << "Drew from discard: ";
                        printCard(currentCard);
                        cout << endl;
                    } else {
                        hasDrawnCard = false;
                        currentCard.rank = 0;
                    }
                } else {
                    cout << "Cards don't add up to 13!" << endl;
                }
            }
        } else {
            cout << "Invalid command!" << endl;
        }
    }
}

void Pyramid::help() {
    cout << "=============================================================================================================================" << endl;
    cout << "\nThe Pyramid awaits! Here is how to play:" << endl;
    cout << "\nThe game starts with a single card dealt facing up, and another card is dealt on each bottom corner of it." << endl;
    cout << "This overlapping continues until the bottom row has 7 cards. The entire structure resembles a pyramid." << endl;
    cout << "\nThe goal of the game is to eliminate the entire pyramid by making sums of 13 with the cards on the board and a stock pile." << endl;
    cout << "Aces count as one. Jacks, queens, and kings are valued 11, 12, and 13, respectively." << endl;
    cout << "Since the King is already 13, it does not need to be paired with another card." << endl;
    cout << "\nIf a card pairs with that of another on board select ‘p’ and enter the row and column for each. Example: p 7 3 6 1 <enter>" << endl;
    cout << "If there is a King (13) on the board select ‘k’ and enter the row and column.  Example: k 6 2 <enter>" << endl;
    cout << "If there are no pairs on the board and/or the current draw card has been used select ‘d’ to draw from the stockpile." << endl;
    cout << "If the drawn card pairs with on the board select ‘m’ and the row and column of the board card. Example: m 3 2 <enter>" << endl;
    cout << "If the drawn card is a King (13) select ‘s’ to discard." << endl;
    cout << "If no pairs can be made from the current draw card and/or the board select ‘n’ to discard the current card and draw a new one." << endl;
    cout << "\nClear the Pyramid and you Win!!!" << endl;
    cout << "\n=============================================================================================================================" << endl;
}
