#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <limits> 

#define WIN32_LEAN_AND_MEAN

#include "Personagens.h"
#include "Mapas.h"
#include "Menu.h"
#include "Render.h"

using namespace std;

int main() {
    esconderCursorTerminal();
    srand(time(NULL)); 
    
    int opcaoMenu = 0;
    
    while (opcaoMenu != 6) {
        exibirMenu();
        cin >> opcaoMenu;
        
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }
        
        if (opcaoMenu == 1) {
            iniciarPartida();
        } else if (opcaoMenu >= 2 && opcaoMenu <= 4) {
            mostrarInstrucoes(opcaoMenu);
        } else if (opcaoMenu == 5) {
            exibirPlacar();
        }
    }
    
    system("cls");
    return 0;
}