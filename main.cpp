#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include <string>
#include <windows.h>
#include <mmsystem.h> // Biblioteca para tocar som em segundo plano

#define WIN32_LEAN_AND_MEAN

#include "Personagens.h"
#include "Mapas.h"
#include "Menu.h"
#include "Mecanicas.h"
#include "Render.h"

using namespace std;

Recorde melhoresPlacares[5] = {
    {"Vazio", 0, 0}, 
    {"Vazio", 0, 0}, 
    {"Vazio", 0, 0}, 
    {"Vazio", 0, 0}, 
    {"Vazio", 0, 0}
};

int main() {
    esconderCursorTerminal();
    srand(time(NULL));
    
    carregarPlacar();

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
            int dificuldadeEscolhida = escolherDificuldade();
            
            system("cls");
            std::cout << "=======================================\n";
            std::cout << "          REGISTRO DA GUILDA           \n";
            std::cout << "=======================================\n\n";
            std::cout << "Digite seu nome de jogador (sem espacos): ";
            std::string nomeJogador;
            std::cin >> nomeJogador;
            
            system("cls");
            cout << "=====================================================\n";
            cout << "                  AS RUINAS ESQUECIDAS               \n";
            cout << "=====================================================\n\n";

            cout << "Voce nao se lembra exatamente como chegou aqui.\n";
            cout << "Em meio a uma expedicao, o chao cedeu sob seus pes\n";
            cout << "e voce despencou nas profundezas de antigas ruinas.\n\n";

            cout << "Os corredores estao infestados por criaturas,\n";
            cout << "armadilhas e aventureiros de moral duvidosa.\n";
            cout << "Alguns podem ajuda-lo... outros desejam apenas\n";
            cout << "se aproveitar do seu desespero.\n\n";

            cout << "Seu objetivo e simples:\n";
            cout << "encontrar uma maneira de escapar vivo.\n\n";

            cout << "Mas cuidado...\n";
            cout << "Nestas ruinas, nem todo atalho leva a salvacao.\n\n";

            cout << "Pressione qualquer tecla para iniciar sua jornada...";
            _getch();
            
            // Inicia a musica de fundo em loop
            PlaySound(TEXT("musica.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            
            iniciarPartida(dificuldadeEscolhida, nomeJogador);
            
            // Para a musica quando voltar para o menu
            PlaySound(NULL, 0, 0);

        } else if (opcaoMenu >= 2 && opcaoMenu <= 4) {
            mostrarInstrucoes(opcaoMenu);
        } else if (opcaoMenu == 5) {
            exibirPlacar();
        }
    }

    system("cls");
    return 0;
}


//g++ main.cpp -o roguelike.exe -lwinmm