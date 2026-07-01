#ifndef COMBATE_H
#define COMBATE_H

void sistemaBatalha(Jogador *j, int inimigoIndex) {
    Inimigo *ini = &inimigos[inimigoIndex];
    if(ini->ativo == 1) {
        if(rand() % 100 < j->status.acerto) {
            if(rand() % 100 >= ini->status.esquiva) {
                int danoCausado = j->status.danoForca - ini->status.armadura;
                if(danoCausado < 0) danoCausado = 0;
                ini->status.vidaAtual -= danoCausado;
            }
        }

        if(ini->status.vidaAtual > 0) {
            if(rand() % 100 < ini->status.acerto) {
                if(rand() % 100 >= j->status.esquiva) {
                    int danoSofrido = ini->status.danoForca - j->status.armadura;
                    if(danoSofrido < 0) danoSofrido = 0;
                    j->status.vidaAtual -= danoSofrido;
                }
            }
        } else {
            ini->ativo = 0;
            mapa[ini->posY][ini->posX] = '.';
            j->pontuacao += 50;
            j->status.experienciaAtual += 5;

            if(j->status.experienciaAtual >= j->status.experienciaProximoNivel) {
                j->status.nivel++;
                j->status.vidaMaxima += 5;
                j->status.vidaAtual = j->status.vidaMaxima;
                j->status.danoForca += 2;
                j->cargasHabilidade += 2; 
                j->status.experienciaProximoNivel *= 2;
            }
        }
    }
}

void moverInimigos(Jogador *j) {
    for(int i = 0; i < totalInimigos; i++) {
        if(inimigos[i].ativo == 1) {
            int ex = inimigos[i].posX;
            int ey = inimigos[i].posY;
            int nx = ex;
            int ny = ey;

            if(inimigos[i].tipo == 1) {
                int dir = rand() % 4;
                if(dir == 0) ny--;
                else if(dir == 1) ny++;
                else if(dir == 2) nx--;
                else if(dir == 3) nx++;
            } else if(inimigos[i].tipo == 2) {
                if(j->posX > ex) nx++;
                else if(j->posX < ex) nx--;
                else if(j->posY > ey) ny++;
                else if(j->posY < ey) ny--;
            }

            if (nx == j->posX && ny == j->posY) {
                sistemaBatalha(j, i);
            } else {
                char alvo = mapa[ny][nx];
                if(alvo == '.' || alvo == 'O' || alvo == 'P' || alvo == 'D' || alvo == 'A' || alvo == 'M' || alvo == 'E' || alvo == 'V') {
                    mapa[ey][ex] = '.';
                    mapa[ny][nx] = inimigos[i].boss ? 'B' : 'I';
                    inimigos[i].posX = nx;
                    inimigos[i].posY = ny;
                }
            }
        }
    }
}
#endif