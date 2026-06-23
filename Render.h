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

void iniciarPartida(int dificuldade, std::string nomeJogador) {
    Jogador player;
    inicializarJogador(&player, 1, 1);

    int faseAtual = 0;
    bool jogando = true;
    bool zerouOJogo = false;
    bool habilidadeEnsinada = false;
    
    clock_t tempoInicioGlobal = clock();

    system("cls");

    while(faseAtual < TOTAL_MAPAS && jogando && player.status.vidaAtual > 0) {

        resetarMapa(faseAtual);
        player.posX = spawnX;
        player.posY = spawnY;
        mapa[spawnY][spawnX] = '.';

        for(int i = 0; i < 20; i++) inimigos[i].ativo = 0;

        espalharChavesEstrategicas(faseAtual);
        
        if(faseAtual == 0) mapa[spawnY][spawnX + 2] = 'G';
        if(faseAtual == 1) mapa[5][10] = 'L'; 
        if(faseAtual == 2) mapa[12][7] = 'N';
        if(faseAtual == 3) mapa[8][15] = 'F';

        int qtdPocoes = (dificuldade == 0) ? 3 : (dificuldade == 1) ? 2 : 1;
        int chanceItens = (dificuldade == 0) ? 70 : (dificuldade == 1) ? 40 : 15;
        int qtdArmadilhas = (dificuldade == 0) ? 2 : (dificuldade == 1) ? 3 : 5;

        espalharPeloMapa('O', 8);
        espalharPeloMapa('^', qtdArmadilhas);
        espalharPeloMapa('P', qtdPocoes);
        espalharPeloMapa('D', 1); 
        espalharPeloMapa('A', 1); 
        espalharPeloMapa('V', 1); 
        if (rand() % 100 < chanceItens) espalharPeloMapa('E', 1); 
        if (rand() % 100 < chanceItens) espalharPeloMapa('M', 1); 

        if(faseAtual == TOTAL_MAPAS - 1) {
            totalInimigos = 1; 
            int rx, ry;
            bool posOk = false;
            while(!posOk) {
                rx = rand() % MAP_COLUNAS;
                ry = rand() % MAP_LINHAS;
                if(mapa[ry][rx] == '.') {
                    mapa[ry][rx] = 'B';
                    inicializarBoss(&inimigos[0], rx, ry, faseAtual, dificuldade);
                    posOk = true;
                }
            }
        } else {
            totalInimigos = 4 + (faseAtual * 2);
            for(int i = 0; i < totalInimigos; i++) {
                int rx, ry;
                bool posOk = false;
                while(!posOk) {
                    rx = rand() % MAP_COLUNAS;
                    ry = rand() % MAP_LINHAS;
                    if(mapa[ry][rx] == '.') {
                        mapa[ry][rx] = 'I';
                        int tipoInimigo = (i % 3 == 0) ? 2 : 1;
                        inicializarInimigo(&inimigos[i], rx, ry, tipoInimigo, faseAtual, dificuldade);
                        posOk = true;
                    }
                }
            }
        }

        bool faseConcluida = false;
        char tecla;
        bool telaPrecisaAtualizar = true;
        clock_t tempoUltimoMovimento = clock();
        
        int ultimoTempoDesenhado = -1; 

        system("cls");

        while(!faseConcluida && jogando && player.status.vidaAtual > 0) {
            
            int tempoDecorrido = (clock() - tempoInicioGlobal) / CLOCKS_PER_SEC;

            if (tempoDecorrido != ultimoTempoDesenhado) {
                telaPrecisaAtualizar = true;
                ultimoTempoDesenhado = tempoDecorrido;
            }

            if (telaPrecisaAtualizar) {
                atualizarFogOfWar(&player);
                desenharTela(&player, faseAtual, tempoDecorrido); 
                telaPrecisaAtualizar = false;
            }

            if (clock() >= tempoUltimoMovimento + 1500) {
                moverInimigos(&player);
                tempoUltimoMovimento = clock();
                telaPrecisaAtualizar = true;
            }

            static clock_t ultimoMovimentoJogador = 0;

            if (_kbhit()) {
                if (clock() < ultimoMovimentoJogador + (200 - player.status.velocidade * 30)) {
                    _getch(); 
                    continue;
                }

                tecla = _getch();
                telaPrecisaAtualizar = true;
                
                if(tecla == 'p' || tecla == 'P') {
                    clock_t inicioPause = clock(); 
                    system("cls");
                    cout << "\n\n\n\n\n\n";
                    cout << "\t\t=======================================\n";
                    cout << "\t\t             JOGO PAUSADO              \n";
                    cout << "\t\t=======================================\n";
                    cout << "\n\t\t Pressione qualquer tecla para voltar...\n";
                    _getch(); 
                    
                    tempoInicioGlobal += (clock() - inicioPause); 
                    
                    system("cls");
                    continue;
                }

                if(tecla == 't' || tecla == 'T') {
                    jogando = false;
                    continue;
                }

                if(tecla == 'e' || tecla == 'E') {
                    processarHabilidadeEspecial(&player, faseAtual, tempoDecorrido);
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
                    processarAlvoEncontrado(&player, alvo, proxX, proxY, faseAtual, faseConcluida, habilidadeEnsinada, tempoDecorrido);
                    ultimoMovimentoJogador = clock();
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

    int tempoFinal = (clock() - tempoInicioGlobal) / CLOCKS_PER_SEC;

    if (zerouOJogo) {
        int bonusDeTempo = 600 - tempoFinal; 
        if(bonusDeTempo > 0) {
            player.pontuacao += bonusDeTempo;
        }
    }

    float multDif = (dificuldade == 0) ? 0.8 : (dificuldade == 1) ? 1.0 : 1.5;
    player.pontuacao = (int)(player.pontuacao * multDif);

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

    cout << "\nSua pontuacao final: " << player.pontuacao << " pontos\n";
    cout << "Seu tempo: " << tempoFinal << " segundos\n";
    cout << "=======================================\n";
    cout << "Pressione qualquer tecla para retornar ao menu...";
    _getch();
    
    // Atualiza o placar passando o nome que pegamos la no inicio
    atualizarPlacar(player.pontuacao, tempoFinal, nomeJogador);
}

#endif