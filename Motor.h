#ifndef MOTOR_H
#define MOTOR_H

void iniciarPartida(int dificuldade, std::string nomeJogador, bool modoBot) {
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
        player.posX = spawnX; player.posY = spawnY; mapa[spawnY][spawnX] = '.';
        for(int i = 0; i < 20; i++) inimigos[i].ativo = 0;

        espalharChavesEstrategicas(faseAtual);
        if(faseAtual == 0) mapa[spawnY][spawnX + 2] = 'G';
        if(faseAtual == 1) mapa[5][10] = 'L'; 
        if(faseAtual == 2) mapa[12][7] = 'N';
        if(faseAtual == 3) mapa[8][15] = 'F';

        int qtdPocoes = (dificuldade == 0) ? 3 : (dificuldade == 1) ? 2 : 1;
        int chanceItens = (dificuldade == 0) ? 70 : (dificuldade == 1) ? 40 : 15;
        int qtdArmadilhas = (dificuldade == 0) ? 2 : (dificuldade == 1) ? 3 : 5;

        espalharPeloMapa('O', 8); espalharPeloMapa('^', qtdArmadilhas); espalharPeloMapa('P', qtdPocoes);
        espalharPeloMapa('D', 1); espalharPeloMapa('A', 1); espalharPeloMapa('V', 1); 
        if (rand() % 100 < chanceItens) espalharPeloMapa('E', 1); 
        if (rand() % 100 < chanceItens) espalharPeloMapa('M', 1); 

        if(faseAtual == TOTAL_MAPAS - 1) {
            totalInimigos = 1; int rx, ry; bool posOk = false;
            while(!posOk) {
                rx = rand() % MAP_COLUNAS; ry = rand() % MAP_LINHAS;
                if(mapa[ry][rx] == '.') {
                    mapa[ry][rx] = 'B'; inicializarBoss(&inimigos[0], rx, ry, faseAtual, dificuldade); posOk = true;
                }
            }
        } else {
            totalInimigos = 4 + (faseAtual * 2);
            for(int i = 0; i < totalInimigos; i++) {
                int rx, ry; bool posOk = false;
                while(!posOk) {
                    rx = rand() % MAP_COLUNAS; ry = rand() % MAP_LINHAS;
                    if(mapa[ry][rx] == '.') {
                        mapa[ry][rx] = 'I'; int tipoInimigo = (i % 3 == 0) ? 2 : 1;
                        inicializarInimigo(&inimigos[i], rx, ry, tipoInimigo, faseAtual, dificuldade); posOk = true;
                    }
                }
            }
        }

        bool faseConcluida = false; char tecla; bool telaPrecisaAtualizar = true;
        clock_t tempoUltimoMovimento = clock(); int ultimoTempoDesenhado = -1; 
        static clock_t ultimoMovimentoJogador = 0;
        system("cls");

        while(!faseConcluida && jogando && player.status.vidaAtual > 0) {
            int tempoDecorrido = (clock() - tempoInicioGlobal) / CLOCKS_PER_SEC;
            if (tempoDecorrido != ultimoTempoDesenhado) { telaPrecisaAtualizar = true; ultimoTempoDesenhado = tempoDecorrido; }
            if (telaPrecisaAtualizar) { atualizarFogOfWar(&player); desenharTela(&player, faseAtual, tempoDecorrido); telaPrecisaAtualizar = false; }
            if (clock() >= tempoUltimoMovimento + 1500) { moverInimigos(&player); tempoUltimoMovimento = clock(); telaPrecisaAtualizar = true; }

            bool executarAcao = false;

            if (modoBot) {
                if (clock() >= ultimoMovimentoJogador + 350) {
                    tecla = jogarBot(&player); 
                    if (tecla != ' ') executarAcao = true;
                    ultimoMovimentoJogador = clock();
                }
                if (_kbhit()) { char manual = _getch(); if(manual == 't' || manual == 'T') jogando = false; }
            } else { 
                if (_kbhit()) {
                    if (clock() >= ultimoMovimentoJogador + (200 - player.status.velocidade * 30)) {
                        tecla = _getch(); executarAcao = true; ultimoMovimentoJogador = clock();
                    } else { _getch(); }
                }
            }

            if (executarAcao) {
                telaPrecisaAtualizar = true;
                if(tecla == 'p' || tecla == 'P') {
                    clock_t inicioPause = clock(); system("cls");
                    cout << "\n\n\n\n\n\n\t\t=======================================\n\t\t             JOGO PAUSADO              \n\t\t=======================================\n\n\t\t Pressione qualquer tecla para voltar...\n";
                    _getch(); tempoInicioGlobal += (clock() - inicioPause); system("cls"); continue;
                }
                if(tecla == 't' || tecla == 'T') { jogando = false; continue; }
                if(tecla == 'e' || tecla == 'E') { processarHabilidadeEspecial(&player, faseAtual, tempoDecorrido); continue; }

                int proxX = player.posX; int proxY = player.posY;
                if(tecla == 'w') proxY--; if(tecla == 's') proxY++;
                if(tecla == 'a') proxX--; if(tecla == 'd') proxX++;
                char alvo = mapa[proxY][proxX];
                if(alvo != '#') processarAlvoEncontrado(&player, alvo, proxX, proxY, faseAtual, faseConcluida, habilidadeEnsinada, tempoDecorrido, modoBot);
            }
        } 
        if (faseConcluida && player.status.vidaAtual > 0) {
            faseAtual++;
            if (faseAtual < TOTAL_MAPAS) { system("cls"); cout << "=======================================\n   FASE CONCLUIDA! AVANCANDO DE NIVEL  \n=======================================\n"; esperar(1500); } 
            else { zerouOJogo = true; }
        }
    } 

    int tempoFinal = (clock() - tempoInicioGlobal) / CLOCKS_PER_SEC;
    if (zerouOJogo) { int bonusDeTempo = 600 - tempoFinal; if(bonusDeTempo > 0) player.pontuacao += bonusDeTempo; }

    float multDif = (dificuldade == 0) ? 0.8 : (dificuldade == 1) ? 1.0 : 1.5;
    player.pontuacao = (int)(player.pontuacao * multDif);

    system("cls");
    cout << "=======================================\n              FIM DE JOGO              \n=======================================\n";
    if (zerouOJogo) cout << "VOCE ZEROU A MASMORRA! PARABENS O HERIO!\n";
    else if (player.status.vidaAtual <= 0) cout << "Voce foi derrotado na Fase " << faseAtual + 1 << "!\n";
    else cout << "Voce abandonou a campanha.\n";

    cout << "\nSua pontuacao final: " << player.pontuacao << " pontos\nSeu tempo: " << tempoFinal << " segundos\n=======================================\nPressione qualquer tecla para retornar ao menu...";
    _getch();
    atualizarPlacar(player.pontuacao, tempoFinal, nomeJogador);
}
#endif