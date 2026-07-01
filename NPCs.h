#ifndef NPCS_H
#define NPCS_H
#include <iostream>
#include <conio.h>

using namespace std;

void interagirComComerciante(Jogador *player, bool modoBot) {
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

    char op = '0'; 
    if (modoBot) {
        esperar(400);
        if(player->ouro >= 15) op = '6';
        else if(player->ouro >= 10) op = '5';
        else if(player->ouro >= 8) op = '2';
        else if(player->ouro >= 5 && player->status.vidaAtual < player->status.vidaMaxima) op = '1';
    } else { op = _getch(); }

    switch(op) {
        case '1':
            if(player->ouro >= 5) {
                player->ouro -= 5;
                player->status.vidaAtual += 10;
                if(player->status.vidaAtual > player->status.vidaMaxima) player->status.vidaAtual = player->status.vidaMaxima;
            } else { cout << "\nVoce nao possui ouro suficiente!"; if(!modoBot) _getch(); }
            break;
        case '2':
            if(player->ouro >= 8) { player->ouro -= 8; player->status.danoForca += 3; } 
            else { cout << "\nVoce nao possui ouro suficiente!"; if(!modoBot) _getch(); }
            break;
        case '3':
            if(player->ouro >= 8) { player->ouro -= 8; player->status.armadura += 2; } 
            else { cout << "\nVoce nao possui ouro suficiente!"; if(!modoBot) _getch(); }
            break;
        case '4':
            if(player->ouro >= 10) { player->ouro -= 10; player->status.esquiva += 5; } 
            else { cout << "\nVoce nao possui ouro suficiente!"; if(!modoBot) _getch(); }
            break;
        case '5':
            if(player->ouro >= 10) { 
                player->ouro -= 10; player->status.acerto += 5;
                if(player->status.acerto > 95) player->status.acerto = 95;
            } else { cout << "\nVoce nao possui ouro suficiente!"; if(!modoBot) _getch(); }
            break;
        case '6':
            if(player->ouro >= 15) { player->ouro -= 15; player->status.velocidade++; } 
            else { cout << "\nVoce nao possui ouro suficiente!"; if(!modoBot) _getch(); }
            break;
    }
    system("cls");
}

void interagirComGuia(Jogador *player, int x, int y, bool modoBot) {
    system("cls");
    cout << "=== GUIA SECRETO ===\n";
    cout << "\"Conheco um caminho oculto para a proxima fase.\"\n";
    cout << "\"Por 20 moedas, eu o revelarei.\"\n\n";
    cout << "1 - Comprar\n2 - Sair\n";

    char op = '2';
    if (modoBot) { esperar(400); if (player->ouro >= 20) op = '1'; } 
    else { op = _getch(); }
    
    if(op == '1') {
        if(player->ouro >= 20) {
            player->ouro -= 20; mapa[y][x] = '>'; 
            cout << "\nO guia desaparece apos revelar o caminho!\n\"Que a sorte esteja ao seu lado...\"";
            if(!modoBot) _getch();
        } else {
            cout << "\nVoce nao possui ouro suficiente!"; if(!modoBot) _getch();
        }
    }
    system("cls");
}

void interagirComViajante(Jogador *player, int x, int y, int faseAtual, int tempoDecorrido, bool modoBot) {
    system("cls");
    cout << "=== ESTRANHO VIAJANTE ===\n";
    cout << "\"Ola amigo, aceite minha ajuda e te colocarei fora dessas masmorras nesse instante.\"\n";
    cout << "Aceitar?\n1 - Sim\n2 - Nao\n";

    char op = '2';
    if (modoBot) { esperar(400); op = '2'; } 
    else { op = _getch(); }
    
    if(op == '1') {
        cout << "\n\"Excelente escolha...\"\n"; Sleep(1000);
        cout << "\n\"HAHAHAHAHAHA!\"\n"; Sleep(1000);
        while(player->status.vidaAtual > 1) {
            player->status.vidaAtual--;
            desenharTela(player, faseAtual, tempoDecorrido); 
            Sleep(80);
        }
        cout << "\nO falsante roubou sua forca vital!\n";
        mapa[y][x] = '.'; if(!modoBot) _getch();
    } else {
        cout << "\n\"Sua alma sera minha em breve...\"\n";
        mapa[y][x] = '.'; if(!modoBot) _getch();
    }
    system("cls");
}

void interagirComSobrevivente(Jogador *player, int x, int y, bool &habilidadeEnsinada, bool modoBot) {
    system("cls");
    if(!habilidadeEnsinada) {
        cout << "=== SOBREVIVENTE ===\n\n\"Finalmente encontrei alguem vivo...\"\n\n";
        cout << "\"Voce caiu nas Ruinas Esquecidas.\"\n\"Se quiser escapar, precisara atravessar todos os andares.\"\n\n";
        cout << "\"Aceite esta tecnica.\"\n\"Ela pode salvar sua vida.\"\n\n";
        cout << "Voce aprendeu a habilidade especial: STOMPADA!\n";
        cout << "Pressione E para utiliza-la.\n";
        player->cargasHabilidade = 3; habilidadeEnsinada = true; mapa[y][x] = '.'; 
    } else { cout << "\"Boa sorte, aventureiro.\"\n"; }
    if(!modoBot) _getch(); else esperar(400);
    system("cls");
}
#endif