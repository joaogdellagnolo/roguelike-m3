#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <fstream>
#include <string>

struct Recorde {
    std::string nome;
    int pontuacao;
    int tempo;
};

extern Recorde melhoresPlacares[5];

inline void salvarPlacar() {
    std::ofstream arquivo("placar.txt");
    if(arquivo.is_open()) {
        for(int i = 0; i < 5; i++) {
            arquivo << melhoresPlacares[i].nome << " " 
                    << melhoresPlacares[i].pontuacao << " " 
                    << melhoresPlacares[i].tempo << "\n";
        }
        arquivo.close();
    }
}

inline void carregarPlacar() {
    std::ifstream arquivo("placar.txt");
    if(arquivo.is_open()) {
        for(int i = 0; i < 5; i++) {
            arquivo >> melhoresPlacares[i].nome 
                    >> melhoresPlacares[i].pontuacao 
                    >> melhoresPlacares[i].tempo;
        }
        arquivo.close();
    } else {
        for(int i = 0; i < 5; i++) {
            melhoresPlacares[i].nome = "Vazio";
            melhoresPlacares[i].pontuacao = 0;
            melhoresPlacares[i].tempo = 0;
        }
    }
}

inline void atualizarPlacar(int novaPontuacao, int novoTempo, std::string nomeJogador) {
    int posicao = -1;
    for(int i = 0; i < 5; i++) {
        if(novaPontuacao > melhoresPlacares[i].pontuacao) {
            posicao = i;
            break;
        }
    }
    if(posicao != -1) {
        for(int j = 4; j > posicao; j--) {
            melhoresPlacares[j] = melhoresPlacares[j-1];
        }
        melhoresPlacares[posicao].nome = nomeJogador;
        melhoresPlacares[posicao].pontuacao = novaPontuacao;
        melhoresPlacares[posicao].tempo = novoTempo;
        salvarPlacar();
    }
}

inline void exibirPlacar() {
    system("cls");
    std::cout << "=================================================\n";
    std::cout << "               PLACAR DE LIDERANCA               \n";
    std::cout << "=================================================\n\n";
    for(int i = 0; i < 5; i++) {
        std::cout << "  " << i + 1 << " LUGAR: " << melhoresPlacares[i].nome 
                  << " - " << melhoresPlacares[i].pontuacao << " pts"
                  << " (" << melhoresPlacares[i].tempo << "s)\n";
    }
    std::cout << "\n=================================================\n";
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

// NOVA FUNCAO: ESCOLHER QUEM VAI JOGAR
inline bool escolherModoDeJogo() {
    system("cls");
    std::cout << "=======================================\n";
    std::cout << "           QUEM VAI JOGAR?             \n";
    std::cout << "=======================================\n\n";
    std::cout << "  [ 1 ] Jogador Manual\n";
    std::cout << "  [ 2 ] Computador (Bot Inteligente)\n\n";
    std::cout << "=======================================\n";
    std::cout << "Escolha uma opcao: ";
    
    char op = _getch();
    if(op == '2') return true; // Retorna true se for o Bot
    return false; 
}

inline int escolherDificuldade() {
    system("cls");
    std::cout << "=======================================\n";
    std::cout << "        SELECIONE A DIFICULDADE        \n";
    std::cout << "=======================================\n\n";
    std::cout << "  [ 1 ] Facil\n";
    std::cout << "  [ 2 ] Normal\n";
    std::cout << "  [ 3 ] Dificil\n\n";
    std::cout << "=======================================\n";
    std::cout << "Escolha uma opcao: ";
    
    char op = _getch();
    if(op == '1') return 0;
    if(op == '3') return 2;
    return 1; 
}

inline void mostrarInstrucoes(int opcao) {
    system("cls");
    if (opcao == 2) {
        std::cout << "-- COMO FUNCIONA --\n\n";
        std::cout << "Mova-se com W, A, S, D. Aperte T para sair ou P para Pausar.\n";
        std::cout << "Aperte E para usar a STOMPADA (Ataque em Area).\n";
        std::cout << "ATENCAO: O jogo e em tempo real! Inimigos movem sozinhos.\n";
        std::cout << "Voce deve sobreviver, derrotar inimigos e achar a saida (>).\n";
    } else if (opcao == 3) {
        std::cout << "-- ITENS E OBSTACULOS --\n\n";
        std::cout << "@ = Voce | # = Parede | . = Chao Livre | + = Porta Trancada\n";
        std::cout << "I = Inimigo | ^ = Armadilha | > = Saida para proxima fase\n";
        std::cout << "O = Ouro  | C = Chave  | P = Pocao de Vida\n";
        std::cout << "D = Arma (Ganha Dano) | A = Escudo (Ganha Armadura)\n";
        std::cout << "V = Item Velocidade   | E = Item de Esquiva\n";
        std::cout << "M = Item de Mira (Aumenta a chance de Acerto)\n";
        std::cout << "L = NPC da Loja       | N = NPC da passagem secreta \n";
        std::cout << "F = NPC misterioso    | G = NPC ajudante";
    } else if (opcao == 4) {
        std::cout << "-- PONTUACAO --\n\n";
        std::cout << "Cada Ouro (O) coletado da +10 pontos.\n";
        std::cout << "Derrotar inimigos da +50 pontos e Experiencia.\n";
        std::cout << "Escapar da masmorra inteira concede +500 pontos.\n";
        std::cout << "Terminar o jogo rapido concede bonus de tempo!\n";
        std::cout << "Jogar no DIFICIL aumenta sua pontuacao final em 50%.\n";
    }
    std::cout << "\n=======================================\n";
    std::cout << "Pressione qualquer tecla para voltar...";
    _getch();
}

#endif