#ifndef MAPAS_H
#define MAPAS_H
#include <stdlib.h>

const int MAP_LINHAS = 15;
const int MAP_COLUNAS = 40;

char mapa[MAP_LINHAS][MAP_COLUNAS + 1];
bool mapaExplorado[MAP_LINHAS][MAP_COLUNAS];

int spawnX = 1;
int spawnY = 1;

// Os seus 5 mapas customizados
const int TOTAL_MAPAS = 5;
const char mapasBase[TOTAL_MAPAS][MAP_LINHAS][MAP_COLUNAS + 1] = {
    // FASE 1: Os Três Salões
    {
        "########################################",
        "#@...#....#............#...............#",
        "#....#....#............#...............#",
        "#....#....+............#...............#",
        "#....#....#............#...............#",
        "#....#....#............#...............#",
        "#....#....############.#...............#",
        "#....#....#............#...............#",
        "#.........#............#...............#",
        "#.........#............#...............#",
        "#.........#............#...............#",
        "#.........#............+...............#",
        "#.........#............#..............>#",
        "#.........#............#...............#",
        "########################################"
    },
    // FASE 2: Quatro Quadrantes
    {
        "########################################",
        "#@......#.........#....................#",
        "#.......#.........#....................#",
        "#.......#.........+....................#",
        "#.......#.........################.....#",
        "#.......#######...#....................#",
        "#.................#....................#",
        "##.#####################################",
        "#.................+....................#",
        "#.................#....................#",
        "#.................#....................#",
        "#.................#....................#",
        "#.................#....................#",
        "#.................#...................>#",
        "########################################"
    },
    // FASE 3: A Primeira Arena (Futuro Boss 1)
    {
        "########################################",
        "#@.....................................#",
        "#......................................#",
        "#......................................#",
        "########+###############################",
        "#......................................#",
        "#......................................#",
        "#......................................#",
        "#......................................#",
        "#......................................#",
        "#....#######.......................#####",
        "#....#.............................#...#",
        "#....#.............................#...#",
        "#....#.............................+..>#",
        "########################################"
    },
    // FASE 4: O Fosso Estreito
    {
        "########################################",
        "#@.............#.......#...............#",
        "#..............#.......#...............#",
        "#..............+.......#...............#",
        "################.......###########.#####",
        "#......................................#",
        "#......................................#",
        "################.......###########+#####",
        "#..............#.......#...............#",
        "#......................#...............#",
        "################...######.##############",
        "#..................#...................#",
        "#..................#...................#",
        "#..................#..................>#",
        "########################################"
    },
    // FASE 5: A Sala do Trono (Futuro Boss 2)
    {
        "########################################",
        "#@.....................................#",
        "#........##...................##.......#",
        "#........##...................##.......#",
        "#......................................#",
        "#......................................#",
        "#......................................#",
        "#......................................#",
        "#......................................#",
        "#......................................#",
        "#......................................#",
        "#........##.....##.....##.....##########",
        "#........##.....###...###.....#........#",
        "#...............#########.....+.......>#",
        "########################################"
    }
};

void resetarMapa(int idMapa) {
    for(int y = 0; y < MAP_LINHAS; y++) {
        for(int x = 0; x < MAP_COLUNAS; x++) {
            char celula = mapasBase[idMapa][y][x];
            if (celula == '@') {
                spawnX = x;
                spawnY = y;
                mapa[y][x] = '.';
            } else {
                mapa[y][x] = celula;
            }
            // TRUE PARA VISUALIZAR, FALSE PARA A JOGATINA REAL COM NEVOA DE GUERRA
            mapaExplorado[y][x] = false; 
        }
        mapa[y][MAP_COLUNAS] = '\0';
    }
}

// Funcao que forca o nascimento de itens em setores seguros
void espalharEmZona(int minX, int minY, int maxX, int maxY, char elemento) {
    bool colocado = false;
    int tentativas = 0;
    while (!colocado && tentativas < 200) {
        int rx = rand() % (maxX - minX + 1) + minX;
        int ry = rand() % (maxY - minY + 1) + minY;
        if (mapa[ry][rx] == '.' && mapa[ry][rx] != '>') {
            mapa[ry][rx] = elemento;
            colocado = true;
        }
        // Incrementa tentativa para impedir loops infinitos caso a area esteja cheia
        tentativas++;
    }
}

// Caixas Delimitadoras (Bounding Boxes) atualizadas para os 5 mapas
void espalharChavesEstrategicas(int idMapa) {
    if (idMapa == 0) { 
        // Fase 1: Duas portas
        espalharEmZona(1, 1, 9, 13, 'C');   // Chave para a 1a porta
        espalharEmZona(11, 1, 21, 13, 'C'); // Chave para a 2a porta
    } 
    else if (idMapa == 1) { 
        // Fase 2: Duas portas
        espalharEmZona(1, 1, 16, 6, 'C');   // Chave no lado de cima
        espalharEmZona(1, 8, 16, 13, 'C');  // Chave no lado de baixo
    } 
    else if (idMapa == 2) { 
        // Fase 3 (Arena 1): Duas portas
        espalharEmZona(1, 1, 38, 3, 'C');   // Chave na primeira area de cima
        espalharEmZona(1, 5, 38, 9, 'C');   // Chave no meio da arena principal
    }
    else if (idMapa == 3) { 
        // Fase 4: Duas portas
        espalharEmZona(1, 1, 14, 3, 'C');   // Chave no primeiro cercado superior esquerdo
        espalharEmZona(1, 5, 33, 6, 'C');   // Chave no corredor horizontal do meio
    }
    else if (idMapa == 4) { 
        // Fase 5 (Arena Final): Uma porta protegendo a saida
        espalharEmZona(1, 1, 38, 10, 'C');  // Chave espalhada pela arena inteira
    }
}

void espalharPeloMapa(char elemento, int quantidade) {
    int colocados = 0;
    while(colocados < quantidade) {
        int rx = rand() % MAP_COLUNAS;
        int ry = rand() % MAP_LINHAS;
        if(mapa[ry][rx] == '.' && mapa[ry][rx] != '>') {
            mapa[ry][rx] = elemento;
            colocados++;
        }
    }
}

#endif