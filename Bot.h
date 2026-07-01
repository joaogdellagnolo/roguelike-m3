#ifndef BOT_H
#define BOT_H
#include <queue>

struct PontoBot { int x, y; };

char proximoPassoBFS(int startX, int startY, char targetChar, bool &achouAlvo, bool aceitarArmadilha) {
    int dx[] = {0, 0, -1, 1}; 
    int dy[] = {-1, 1, 0, 0};
    bool visitado[MAP_LINHAS][MAP_COLUNAS] = {false};
    int paiX[MAP_LINHAS][MAP_COLUNAS];
    int paiY[MAP_LINHAS][MAP_COLUNAS];
    
    std::queue<PontoBot> fila;
    fila.push({startX, startY});
    visitado[startY][startX] = true;
    int alvoX = -1, alvoY = -1;
    
    while(!fila.empty()) {
        PontoBot atual = fila.front();
        fila.pop();
        if (mapa[atual.y][atual.x] == targetChar) {
            alvoX = atual.x; alvoY = atual.y;
            achouAlvo = true; break;
        }
        for(int i = 0; i < 4; i++) {
            int nx = atual.x + dx[i];
            int ny = atual.y + dy[i];
            if (nx >= 0 && nx < MAP_COLUNAS && ny >= 0 && ny < MAP_LINHAS) {
                char tile = mapa[ny][nx];
                bool passavel = (tile != '#');
                if (!aceitarArmadilha && tile == '^') passavel = false;
                if (targetChar != '+' && tile == '+') passavel = false;
                if (!visitado[ny][nx] && passavel) {
                    visitado[ny][nx] = true;
                    paiX[ny][nx] = atual.x; paiY[ny][nx] = atual.y;
                    fila.push({nx, ny});
                }
            }
        }
    }
    
    if (alvoX != -1) {
        int cx = alvoX; int cy = alvoY;
        while(paiX[cy][cx] != startX || paiY[cy][cx] != startY) {
            int px = paiX[cy][cx]; int py = paiY[cy][cx];
            cx = px; cy = py;
        }
        if (cx > startX) return 'd';
        if (cx < startX) return 'a';
        if (cy > startY) return 's';
        if (cy < startY) return 'w';
    }
    achouAlvo = false; return ' ';
}

char proximoPassoBFSMulti(int startX, int startY, bool aceitarArmadilha, Jogador *player, bool &achouAlvo) {
    int dx[] = {0, 0, -1, 1}; 
    int dy[] = {-1, 1, 0, 0};
    bool visitado[MAP_LINHAS][MAP_COLUNAS] = {false};
    int paiX[MAP_LINHAS][MAP_COLUNAS];
    int paiY[MAP_LINHAS][MAP_COLUNAS];
    
    std::queue<PontoBot> fila;
    fila.push({startX, startY});
    visitado[startY][startX] = true;
    int alvoX = -1, alvoY = -1;
    
    while(!fila.empty()) {
        PontoBot atual = fila.front();
        fila.pop();
        char tileAtual = mapa[atual.y][atual.x];
        
        if (!(atual.x == startX && atual.y == startY)) {
            bool ehAlvo = false;
            if (tileAtual == 'O' || tileAtual == 'P' || tileAtual == 'C' || 
                tileAtual == 'D' || tileAtual == 'A' || tileAtual == 'V' || 
                tileAtual == 'E' || tileAtual == 'M' || tileAtual == 'G' || 
                tileAtual == 'N' || tileAtual == 'L' || tileAtual == 'F' || 
                tileAtual == 'I' || tileAtual == 'B' || tileAtual == '>') ehAlvo = true;
            if (tileAtual == '+' && player->chaves > 0) ehAlvo = true;
            
            if (ehAlvo) {
                alvoX = atual.x; alvoY = atual.y;
                achouAlvo = true; break;
            }
        }
        
        for(int i = 0; i < 4; i++) {
            int nx = atual.x + dx[i]; int ny = atual.y + dy[i];
            if (nx >= 0 && nx < MAP_COLUNAS && ny >= 0 && ny < MAP_LINHAS) {
                char tile = mapa[ny][nx];
                bool passavel = (tile != '#');
                if (!aceitarArmadilha && tile == '^') passavel = false;
                if (tile == '+' && player->chaves == 0) passavel = false;
                if (!visitado[ny][nx] && passavel) {
                    visitado[ny][nx] = true;
                    paiX[ny][nx] = atual.x; paiY[ny][nx] = atual.y;
                    fila.push({nx, ny});
                }
            }
        }
    }
    
    if (alvoX != -1) {
        int cx = alvoX; int cy = alvoY;
        while(paiX[cy][cx] != startX || paiY[cy][cx] != startY) {
            int px = paiX[cy][cx]; int py = paiY[cy][cx];
            cx = px; cy = py;
        }
        if (cx > startX) return 'd';
        if (cx < startX) return 'a';
        if (cy > startY) return 's';
        if (cy < startY) return 'w';
    }
    achouAlvo = false; return ' ';
}

char jogarBot(Jogador *player) {
    bool achou = false;
    char mov = ' ';
    
    if (player->cargasHabilidade > 0) {
        char cCima = mapa[player->posY - 1][player->posX];
        char cBaixo = mapa[player->posY + 1][player->posX];
        char cEsq = mapa[player->posY][player->posX - 1];
        char cDir = mapa[player->posY][player->posX + 1];
        if (cCima == 'I' || cCima == 'B' || cBaixo == 'I' || cBaixo == 'B' || 
            cEsq == 'I' || cEsq == 'B' || cDir == 'I' || cDir == 'B') return 'e';
    }

    int dx[] = {0, 0, -1, 1}; 
    int dy[] = {-1, 1, 0, 0};
    char comandos[] = {'w', 's', 'a', 'd'};
    for (int i = 0; i < 4; i++) {
        int nx = player->posX + dx[i]; int ny = player->posY + dy[i];
        char tile = mapa[ny][nx];
        if (tile == 'I' || tile == 'B' || tile == 'O' || tile == 'P' || tile == 'C' || 
            tile == 'D' || tile == 'A' || tile == 'V' || tile == 'E' || tile == 'M' || 
            tile == 'G' || tile == 'N' || tile == 'L' || tile == 'F' || tile == '>') return comandos[i];
        if (tile == '+' && player->chaves > 0) return comandos[i];
    }
    
    if (player->status.vidaAtual < player->status.vidaMaxima * 0.4) {
        mov = proximoPassoBFS(player->posX, player->posY, 'P', achou, false);
        if (achou) return mov;
    }
    mov = proximoPassoBFS(player->posX, player->posY, 'G', achou, false);
    if (achou) return mov;
    
    mov = proximoPassoBFSMulti(player->posX, player->posY, false, player, achou);
    if (achou) return mov;
    mov = proximoPassoBFSMulti(player->posX, player->posY, true, player, achou);
    if (achou) return mov;
    return ' '; 
}
#endif