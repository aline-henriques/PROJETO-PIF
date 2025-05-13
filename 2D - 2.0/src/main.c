#include <string.h>
#include <stdlib.h>
#include <windows.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"


int pontuacao = 0;
int x_menuInicial = MAXX*0.05, y_menuInicial = MAXY*0.5;
int x_setaMenu = (MAXX*0.05)-2, y_setaMenu = MAXY*0.5;
int x_perso = MAXX*0.5, y_perso = MAXY*0.5;

typedef enum
{
    VERMELHO,
    VERDE,
    AZUL,
    AMARELO
} Cor;

typedef struct
{
    const char* linhas[3];
} Personagem;

typedef struct
{
    Cor cor;
    Personagem personagem;
} Avatar;

Personagem personagens[] = {
        { {" o ", "/|\\", "/ \\"} },
        { {" o ", "/|\\", " ^ "} },
        { {" o/", "/| ", "/ \\"} },
        { {"ooo", "ooo", "ooo"} }
    };

typedef struct
{
    int tamanho;
    int x_obj;
    int y_obj;
} Obstaculos;

typedef enum
{
    APAGAR,
    DESENHAR
} TipoAcao;

typedef enum
{
    HORIZONTAL,
    VERTICAL
} Direcao;


void printObj(Obstaculos o, TipoAcao acao)
{
    char simbolo = (acao == DESENHAR) ? 'X' : ' ';
    screenSetColor(WHITE, WHITE);

    for (int i = 0; i < o.tamanho; i++)
    {
        screenGotoxy(o.x_obj, o.y_obj + i);
        for (int j = 0; j < o.tamanho; j++)
        {
            printf("%c", simbolo);
        }
    }
}

void printGameOver(TipoAcao acao)
{
    const char* simbolo = (acao == DESENHAR) ? "💥 GAME OVER! 💥" : "              ";
    screenSetColor(WHITE, WHITE);

        screenSetColor(RED, DARKGRAY);
        screenGotoxy(MAXX * 0.60, MAXY * 0.5);
        printf("%s", simbolo);
}

Obstaculos gerarNovoObj()
{
    Obstaculos novo;
    novo.tamanho = 1 + rand() % 5;
    novo.x_obj = 1 + rand() % (MAXX - 6);
    novo.y_obj = 1 + rand() % (MAXY - 6);
    return novo;
}

void moverObsFase1(Obstaculos* obs, int maxObj)
{
    for (int i = 0; i < maxObj; i++) 
    {
        if (obs[i].tamanho > 0) 
        {  
            printObj(obs[i], APAGAR);

            obs[i].x_obj--;

            if (obs[i].x_obj <= 1)
            {
                obs[i].tamanho = 0;
            } else {
                printObj(obs[i], DESENHAR);

            }
        }
    }
}

void moverObsFase2(Obstaculos* obs, int maxObj)
{
    for (int i = 0; i < maxObj; i++) 
    {
        if (obs[i].tamanho > 0) 
        {  
            printObj(obs[i], APAGAR);

            obs[i].x_obj++;

            if (obs[i].x_obj >= MAXX - obs[i].tamanho)
            {
                obs[i].tamanho = 0;
            } else {
                printObj(obs[i], DESENHAR);

            }
        }
    }
}

void moverObsFase3(Obstaculos* obs, int maxObj)
{
    for (int i = 0; i < maxObj; i++) 
    {
        if (obs[i].tamanho > 0) 
        {  
            printObj(obs[i], APAGAR);

            obs[i].y_obj++;

            if (obs[i].y_obj >= MAXY - obs[i].tamanho)
            {
                obs[i].tamanho = 0;
            } else {
                printObj(obs[i], DESENHAR);

            }
        }
    }
}

void moverObsFase4(Obstaculos* obs, int maxObj)
{
    for (int i = 0; i < maxObj; i++) 
    {
        if (obs[i].tamanho > 0) 
        {  
            printObj(obs[i], APAGAR);

            obs[i].y_obj--;

            if (obs[i].y_obj <= MINY + 1)
            {
                obs[i].tamanho = 0;
            } else {
                printObj(obs[i], DESENHAR);

            }
        }
    }
}

void printMenuInicial(){
    const char* opcoes[] = {
        "Novo Jogo",
        "Continuar Jogo",
        "Configuração",
        "Fechar Jogo"
    };

    screenSetColor(CYAN, DARKGRAY);

    for (int i = 0; i < 4; i++){
        screenGotoxy(x_menuInicial, y_menuInicial+i);
        printf(opcoes[i]);
    }

}

void printSetaMenu(int nextY){
    const char* seta = "->";

    screenSetColor(CYAN, DARKGRAY);

    screenGotoxy(x_setaMenu, y_setaMenu);
    printf("  ");

    y_setaMenu = nextY;

    screenGotoxy(x_setaMenu, y_setaMenu);
    printf(seta);
}

void limparMenuInicial(){
    const char* opcoes[] = {
        "         ",
        "              ",
        "            ",
        "           "
    };
    const char* seta = "  ";

    for (int i = 0; i < 4; i++){
        screenGotoxy(x_menuInicial, y_menuInicial+i);
        printf(opcoes[i]);
    }

    screenGotoxy(x_setaMenu, y_setaMenu);
    printf(seta);
}

void printKey(int ch)
{
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(35, 22);
    printf("Key code :");

    screenGotoxy(34, 23);
    printf("            ");
    
    if (ch == 27) screenGotoxy(36, 23);
    else screenGotoxy(39, 23);

    printf("%d ", ch);
    while (keyhit())
    {
        printf("%d ", readch());
    }
}

void printPontuacao()
{
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(0, 0);
    printf("Pontuação: ");

    screenGotoxy(12, 0);
    printf(" ");
    screenGotoxy(12, 0);
    printf("%d ", pontuacao);
}

void printPersonagem(int nextX, int nextY)
{
    Avatar a1;
    a1.cor=VERMELHO;
    a1.personagem=personagens[0];

    screenSetColor(RED, DARKGRAY);

    for (int i = 0; i < 3; i++)
    {
        screenGotoxy(x_perso, y_perso+i);
        printf("   ");
    }

    x_perso = nextX;
    y_perso = nextY;

    for (int i = 0; i < 3; i++)
    {
        screenGotoxy(x_perso, y_perso+i);
        printf("%s",a1.personagem.linhas[i]);
    }

}

int verificarColisao(Obstaculos* obs, int maxObj)
{
    for (int i = 0; i < maxObj; i++) 
    {
        if (obs[i].tamanho > 0) 
        {
            int ox = obs[i].x_obj;
            int oy = obs[i].y_obj;
            int ot = obs[i].tamanho;

            for (int py = 0; py < 3; py++) {
                for (int px = 0; px < 3; px++) {
                    int x = x_perso + px;
                    int y = y_perso + py;

                    if (x >= ox && x < ox + ot && y >= oy && y < oy + ot)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}


int main() 
{
    static int ch = 0;
    int jogoIniciado = 0;
    int maxObj = 100;
    Obstaculos* obs = malloc(sizeof(Obstaculos) * maxObj);
    int obstaculoAtual = 0;
    int tempoEntreObstaculos = 60;
    int contadorObstaculos = 0;
    int tempoTotal = 0;
    int faseAtual = 1;
    int novaFase = 1;

    for (int i = 0; i < maxObj; i++)
    {
        obs[i].tamanho = 0;
    }

    SetConsoleOutputCP(CP_UTF8);
    screenInit(1);
    keyboardInit();
    screenHideCursor();
    screenUpdate();

    while (ch != 27) //Esc
    {
        // Handle user input
        if (jogoIniciado == 0)
        {
            printMenuInicial();
            printSetaMenu(y_setaMenu);
            if (keyhit())
            {
                ch = readch();
                if (ch == 0 || ch == 224) 
                {
                    ch = readch();
                }

                switch (ch)
                {
                    case 72:
                        if (y_setaMenu > y_menuInicial)
                        {
                            printSetaMenu(y_setaMenu-1);
                        }
                        break;
                    case 80:
                        if (y_setaMenu < y_menuInicial+3)
                        {
                            printSetaMenu(y_setaMenu+1);
                        }
                        break;
                }

                if (ch == 13)
                {
                    int opcaoSelecionada = y_setaMenu - y_menuInicial;

                    switch (opcaoSelecionada)
                    {
                        case 0:
                            limparMenuInicial();
                            jogoIniciado = 1;
                            printGameOver(APAGAR);
                            printPersonagem(MAXX*0.5, MAXY*0.5);
                            timerInit(30);
                            break;
                        case 1:
                            // Continuar
                            break;
                        case 2:
                            // Configurações
                            break;
                        case 3:
                            keyboardDestroy();
                            screenDestroy();
                            timerDestroy();
                            return 0;
                    }
                }
            }
            screenHideCursor();
            screenUpdate();
        }


        if (jogoIniciado==1)
        {
            if (keyhit())
            {
                ch = readch();
                if (ch == 0 || ch == 224)
                {
                    ch = readch();
                }

                switch (ch){

                    //cima
                    case 72:
                        if(y_perso - 1> MINY)
                        {
                            printPersonagem(x_perso, y_perso - 1);
                        }
                        break;

                    //esquerda
                    case 75:
                        if(x_perso - 1 >MINX)
                        {
                            printPersonagem(x_perso - 2, y_perso);
                        }
                        break;

                    //baixo
                    case 80:
                        if(y_perso + 3 < MAXY)
                        {
                            printPersonagem(x_perso, y_perso + 1);
                        }
                        break;

                    //direita
                    case 77:
                        if(x_perso + 4 < MAXX)
                        {
                            printPersonagem(x_perso + 2, y_perso);
                        }
                        break;
                }
            }
        }
        
        


        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1)
        {
            if (jogoIniciado == 1)
            {

                // A cada ciclo, conta o tempo
                contadorObstaculos++;
                tempoTotal++;

                //Atualização de fase
                if(obstaculoAtual>=60)novaFase = 4;
                else if (obstaculoAtual >= 40)novaFase = 3;
                else if (obstaculoAtual >= 20)novaFase = 2;

                screenGotoxy(MINX, MINY);
                printf("%d", obstaculoAtual);
                screenGotoxy(MINX, MINY+3);
                printf("%d", tempoEntreObstaculos);

                if (novaFase != faseAtual) 
                {
                    faseAtual = novaFase;
                    //Atualização de inimigos
                    switch (faseAtual) 
                    {
                        case 1: tempoEntreObstaculos = 60; break;
                        case 2: tempoEntreObstaculos = 50; break;
                        case 3: tempoEntreObstaculos = 40; timerUpdateTimer(50); break;
                        case 4: tempoEntreObstaculos = 30; timerUpdateTimer(40);break;
                    }
                }

                switch (faseAtual) {
                    //FASE1
                    case 1:
                        if (contadorObstaculos >= tempoEntreObstaculos)
                        {
                            obstaculoAtual = (obstaculoAtual + 1) % maxObj;
                            obs[obstaculoAtual] = gerarNovoObj();
                            obs[obstaculoAtual].x_obj = MAXX - obs[obstaculoAtual].tamanho - 1;

                            if (obs[obstaculoAtual].y_obj + obs[obstaculoAtual].tamanho >= MAXY) {
                                obs[obstaculoAtual].y_obj = MAXY - obs[obstaculoAtual].tamanho - 1;
                            }

                            printObj(obs[obstaculoAtual], DESENHAR);

                            contadorObstaculos = 0;                    
                            tempoEntreObstaculos = tempoEntreObstaculos > 40 ? tempoEntreObstaculos - 1 : tempoEntreObstaculos;  
                        }
                        moverObsFase1(obs, maxObj);
                        break;
                    //FASE2    
                    case 2:
                        if (contadorObstaculos >= tempoEntreObstaculos)
                        {
                            obstaculoAtual = (obstaculoAtual + 1) % maxObj;
                            obs[obstaculoAtual] = gerarNovoObj();
                            obs[obstaculoAtual].x_obj = MINX + 1;

                            if (obs[obstaculoAtual].y_obj <= MINY) {
                                obs[obstaculoAtual].y_obj = MINY + 1;
                            }

                            printObj(obs[obstaculoAtual], DESENHAR);

                            contadorObstaculos = 0;                    
                            tempoEntreObstaculos = tempoEntreObstaculos > 30 ? tempoEntreObstaculos - 1 : tempoEntreObstaculos;
                        }
                        moverObsFase2(obs, maxObj);
                        break;
                    //FASE3    
                    case 3:
                        if (contadorObstaculos >= tempoEntreObstaculos)
                        {
                            obstaculoAtual = (obstaculoAtual + 1) % maxObj;
                            obs[obstaculoAtual] = gerarNovoObj();
                            obs[obstaculoAtual].y_obj = MINY + 1;

                            if (obs[obstaculoAtual].x_obj + obs[obstaculoAtual].tamanho >= MAXX) {
                                obs[obstaculoAtual].x_obj = MAXX - obs[obstaculoAtual].tamanho - 1;
                            }

                            printObj(obs[obstaculoAtual], DESENHAR);

                            contadorObstaculos = 0;                    
                            tempoEntreObstaculos = tempoEntreObstaculos > 25 ? tempoEntreObstaculos - 1 : tempoEntreObstaculos;
                        }
                        moverObsFase3(obs, maxObj);
                        break;
                    //FASE4
                    case 4:
                        if (contadorObstaculos >= tempoEntreObstaculos)
                        {
                            obstaculoAtual = (obstaculoAtual + 1) % maxObj;
                            obs[obstaculoAtual] = gerarNovoObj();
                            obs[obstaculoAtual].y_obj = MAXY - obs[obstaculoAtual].tamanho - 1;

                            if (obs[obstaculoAtual].x_obj + obs[obstaculoAtual].tamanho >= MAXX) {
                                obs[obstaculoAtual].x_obj = MAXX - obs[obstaculoAtual].tamanho - 1;
                            }

                            printObj(obs[obstaculoAtual], DESENHAR);

                            contadorObstaculos = 0;                    
                            tempoEntreObstaculos = tempoEntreObstaculos > 20 ? tempoEntreObstaculos - 1 : tempoEntreObstaculos;
                        }
                        moverObsFase4(obs, maxObj);
                        break;
                    default:
                        break;
                }
                
                printPersonagem(x_perso, y_perso);
                if (verificarColisao(obs, maxObj))
                {   
                    printGameOver(DESENHAR);
                    
                    timerDestroy();
                    jogoIniciado = 0;

                    for (int i = 0; i < maxObj; i++) {
                        printObj(obs[i], APAGAR);
                        obs[i].tamanho = 0;
                    }

                    y_setaMenu = y_menuInicial;
                    printMenuInicial();
                    printSetaMenu(y_setaMenu);
                }
            }
            
            screenHideCursor();
            screenUpdate();
        }


        screenHideCursor();
        screenUpdate();
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    free(obs);
    obs = NULL;

    return 0;
}
