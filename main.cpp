#include <iostream>
#include "Pyramid.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  //Set console to UTF-8
    #endif                        //for symbols
   
    cout << "Building the Pyramid...." << endl;
    
    Pyramid game;
    game.mainMenu();
    
    cout << "\nThanks for playing!" << endl;
    return 0;
}
