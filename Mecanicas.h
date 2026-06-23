#ifndef MECANICAS_H
#define MECANICAS_H
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <math.h>

using namespace std;

void desenharTela(Jogador *j, int faseAtual, int tempoDecorrido);
void sistemaBatalha(Jogador *j, int inimigoIndex);
void esperar(int milissegundos);

void interagirComComerciante(Jogador *player) {
    system("cls");
    cout << "=== LOJA DO COMERCIANTE ===\n";
    cout << "Ouro: " << player->ouro << "\n\n";
    cout << "1 - Vida (+10) .......... 5 ouro\n";
    cout << "2 - Dano (+3) ........... 8 ouro\n";
    cout << "3 - Armadura (+2) ....... 8 ouro\n";
    cout << "4 - Esquiva (+5) ........ 10 ouro\n";
    cout << "5 - Mira/Acerto (+5) .... 10 ouro\n";
    cout << "6 - Velocidade (+1) ..... 15 ouro\n";
    cout << "0 - Sair\n";

    char op = _getch();

    switch(op) {
        case '1':
            if(player->ouro >= 5) {
                player->ouro -= 5;
                player->status.vidaAtual += 10;
                if(player->status.vidaAtual > player->status.vidaMaxima)
                    player->status.vidaAtual = player->status.vidaMaxima;
            } else { cout << "\nVoce nao possui ouro suficiente!"; _getch(); }
            break;
        case '2':
            if(player->ouro >= 8) { player->ouro -= 8; player->status.danoForca += 3; } 
            else { cout << "\nVoce nao possui ouro suficiente!"; _getch(); }
            break;
        case '3':
            if(player->ouro >= 8) { player->ouro -= 8; player->status.armadura += 2; } 
            else { cout << "\nVoce nao possui ouro suficiente!"; _getch(); }
            break;
        case '4':
            if(player->ouro >= 10) { player->ouro -= 10; player->status.esquiva += 5; } 
            else { cout << "\nVoce nao possui ouro suficiente!"; _getch(); }
            break;
        case '5':
            if(player->ouro >= 10) { 
                player->ouro -= 10; 
                player->status.acerto += 5;
                if(player->status.acerto > 95) player->status.acerto = 95;
            } else { cout << "\nVoce nao possui ouro suficiente!"; _getch(); }
            break;
        case '6':
            if(player->ouro >= 15) { player->ouro -= 15; player->status.velocidade++; } 
            else { cout << "\nVoce nao possui ouro suficiente!"; _getch(); }
            break;
    }
    system("cls");
}

void interagirComGuia(Jogador *player, int x, int y) {
    system("cls");
    cout << "=== GUIA SECRETO ===\n";
    cout << "\"Conheco um caminho oculto para a proxima fase.\"\n";
    cout << "\"Por 20 moedas, eu o revelarei.\"\n\n";
    cout << "1 - Comprar\n";
    cout << "2 - Sair\n";

    char op = _getch();
    if(op == '1') {
        if(player->ouro >= 20) {
            player->ouro -= 20;
            mapa[y][x] = '>'; 
            cout << "\nO guia desaparece apos revelar o caminho!";
            cout << "\n\"Que a sorte esteja ao seu lado...\"";
            _getch();
        } else {
            cout << "\nVoce nao possui ouro suficiente!";
            _getch();
        }
    }
    system("cls");
}

void interagirComViajante(Jogador *player, int x, int y, int faseAtual, int tempoDecorrido) {
    system("cls");
    cout << "=== ESTRANHO VIAJANTE ===\n";
    cout << "\"Ola amigo, aceite minha ajuda e te colocarei fora dessas masmorras nesse instante.\"\n";
    cout << "Aceitar?\n1 - Sim\n2 - Nao\n";

    char op = _getch();
    if(op == '1') {
        cout << "\n\"Excelente escolha...\"\n";
        Sleep(1000);
        cout << "\n\"HAHAHAHAHAHA!\"\n";
        Sleep(1000);

        while(player->status.vidaAtual > 1) {
            player->status.vidaAtual--;
            desenharTela(player, faseAtual, tempoDecorrido); 
            Sleep(80);
        }
        cout << "\nO falsante roubou sua forca vital!\n";
        mapa[y][x] = '.';
        _getch();
    }
    system("cls");
}

void interagirComSobrevivente(Jogador *player, int x, int y, bool &habilidadeEnsinada) {
    system("cls");
    if(!habilidadeEnsinada) {
        cout << "=== SOBREVIVENTE ===\n\n";
        cout << "\"Finalmente encontrei alguem vivo...\"\n\n";
        cout << "\"Voce caiu nas Ruinas Esquecidas.\"\n";
        cout << "\"Se quiser escapar, precisara atravessar todos os andares.\"\n\n";
        cout << "\"Aceite esta tecnica.\"\n";
        cout << "\"Ela pode salvar sua vida.\"\n\n";
        cout << "Voce aprendeu a habilidade especial: STOMPADA!\n";
        cout << "Pressione E para utiliza-la.\n";

        player->cargasHabilidade = 3;
        habilidadeEnsinada = true;
        mapa[y][x] = '.'; 
    } else {
        cout << "\"Boa sorte, aventureiro.\"\n";
    }
    _getch();
    system("cls");
}

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
                        player->pontuacao += 50;
                        player->status.experienciaAtual += 5;
                        if(player->status.experienciaAtual >= player->status.experienciaProximoNivel) {
                            player->status.nivel++;
                            player->status.vidaMaxima += 5;
                            player->status.vidaAtual = player->status.vidaMaxima;
                            player->status.danoForca += 2;
                            player->cargasHabilidade += 2;
                            player->status.experienciaProximoNivel *= 2;
                        }
                    }
                }
            }
        }
    }
}

void processarAlvoEncontrado(Jogador *player, char alvo, int proxX, int proxY, int faseAtual, bool &faseConcluida, bool &habilidadeEnsinada, int tempoDecorrido) {
    if (alvo == 'I' || alvo == 'B') {
        for(int i=0; i < totalInimigos; i++) {
            if(inimigos[i].posX == proxX && inimigos[i].posY == proxY) {
                sistemaBatalha(player, i);
                break;
            }
        }
    } else if (alvo == '+') {
        if (player->chaves > 0) {
            player->chaves--;
            mapa[proxY][proxX] = '.';
            player->posX = proxX;
            player->posY = proxY;
        }
    } else if (alvo == '>') {
        faseConcluida = true;
        player->pontuacao += 200;
    } else {
        player->posX = proxX;
        player->posY = proxY;

        if(alvo == 'O') {
            player->ouro++;
            player->pontuacao += 10;
            mapa[proxY][proxX] = '.';
        } else if (alvo == 'C') {
            player->chaves++;
            mapa[proxY][proxX] = '.';
        } else if (alvo == 'D') { 
            player->status.danoForca += 3;
            mapa[proxY][proxX] = '.';
        } else if (alvo == 'A') { 
            player->status.armadura += 2;
            mapa[proxY][proxX] = '.';
        } else if (alvo == 'P') {
            player->status.vidaAtual += 10;
            if (player->status.vidaAtual > player->status.vidaMaxima) {
                player->status.vidaAtual = player->status.vidaMaxima;
            }
            mapa[proxY][proxX] = '.';
        } else if (alvo == 'V') {
            player->status.velocidade++;
            mapa[proxY][proxX] = '.';
        } else if (alvo == 'E') {
            player->status.esquiva += 5;
            mapa[proxY][proxX] = '.';
        } else if (alvo == 'M') { 
            player->status.acerto += 5;
            if (player->status.acerto > 95) player->status.acerto = 95;
            mapa[proxY][proxX] = '.';
        } else if (alvo == '^') {
            player->status.vidaAtual -= 2;
        } else if(alvo == 'L') {  
            interagirComComerciante(player);
        } else if(alvo == 'N') {
            interagirComGuia(player, proxX, proxY);
        } else if(alvo == 'F') {
            interagirComViajante(player, proxX, proxY, faseAtual, tempoDecorrido);
        } else if(alvo == 'G') {
            interagirComSobrevivente(player, proxX, proxY, habilidadeEnsinada);
        }
    }
}

#endif