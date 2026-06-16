#ifndef PERSONAGENS_H
#define PERSONAGENS_H

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
    AtributosRPG status;
} Inimigo;

// O VETOR FOI EXPANDIDO PARA SUPORTAR A HORDA!
Inimigo inimigos[20]; 
int totalInimigos = 3; // Esse valor agora sera reescrito pelo Render.h a cada fase

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
}

// A FUNCAO AGORA RECEBE A 'FASE' PARA MULTIPLICAR OS ATRIBUTOS
void inicializarInimigo(Inimigo *ini, int x, int y, int tipo, int fase) {
    ini->posX = x;
    ini->posY = y;
    ini->tipo = tipo;
    ini->ativo = 1;
    
    // Matemática de Dificuldade (Level Scaling)
    int buffVida = fase * 3;
    int buffDano = fase * 2;
    
    if (tipo == 1) { // Errante (Lento mas fortinho)
        ini->status.vidaMaxima = 8 + buffVida;
        ini->status.danoForca = 3 + buffDano;
    } else { // Caçador (Rápido e agressivo)
        ini->status.vidaMaxima = 12 + buffVida;
        ini->status.danoForca = 4 + buffDano;
    }
    
    ini->status.vidaAtual = ini->status.vidaMaxima;
    ini->status.armadura = (fase > 2) ? 1 : 0; // So ganham armadura das fases 3 em diante
}

#endif