#ifndef RENDER_H
#define RENDER_H
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h> 

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

void sistemaBatalha(Jogador *j, int inimigoIndex) {
    Inimigo *ini = &inimigos[inimigoIndex];
    if(ini->ativo == 1) {
        int danoCausado = j->status.danoForca - ini->status.armadura;
        if(danoCausado < 0) danoCausado = 0;
        ini->status.vidaAtual -= danoCausado;
        
        if(ini->status.vidaAtual > 0) {
            int danoSofrido = ini->status.danoForca - j->status.armadura;
            if(danoSofrido < 0) danoSofrido = 0;
            j->status.vidaAtual -= danoSofrido;
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
            } else if(mapa[ny][nx] == '.') {
                mapa[ey][ex] = '.';
                mapa[ny][nx] = 'I';
                inimigos[i].posX = nx;
                inimigos[i].posY = ny;
            }
        }
    }
}

void desenharTela(Jogador *j, int faseAtual) {
    moverCursor(0, 0); 
    for(int y = 0; y < MAP_LINHAS; y++) {
        for(int x = 0; x < MAP_COLUNAS; x++) {
            if (x == j->posX && y == j->posY) {
                cout << "@"; 
            } else if (mapaExplorado[y][x]) {
                cout << mapa[y][x]; 
            } else {
                cout << " "; 
            }
        }
        cout << "\n";
    }
    
    cout << "\nFase: " << faseAtual + 1 << "/" << TOTAL_MAPAS 
         << " | Lvl: " << j->status.nivel 
         << " | Exp: " << j->status.experienciaAtual << "/" << j->status.experienciaProximoNivel << "   "
         << "\nHp: " << j->status.vidaAtual << "/" << j->status.vidaMaxima 
         << " | Str: " << j->status.danoForca 
         << " | Arm: " << j->status.armadura << "   "
         << "\nOuro: " << j->ouro << " | Chaves: " << j->chaves << " | Especial (E): " << j->cargasHabilidade << " | Pontos: " << j->pontuacao << "          \n";
    cout << "Pressione 'T' para abandonar a campanha.          \n";
}

void iniciarPartida() {
    Jogador player;
    inicializarJogador(&player, 1, 1); 
    
    int faseAtual = 0;
    bool jogando = true;
    bool zerouOJogo = false;
    
    system("cls"); 
    
    while(faseAtual < TOTAL_MAPAS && jogando && player.status.vidaAtual > 0) {
        
        resetarMapa(faseAtual); 
        player.posX = spawnX;
        player.posY = spawnY;
        mapa[spawnY][spawnX] = '.'; 
        
        for(int i = 0; i < 20; i++) inimigos[i].ativo = 0;

        espalharChavesEstrategicas(faseAtual);
        
        espalharPeloMapa('O', 5); 
        espalharPeloMapa('^', 3); 
        espalharPeloMapa('P', 2); 
        espalharPeloMapa('W', 1); 
        espalharPeloMapa('D', 1); 
        
        totalInimigos = 4 + (faseAtual * 2);
        
        for(int i = 0; i < totalInimigos; i++) {
            int rx, ry;
            bool posicaoValida = false;
            while(!posicaoValida) {
                rx = rand() % MAP_COLUNAS;
                ry = rand() % MAP_LINHAS;
                if(mapa[ry][rx] == '.') {
                    mapa[ry][rx] = 'I';
                    
                    int tipoInimigo = (i % 3 == 0) ? 2 : 1; 
                    
                    // AQUI ESTÁ A CORREÇÃO: Enviando a faseAtual para a função!
                    inicializarInimigo(&inimigos[i], rx, ry, tipoInimigo, faseAtual);
                    posicaoValida = true;
                }
            }
        }
        
        bool faseConcluida = false;
        char tecla;
        bool telaPrecisaAtualizar = true;
        clock_t tempoUltimoMovimento = clock();
        
        system("cls"); 
        
        while(!faseConcluida && jogando && player.status.vidaAtual > 0) { 
            if (telaPrecisaAtualizar) {
                atualizarFogOfWar(&player);
                desenharTela(&player, faseAtual);
                telaPrecisaAtualizar = false;
            }
            
            if (clock() >= tempoUltimoMovimento + 1500) { 
                moverInimigos(&player);
                tempoUltimoMovimento = clock();
                telaPrecisaAtualizar = true; 
            }
            
            if (_kbhit()) {
                tecla = _getch(); 
                telaPrecisaAtualizar = true; 
                
                if(tecla == 't' || tecla == 'T') {
                    jogando = false;
                    continue;
                }
                
                if(tecla == 'e' || tecla == 'E') {
                    if (player.cargasHabilidade > 0) {
                        player.cargasHabilidade--;
                        char charCima = mapa[player.posY-1][player.posX];
                        char charBaixo = mapa[player.posY+1][player.posX];
                        char charEsq = mapa[player.posY][player.posX-1];
                        char charDir = mapa[player.posY][player.posX+1];

                        if(charCima != '#') mapa[player.posY-1][player.posX] = '*';
                        if(charBaixo != '#') mapa[player.posY+1][player.posX] = '*';
                        if(charEsq != '#') mapa[player.posY][player.posX-1] = '*';
                        if(charDir != '#') mapa[player.posY][player.posX+1] = '*';

                        desenharTela(&player, faseAtual);
                        esperar(150); 

                        mapa[player.posY-1][player.posX] = charCima;
                        mapa[player.posY+1][player.posX] = charBaixo;
                        mapa[player.posY][player.posX-1] = charEsq;
                        mapa[player.posY][player.posX+1] = charDir;
                        
                        for(int i = 0; i < totalInimigos; i++) {
                            if (inimigos[i].ativo == 1) {
                                int dx = abs(inimigos[i].posX - player.posX);
                                int dy = abs(inimigos[i].posY - player.posY);
                                if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
                                    inimigos[i].status.vidaAtual -= (player.status.danoForca * 2);
                                    if (inimigos[i].status.vidaAtual <= 0) {
                                        inimigos[i].ativo = 0;
                                        mapa[inimigos[i].posY][inimigos[i].posX] = '.';
                                        player.pontuacao += 50;
                                        player.status.experienciaAtual += 5;
                                        if(player.status.experienciaAtual >= player.status.experienciaProximoNivel) {
                                            player.status.nivel++;
                                            player.status.vidaMaxima += 5;
                                            player.status.vidaAtual = player.status.vidaMaxima;
                                            player.status.danoForca += 2;
                                            player.status.experienciaProximoNivel *= 2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    continue; 
                }
                
                int proxX = player.posX;
                int proxY = player.posY;
                
                if(tecla == 'w') proxY--;
                if(tecla == 's') proxY++;
                if(tecla == 'a') proxX--;
                if(tecla == 'd') proxX++;
                
                char alvo = mapa[proxY][proxX];
                
                if(alvo != '#') { 
                    if (alvo == 'I') {
                        for(int i=0; i < totalInimigos; i++) {
                            if(inimigos[i].posX == proxX && inimigos[i].posY == proxY) {
                                sistemaBatalha(&player, i);
                                break;
                            }
                        }
                    } else if (alvo == '+') {
                        if (player.chaves > 0) {
                            player.chaves--;
                            mapa[proxY][proxX] = '.';
                            player.posX = proxX;
                            player.posY = proxY;
                        }
                    } else if (alvo == '>') {
                        faseConcluida = true; 
                        player.pontuacao += 200;
                    } else {
                        player.posX = proxX;
                        player.posY = proxY;
                        
                        if(alvo == 'O') {
                            player.ouro++;
                            player.pontuacao += 10;
                            mapa[proxY][proxX] = '.'; 
                        } else if (alvo == 'C') {
                            player.chaves++;
                            mapa[proxY][proxX] = '.';
                        } else if (alvo == 'W') {
                            player.status.danoForca += 3;
                            mapa[proxY][proxX] = '.';
                        } else if (alvo == 'D') {
                            player.status.armadura += 2;
                            mapa[proxY][proxX] = '.';
                        } else if (alvo == 'P') {
                            player.status.vidaAtual += 10;
                            if (player.status.vidaAtual > player.status.vidaMaxima) {
                                player.status.vidaAtual = player.status.vidaMaxima;
                            }
                            mapa[proxY][proxX] = '.';
                        } else if (alvo == '^') { 
                            player.status.vidaAtual -= 2;
                        }
                    }
                }
            }
        }
        
        if (faseConcluida && player.status.vidaAtual > 0) {
            faseAtual++;
            if (faseAtual < TOTAL_MAPAS) {
                system("cls");
                cout << "=======================================\n";
                cout << "   FASE CONCLUIDA! AVANCANDO DE NIVEL  \n";
                cout << "=======================================\n";
                esperar(2500); 
            } else {
                zerouOJogo = true; 
            }
        }
    } 
    
    atualizarPlacar(player.pontuacao);
    system("cls");
    cout << "=======================================\n";
    cout << "              FIM DE JOGO              \n";
    cout << "=======================================\n";
    
    if (zerouOJogo) {
        cout << "VOCE ZEROU A MASMORRA! PARABENS O HERIO!\n";
    } else if (player.status.vidaAtual <= 0) {
        cout << "Voce foi derrotado na Fase " << faseAtual + 1 << "!\n";
    } else {
        cout << "Voce abandonou a campanha.\n";
    }
    
    cout << "\nSua pontuacao final: " << player.pontuacao << " pontos!\n"; 
    cout << "=======================================\n";
    cout << "Pressione qualquer tecla para retornar ao menu...";
    _getch();
}

#endif