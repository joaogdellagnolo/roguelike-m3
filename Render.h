#ifndef RENDER_H
#define RENDER_H
#include <iostream>
#include <windows.h>

using namespace std;

void esperar(int milissegundos) {
    clock_t tempo_inicial = clock();
    while (clock() < tempo_inicial + milissegundos) {}
}

void moverCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void esconderCursorTerminal() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void atualizarFogOfWar(Jogador *j) {
    for(int y = j->posY - 2; y <= j->posY + 2; y++) {
        for(int x = j->posX - 2; x <= j->posX + 2; x++) {
            if(y >= 0 && y < MAP_LINHAS && x >= 0 && x < MAP_COLUNAS) {
                mapaExplorado[y][x] = true;
            }
        }
    }
}

void desenharTela(Jogador *j, int faseAtual, int tempoDecorrido) {
    moverCursor(0, 0);
    for(int y = 0; y < MAP_LINHAS; y++) {
        for(int x = 0; x < MAP_COLUNAS; x++) {
            if (x == j->posX && y == j->posY) {
                if (j->status.vidaAtual <= j->status.vidaMaxima * 0.3) cor(12); 
                else if (j->status.armadura >= 4) cor(9); 
                else if (j->status.danoForca >= 10) cor(14); 
                else cor(10); 
                
                cout << "@";
                cor(15);
            } else if (mapaExplorado[y][x]) {
                switch(mapa[y][x]) {
                    case '#': cor(8); break;    
                    case '.': cor(7); break;    
                    case 'I': cor(12); break;   
                    case 'B': cor(13); break;   
                    case 'O': cor(14); break;   
                    case 'C': cor(11); break;   
                    case 'P': cor(10); break;   
                    case 'D': cor(6); break;   
                    case 'A': cor(6); break;   
                    case 'M': cor(6); break;  
                    case 'E': cor(6); break;  
                    case 'V': cor(6); break;  
                    case 'L': cor(11); break;  
                    case 'N': cor(11); break;   
                    case '>': cor(10); break;   
                    case '^': cor(4); break;    
                    default: cor(15); break;
                }
                cout << mapa[y][x];
                cor(15);
            } else {
                cout << " ";
            }
        }
        cout << "\n";
    }

    cout << "\n========================================================================\n";
    cout << " [ FASE: " << faseAtual + 1 << "/" << TOTAL_MAPAS << " ]";
    cout << "   TEMPO: " << tempoDecorrido << "s";
    cout << "   PONTOS: " << j->pontuacao << "\n";
    cout << "------------------------------------------------------------------------\n";
    cout << " Lvl: " << j->status.nivel << " (" << j->status.experienciaAtual << "/" << j->status.experienciaProximoNivel << ")";
    cout << " | HP: " << j->status.vidaAtual << "/" << j->status.vidaMaxima;
    cout << " | Str: " << j->status.danoForca;
    cout << " | Arm: " << j->status.armadura;
    cout << " | Acc: " << j->status.acerto << "%";
    cout << " | Eva: " << j->status.esquiva << "%";
    cout << " | Vel: " << j->status.velocidade << "\n";
    cout << "------------------------------------------------------------------------\n";
    cout << " INVENTARIO -> Ouro: " << j->ouro << " | Chaves: " << j->chaves << " | Especial (E): " << j->cargasHabilidade << "    \n";
    cout << "========================================================================\n";
    cout << " [W/A/S/D] Mover | [E] Especial | [P] Pausar | [T] Abandonar            \n";
}
#endif