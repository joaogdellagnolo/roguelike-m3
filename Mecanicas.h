#ifndef MECANICAS_H
#define MECANICAS_H
#include <math.h>

void processarHabilidadeEspecial(Jogador *player, int faseAtual, int tempoDecorrido) {
    if (player->cargasHabilidade > 0) {
        player->cargasHabilidade--;
        char charCima = mapa[player->posY-1][player->posX];
        char charBaixo = mapa[player->posY+1][player->posX];
        char charEsq = mapa[player->posY][player->posX-1];
        char charDir = mapa[player->posY][player->posX+1];

        if(charCima != '#') mapa[player->posY-1][player->posX] = '*';
        if(charBaixo != '#') mapa[player->posY+1][player->posX] = '*';
        if(charEsq != '#') mapa[player->posY][player->posX-1] = '*';
        if(charDir != '#') mapa[player->posY][player->posX+1] = '*';

        desenharTela(player, faseAtual, tempoDecorrido);
        esperar(150); 

        mapa[player->posY-1][player->posX] = charCima;
        mapa[player->posY+1][player->posX] = charBaixo;
        mapa[player->posY][player->posX-1] = charEsq;
        mapa[player->posY][player->posX+1] = charDir;

        for(int i = 0; i < totalInimigos; i++) {
            if (inimigos[i].ativo == 1) {
                int dx = abs(inimigos[i].posX - player->posX);
                int dy = abs(inimigos[i].posY - player->posY);
                if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
                    inimigos[i].status.vidaAtual -= (player->status.danoForca * 2);
                    if (inimigos[i].status.vidaAtual <= 0) {
                        inimigos[i].ativo = 0;
                        mapa[inimigos[i].posY][inimigos[i].posX] = '.';
                        player->pontuacao += 50; player->status.experienciaAtual += 5;
                        if(player->status.experienciaAtual >= player->status.experienciaProximoNivel) {
                            player->status.nivel++; player->status.vidaMaxima += 5;
                            player->status.vidaAtual = player->status.vidaMaxima;
                            player->status.danoForca += 2; player->cargasHabilidade += 2;
                            player->status.experienciaProximoNivel *= 2;
                        }
                    }
                }
            }
        }
    }
}

void processarAlvoEncontrado(Jogador *player, char alvo, int proxX, int proxY, int faseAtual, bool &faseConcluida, bool &habilidadeEnsinada, int tempoDecorrido, bool modoBot) {
    if (alvo == 'I' || alvo == 'B') {
        for(int i=0; i < totalInimigos; i++) {
            if(inimigos[i].posX == proxX && inimigos[i].posY == proxY) {
                sistemaBatalha(player, i); break;
            }
        }
    } else if (alvo == '+') {
        if (player->chaves > 0) {
            player->chaves--; mapa[proxY][proxX] = '.';
            player->posX = proxX; player->posY = proxY;
        }
    } else if (alvo == '>') {
        faseConcluida = true; player->pontuacao += 200;
    } else {
        player->posX = proxX; player->posY = proxY;
        if(alvo == 'O') { player->ouro++; player->pontuacao += 10; mapa[proxY][proxX] = '.'; } 
        else if (alvo == 'C') { player->chaves++; mapa[proxY][proxX] = '.'; } 
        else if (alvo == 'D') { player->status.danoForca += 3; mapa[proxY][proxX] = '.'; } 
        else if (alvo == 'A') { player->status.armadura += 2; mapa[proxY][proxX] = '.'; } 
        else if (alvo == 'P') {
            player->status.vidaAtual += 10;
            if (player->status.vidaAtual > player->status.vidaMaxima) player->status.vidaAtual = player->status.vidaMaxima;
            mapa[proxY][proxX] = '.';
        } 
        else if (alvo == 'V') { player->status.velocidade++; mapa[proxY][proxX] = '.'; } 
        else if (alvo == 'E') { player->status.esquiva += 5; mapa[proxY][proxX] = '.'; } 
        else if (alvo == 'M') { 
            player->status.acerto += 5;
            if (player->status.acerto > 95) player->status.acerto = 95;
            mapa[proxY][proxX] = '.';
        } 
        else if (alvo == '^') { player->status.vidaAtual -= 2; } 
        else if(alvo == 'L') {  
            interagirComComerciante(player, modoBot);
            if (modoBot) mapa[proxY][proxX] = '.'; 
        } 
        else if(alvo == 'N') {
            interagirComGuia(player, proxX, proxY, modoBot);
            if (modoBot) mapa[proxY][proxX] = '.';
        } 
        else if(alvo == 'F') { interagirComViajante(player, proxX, proxY, faseAtual, tempoDecorrido, modoBot); } 
        else if(alvo == 'G') { interagirComSobrevivente(player, proxX, proxY, habilidadeEnsinada, modoBot); }
    }
}
#endif