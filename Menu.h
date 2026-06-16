// Menu.h
#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <conio.h>
#include <stdlib.h>

inline int melhoresPontuacoes[5] = {0, 0, 0, 0, 0};

inline void atualizarPlacar(int novaPontuacao) {
    for(int i = 0; i < 5; i++) {
        if(novaPontuacao > melhoresPontuacoes[i]) {
            for(int j = 4; j > i; j--) {
                melhoresPontuacoes[j] = melhoresPontuacoes[j-1];
            }
            melhoresPontuacoes[i] = novaPontuacao;
            break;
        }
    }
}

inline void exibirPlacar() {
    system("cls");
    std::cout << "=======================================\n";
    std::cout << "          PLACAR DE LIDERANCA          \n";
    std::cout << "=======================================\n\n";
    for(int i = 0; i < 5; i++) {
        std::cout << "  " << i + 1 << " LUGAR: " << melhoresPontuacoes[i] << " pontos\n";
    }
    std::cout << "\n=======================================\n";
    std::cout << "Pressione qualquer tecla para voltar...";
    _getch();
}

inline void exibirMenu() {
    system("cls"); 
    std::cout << "=======================================\n";
    std::cout << "             ROGUELIKE RPG             \n";
    std::cout << "=======================================\n\n";
    std::cout << "  [ 1 ] Jogar\n";
    std::cout << "  [ 2 ] Como funciona o jogo\n";
    std::cout << "  [ 3 ] Explicacao dos itens\n";
    std::cout << "  [ 4 ] Sistema de pontuacao\n";
    std::cout << "  [ 5 ] Placar de Lideranca\n";
    std::cout << "  [ 6 ] Sair\n\n";
    std::cout << "=======================================\n";
    std::cout << "Escolha uma opcao: ";
}

inline void mostrarInstrucoes(int opcao) {
    system("cls");
    if (opcao == 2) {
        std::cout << "-- COMO FUNCIONA --\n\n";
        std::cout << "Mova-se com W, A, S, D. Aperte T para sair da partida.\n";
        std::cout << "Aperte E para usar o Giro de Batalha (Ataque em Area).\n";
        std::cout << "ATENCAO: O jogo e em tempo real! Inimigos movem sozinhos.\n";
        std::cout << "Voce deve sobreviver, derrotar inimigos e achar a saida (>).\n";
    } else if (opcao == 3) {
        std::cout << "-- ITENS E OBSTACULOS --\n\n";
        std::cout << "@ = Voce | # = Parede | . = Chao Livre | + = Porta Trancada\n";
        std::cout << "I = Inimigo | ^ = Armadilha | > = Saida para proxima fase\n";
        std::cout << "O = Ouro  | C = Chave  | P = Pocao de Vida\n";
        std::cout << "W = Arma (Ganha Forca) | D = Escudo (Ganha Armadura)\n";
    } else if (opcao == 4) {
        std::cout << "-- PONTUACAO --\n\n";
        std::cout << "Cada Ouro (O) coletado da +10 pontos.\n";
        std::cout << "Derrotar inimigos da +50 pontos e Experiencia.\n";
        std::cout << "Escapar da masmorra inteira concede +500 pontos.\n";
    }
    std::cout << "\n=======================================\n";
    std::cout << "Pressione qualquer tecla para voltar...";
    _getch();
}

#endif