#ifndef PERSONAGENS_H
#define PERSONAGENS_H
#include <windows.h>

typedef struct {
    int nivel;
    int experienciaAtual;
    int experienciaProximoNivel;
    int vidaAtual;
    int vidaMaxima;
    int danoForca;
    int armadura;
    int acerto;
    int esquiva;
    int velocidade;
} AtributosRPG;

typedef struct {
    int posX;
    int posY;
    int ouro;
    int chaves;
    int pocoesVida;
    int cargasHabilidade;
    int pontuacao;
    AtributosRPG status;
} Jogador;

typedef struct {
    int id;
    int tipo;
    int posX;
    int posY;
    int ativo;
    bool boss;
    AtributosRPG status;
} Inimigo;

Inimigo inimigos[20];
int totalInimigos = 3;

void inicializarJogador(Jogador *j, int startX, int startY) {
    j->posX = startX;
    j->posY = startY;
    j->ouro = 0;
    j->chaves = 0;
    j->pocoesVida = 0;
    j->cargasHabilidade = 3;
    j->pontuacao = 0;

    j->status.nivel = 1;
    j->status.experienciaAtual = 0;
    j->status.experienciaProximoNivel = 10;
    j->status.vidaMaxima = 20;
    j->status.vidaAtual = 20;
    j->status.danoForca = 4;
    j->status.armadura = 0;
    j->status.acerto = 80;
    j->status.esquiva = 10;
    j->status.velocidade = 1;
}

// Matematica alterada para incluir a Dificuldade
void inicializarInimigo(Inimigo *ini, int x, int y, int tipo, int fase, int dificuldade) {
    ini->posX = x;
    ini->posY = y;
    ini->tipo = tipo;
    ini->ativo = 1;
    ini->boss = false;

    // Multiplicadores baseados na dificuldade selecionada (0=Facil, 1=Normal, 2=Dificil)
    int multiplicadorVida = (dificuldade == 0) ? 2 : (dificuldade == 1) ? 3 : 5;
    int multiplicadorDano = (dificuldade == 0) ? 1 : (dificuldade == 1) ? 2 : 3;

    int buffVida = fase * multiplicadorVida;
    int buffDano = fase * multiplicadorDano;

    if (tipo == 1) { 
        ini->status.vidaMaxima = 8 + buffVida;
        ini->status.danoForca = 3 + buffDano;
    } else { 
        ini->status.vidaMaxima = 12 + buffVida;
        ini->status.danoForca = 4 + buffDano;
    }

    ini->status.vidaAtual = ini->status.vidaMaxima;
    ini->status.armadura = (fase > 2) ? 1 : 0; 
    ini->status.acerto = 70 + fase * 2;
    ini->status.esquiva = 5 + fase;
    ini->status.velocidade = 1;
}

void inicializarBoss(Inimigo *boss, int x, int y, int fase, int dificuldade) {
    boss->posX = x;
    boss->posY = y;
    boss->tipo = 2;
    boss->ativo = 1;
    boss->boss = true;

    int multiplicadorVida = (dificuldade == 0) ? 8 : (dificuldade == 1) ? 10 : 15;
    int multiplicadorDano = (dificuldade == 0) ? 2 : (dificuldade == 1) ? 3 : 5;

    boss->status.vidaMaxima = 60 + fase * multiplicadorVida;
    boss->status.vidaAtual = boss->status.vidaMaxima;
    boss->status.danoForca = 10 + fase * multiplicadorDano;
    boss->status.armadura = 5;
    boss->status.acerto = 90;
    boss->status.esquiva = 15;
    boss->status.velocidade = 1;
}

void cor(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

#endif