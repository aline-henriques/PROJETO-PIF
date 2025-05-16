#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/timer.h"

#define MAX_RECORDS 1000

int pontuacao = 0;
int x_menuInicial = MAXX * 0.05, y_menuInicial = MAXY * 0.5;
int x_setaMenu = (MAXX * 0.05)-2, y_setaMenu = MAXY * 0.5;
int x_perso = MAXX * 0.5, y_perso = MAXY * 0.5;
int x_boss = MAXX + 1, y_boss = MAXY * 0.3;
int x_pont = 0,y_pont = 0;
int x_gameOver = MAXX * 0.60, y_gameOver = MAXY * 0.5;
int x_config = (MAXX * 0.5)-2, y_config = (MAXY * 0.5)-2;


typedef enum
{
    VERMELHO,
    VERDE,
    AZUL,
    AMARELO,
    COR_TOTAL
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

//possiveis melhorias
typedef enum
{
    HORIZONTAL,
    VERTICAL
} Direcao;

typedef struct 
{
    int ativo;
    int x_tiro;
    int y_tiro;
} Tiro;

typedef struct 
{
    char data[20];
    int pontos;
} Pontuacao;

void printTirosBoss(Tiro tb, TipoAcao acao)
{
    const char* tiroBoss_desenhado[] = {
        "   <<<<",
        "  <<<< ",
        " <<<<  ",
        "<<<<   ",
        " <<<<  ",
        "  <<<< ",
        "   <<<<"
    };

    const char* tiroBoss_apagado[] = {
        "       ",
        "       ",
        "       ",
        "       ",
        "       ",
        "       ",
        "       "
    };

    const char* const* tiroBoss = (acao == DESENHAR) ? tiroBoss_desenhado : tiroBoss_apagado;

    screenSetColor(MAGENTA, DARKGRAY);

    for (int i = 0; i < 7; i++)
    {
        screenGotoxy(tb.x_tiro, tb.y_tiro + i);
        for (int j = 0; j < 7; j++)
        {
            printf("%c", tiroBoss[i][j]);
        }
    }
}

Tiro gerarNovoTiroBoss()
{
    Tiro novo;
    novo.ativo = 0;
    novo.x_tiro = x_boss - 3;
    novo.y_tiro = y_boss;
    return novo;
}

void moverTirosBoss(Tiro* tiro, int maxTirosBoss)
{
    for (int i = 0; i < maxTirosBoss; i++) {
        if (tiro[i].ativo) {
            printTirosBoss(tiro[i], APAGAR);
            tiro[i].x_tiro = tiro[i].x_tiro - 2;

            if (tiro[i].x_tiro < MINX + 1) {
                tiro[i].ativo = 0;
            } else {
                printTirosBoss(tiro[i], DESENHAR);
            }
        }
    }
}

void printTiros(Tiro t, TipoAcao acao)
{
    char* tiros = ( acao == DESENHAR ) ? "- - -" : "     ";

    screenSetColor(RED, DARKGRAY);
    screenGotoxy(t.x_tiro, t.y_tiro);
    printf("%s", tiros);
}

Tiro gerarNovoTiro()
{
    Tiro novo;
    novo.ativo = 0;
    novo.x_tiro = x_perso + 3;
    novo.y_tiro = y_perso + 1;
    return novo;
}

void moverTiros(Tiro* tiro, int maxTiros)
{
    for (int i = 0; i < maxTiros; i++) {
        if (tiro[i].ativo) {
            printTiros(tiro[i], APAGAR);
            tiro[i].x_tiro++;

            if (tiro[i].x_tiro >= MAXX - 6) {
                tiro[i].ativo = 0;
                printTiros(tiro[i], APAGAR);
            } else {
                printTiros(tiro[i], DESENHAR);
            }
        }
    }
}

void printBossFinal(int x, int y, TipoAcao acao) {
    const char* boss_desenhado[] = {
        "  _______  ",
        " /       \\ ",
        "|(>_<)   | ",
        " \\_______/ ",
        "  / | \\    ",
        " /  |  \\   ",
        "/   |   \\  "
       
    };

    const char* boss_apagado[] = {
        "           ",
        "           ",
        "           ",
        "           ",
        "           ",
        "           ",
        "           "
    };

    const char* const* boss = (acao == DESENHAR) ? boss_desenhado : boss_apagado;

    x_boss = x;
    y_boss = y;

    screenSetColor(MAGENTA, DARKGRAY);

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < strlen(boss[i]); j++) {
            int pos_x = x + j;
            if (pos_x >= MINX && pos_x < MAXX) {
                screenGotoxy(pos_x - 1, y + i);
                printf("%c", boss[i][j]);
            }
        }
    }
}

int iniciarBossnaTela() {
    static int direcao_boss = 1;
    int bossPronto = 0;

    if (x_boss > MAXX - 10) {
        printBossFinal(x_boss, y_boss, APAGAR);
        x_boss -= 2;
        printBossFinal(x_boss, y_boss, DESENHAR);
    } else {
        bossPronto = 1;
        printBossFinal(x_boss, y_boss, APAGAR);

        y_boss += direcao_boss;

        if (y_boss <= MINY + 1) {
            direcao_boss = 1;
        } else if (y_boss >= MAXY - 7) {
            direcao_boss = -1;
        }

        printBossFinal(x_boss, y_boss, DESENHAR);
    }
    return bossPronto;
}

void printGameOver(TipoAcao acao)
{
    const char* simbolo = (acao == DESENHAR) ? "💥 GAME OVER! 💥" : "                ";

    screenSetColor(RED, DARKGRAY);

    screenGotoxy(x_gameOver, y_gameOver);
    printf("%s", simbolo);
}

void printObj(Obstaculos o, TipoAcao acao)
{
    char simbolo = (acao == DESENHAR) ? 'X' : ' ';
    screenSetColor(WHITE, DARKGRAY);

    for (int i = 0; i < o.tamanho; i++)
    {
        screenGotoxy(o.x_obj, o.y_obj + i);
        for (int j = 0; j < o.tamanho; j++)
        {
            printf("%c", simbolo);
        }
    }
}

Obstaculos gerarNovoObj()
{
    Obstaculos novo;
    novo.tamanho = 3 + rand() % 6;
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

void printMenuInicial(TipoAcao acao)
{
    const char* menu_desenhado[] = {
        "Novo Jogo",
        "Pontuação",
        "Configuração",
        "Fechar Jogo"
    };

    const char* menu_apagado[] = {
        "         ",
        "         ",
        "            ",
        "           "
    };

    const char* const* opcoes = (acao == DESENHAR) ? menu_desenhado : menu_apagado;

    screenSetColor(CYAN, DARKGRAY);

    for (int i = 0; i < 4; i++){
        screenGotoxy(x_menuInicial, y_menuInicial+i);
        printf("%s", opcoes[i]);
    }

}

void printSetaMenu(int nextY, TipoAcao acao){
    const char* seta = (acao == DESENHAR) ? "->" : "  ";

    screenSetColor(CYAN, DARKGRAY);

    y_setaMenu = nextY;

    screenGotoxy(x_setaMenu, y_setaMenu);
    printf("%s", seta);
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

void printPontuacao(int x_pont, int y_pont, TipoAcao acao)
{
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(x_pont, y_pont);
    if (acao == DESENHAR) {
        printf("Score: %d", pontuacao);
    } else {
        printf("          ");
    }
}

void printPersonagem(int nextX, int nextY, TipoAcao acao, Avatar a1)
{

    screenSetColor(a1.cor, DARKGRAY);

    if (acao == DESENHAR)
    {
        x_perso = nextX;
        y_perso = nextY;

        for (int i = 0; i < 3; i++)
        {
            screenGotoxy(x_perso, y_perso+i);
            printf("%s",a1.personagem.linhas[i]);
        }
    }else{
        for (int i = 0; i < 3; i++)
        {
            screenGotoxy(x_perso, y_perso+i);
            printf("   ");
        }
    }
}

int verificarColisaoFases(Obstaculos* obs, int maxObj)
{
    if (!obs) return 0;

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

void printMensagemBoss(TipoAcao acao) 
{
    const char* mensagem[] = {
        "========================",
        "      CHEFÃO FINAL      ",
        " BARRA DE ESPAÇO ATIRA  ",
        "PRESSIONE QUALQUER TECLA",
        "========================"
    };
    
    screenSetColor(YELLOW, DARKGRAY);
    int y_pos = MAXY * 0.4; // Posição vertical centralizada
    
    for (int i = 0; i < 5; i++) {
        screenGotoxy(MAXX/2 - strlen(mensagem[i])/2, y_pos + i);
        printf("%s", (acao == DESENHAR) ? mensagem[i] : "                        ");
    }
}

int verificarColisaoTiroBossComPersonagem(Tiro* tiros, int maxTiros) 
{
    for (int i = 0; i < maxTiros; i++) 
    {
        if (tiros[i].ativo) 
        {
            int tx = tiros[i].x_tiro;
            int ty = tiros[i].y_tiro;
            
            const char* padraoTiro[7] = {
                "   <<<<",
                "  <<<< ",
                " <<<<  ",
                "<<<<   ",
                " <<<<  ",
                "  <<<< ",
                "   <<<<" 
            };
            
            // Verifica cada caractere '<' do tiro
            for (int linha = 0; linha < 7; linha++) 
            {
                for (int coluna = 0; coluna < 7; coluna++) 
                {
                    if (padraoTiro[linha][coluna] == '<') 
                    {
                        // Posição absoluta do '<' na tela
                        int px = tx + coluna;
                        int py = ty + linha;
                        
                        // Verifica colisão com o personagem (3x3)
                        if (px >= x_perso && px <= x_perso + 2 &&
                            py >= y_perso && py <= y_perso + 2) 
                        {
                            return 1; // Colisão detectada
                        }
                    }
                }
            }
        }
    }
    return 0; // Sem colisão
}

int verificarColisaoTiroComBoss(Tiro* tiros, int maxTiros) {
    for (int i = 0; i < maxTiros; i++) {
        if (tiros[i].ativo) {
            // Verifica se o tiro está dentro da área do boss
            if (tiros[i].x_tiro >= x_boss && tiros[i].x_tiro <= x_boss + 10 &&
                tiros[i].y_tiro >= y_boss && tiros[i].y_tiro <= y_boss + 6) {
                tiros[i].ativo = 0;
                printTiros(tiros[i], APAGAR);
                return 1;
            }
        }
    }
    return 0;
}

void limparTiros(Tiro* tiros, int maxTiros) 
{
    for (int i = 0; i < maxTiros; i++) 
    {
        if (tiros[i].ativo) 
        {
            printTirosBoss(tiros[i], APAGAR);
            tiros[i].ativo = 0;
        }
    }
}

int comparar_pontuacoes(const void *a, const void *b) {
    return ((Pontuacao*)b)->pontos - ((Pontuacao*)a)->pontos;
}

void salvar_pontuacao(int nova_pontuacao) {
    // Obter data/hora atual
    time_t agora;
    time(&agora);
    char data_hora[20];
    strftime(data_hora, 20, "%d/%m/%Y %H:%M", localtime(&agora));

    // Carregar pontuações existentes
    Pontuacao records[MAX_RECORDS];
    int num_records = 0;
    
    FILE *arquivo = fopen("pontuacao.json", "r");
    if (arquivo != NULL) {
        char linha[256];
        while (fgets(linha, sizeof(linha), arquivo) && num_records < MAX_RECORDS) {
            if (sscanf(linha, "{\"data\": \"%19[^\"]\", \"pontos\": %d}", 
                      records[num_records].data, &records[num_records].pontos) == 2) {
                num_records++;
            }
        }
        fclose(arquivo);
    }

    // Adicionar nova pontuação
    if (num_records < MAX_RECORDS) {
        strcpy(records[num_records].data, data_hora);
        records[num_records].pontos = nova_pontuacao;
        num_records++;
    } else {
        // Se atingiu o limite, verifica se a nova pontuação é maior que a menor existente
        qsort(records, num_records, sizeof(Pontuacao), comparar_pontuacoes);
        if (nova_pontuacao > records[MAX_RECORDS-1].pontos) {
            strcpy(records[MAX_RECORDS-1].data, data_hora);
            records[MAX_RECORDS-1].pontos = nova_pontuacao;
        } else {
            // Se não for maior, não salva
            return;
        }
    }

    // Ordenar do maior para o menor
    qsort(records, num_records, sizeof(Pontuacao), comparar_pontuacoes);

    // Salvar de volta no arquivo
    arquivo = fopen("pontuacao.json", "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar pontuação!\n");
        return;
    }

    // Limitar a 20 melhores pontuações
    int limite = (num_records > 20) ? 20 : num_records;
    for (int i = 0; i < limite; i++) {
        fprintf(arquivo, "{\"data\": \"%s\", \"pontos\": %d}\n", 
                records[i].data, records[i].pontos);
    }

    fclose(arquivo);
}

void ler_pontuacoes() 
{
    char linha[256];
    int y_pos = 8;
    int contador = 1;
    
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(MAXX/2 - 15, 3);
    printf("📊 HISTÓRICO DE PONTUAÇÕES");

    // Linha divisória
    screenSetColor(WHITE, DARKGRAY);
    for (int x = MAXX/4; x < MAXX*3/4; x++) {
        screenGotoxy(x, 4);
        printf("-");
    }

    // Cabeçalho da tabela
    screenSetColor(GREEN, DARKGRAY);
    screenGotoxy(MAXX/4, 6);
    printf("POSIÇÃO");
    screenGotoxy(MAXX/4 + 15, 6);
    printf("DATA");
    screenGotoxy(MAXX/2 + 15, 6);
    printf("PONTOS");

    // Abre o arquivo
    FILE *arquivo = fopen("pontuacao.json", "r");
    if (arquivo == NULL) {
        screenSetColor(YELLOW, DARKGRAY);
        screenGotoxy(MAXX/2 - 15, 8);
        printf("Nenhuma pontuação registrada ainda.");
        // Espera por input antes de voltar
        screenGotoxy(MAXX/2 - 15, MAXY - 2);
        printf("Pressione qualquer tecla para voltar...");
        while (!keyhit());
        readch();
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        char data[20];
        int pontos;
        
        if (sscanf(linha, "{\"data\": \"%19[^\"]\", \"pontos\": %d}", data, &pontos) == 2) {
            screenSetColor(WHITE, DARKGRAY);
            
            // Posição (ranking)
            screenGotoxy(MAXX/4, y_pos);
            printf("%02d.", contador);
            
            // Data
            screenGotoxy(MAXX/4 + 15, y_pos);
            printf("%s", data);
            
            // Pontuação
            screenSetColor(YELLOW, DARKGRAY);
            screenGotoxy(MAXX/2 + 15, y_pos);
            printf("%d", pontos);
            
            y_pos++;
            contador++;
            
            // Limita a exibir 20 registros por vez
            if (contador > 20) break;
        }
    }
    
    fclose(arquivo);
}

void printConfig(TipoAcao acao, Avatar a1)
{
    const char* config_desenhado[] = {
        "  ↑  ",
        "     ",
        "←   →",
        "     ",
        "  ↓  "
  

       
    };

    const char* config_apagado[] = {
        "     ",
        "     ",
        "     ",
        "     ",
        "     "
    };

    const char* const* config = (acao == DESENHAR) ? config_desenhado : config_apagado;

    screenSetColor(WHITE, DARKGRAY);

    for (int i = 0; i < 5; i++)
    {
        screenGotoxy(x_config, y_config + i);

        for (int i = 0; i < 5; i++)
        {
            screenGotoxy(x_config, y_config + i);
            printf("%s", config[i]);
        } 
    }

    printPersonagem(x_config + 1, y_config + 1, acao, a1);
    
}

int main() 
{
    static int ch = 0;
    int jogoIniciado = 0;
    int maxObj = 110;
    int maxTiros = 500;
    int maxTirosBoss = 500;
    Obstaculos* obs = malloc(sizeof(Obstaculos) * maxObj);
    if (!obs) 
    {
        printf("Erro ao alocar memória para obstáculos!\n");
    }
    Tiro* tiroPersonagem = malloc(sizeof(Tiro) * maxTiros);
    if (!tiroPersonagem) 
    {
        printf("Erro ao alocar memória para Tiros do Personagem!\n");
    }
    Tiro* tiroBoss = malloc(sizeof(Tiro) * maxTirosBoss);
    if (!tiroBoss) 
    {
        printf("Erro ao alocar memória para Tiros do BOSS!\n");
    }
    int obstaculoAtual = 0;
    int tiroAtual = 0;
    int tiroBossAtual = 0;
    int tempoEntreObstaculos = 60;
    int tempoEntreTiros = 10;
    int tempoEntreTirosBoss = 20;
    int contadorObstaculos = 0;
    int contadorTirosBoss = 0;
    int tempoinicial = 0;
    int tempoTiro = 0;
    int intervalo = 30;
    int faseAtual = 1;
    int novaFase = 1;
    int contadorMovimentoBoss = 0;
    int bossIniciado = 0;
    int bossPronto = 0;
    int bossVida = 10;
    Avatar avatar1 = {VERMELHO, personagens[0]};;
    int navePerso = 0;


    for (int i = 0; i < maxObj; i++)
    {
        obs[i].tamanho = 0;
    }
    for (int i = 0; i < maxTiros; i++) {
        tiroPersonagem[i].ativo = 0;
    }
    for (int i = 0; i < maxTirosBoss; i++) {
        tiroBoss[i].ativo = 0;
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
            printMenuInicial(DESENHAR);
            printSetaMenu(y_setaMenu, DESENHAR);
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
                        if (y_setaMenu >= y_menuInicial)
                        {
                            printSetaMenu(y_setaMenu, APAGAR);
                            printSetaMenu(y_setaMenu-1, DESENHAR);
                            if (y_setaMenu == y_menuInicial-1)
                            {
                                printSetaMenu(y_setaMenu, APAGAR);
                                printSetaMenu(y_setaMenu+4, DESENHAR);
                            }
                        }
                        break;

                    case 80:
                        if (y_setaMenu <= y_menuInicial + 3)
                        {
                            printSetaMenu(y_setaMenu, APAGAR);
                            printSetaMenu(y_setaMenu+1, DESENHAR);
                            if (y_setaMenu == y_menuInicial + 4)
                            {
                                printSetaMenu(y_setaMenu, APAGAR);
                                printSetaMenu(y_setaMenu-4, DESENHAR);
                            }
                        }
                        break;

                }

                if (ch == 13)
                {
                    int opcaoSelecionada = y_setaMenu - y_menuInicial;

                    switch (opcaoSelecionada)
                    {
                        case 0:
                            timerDestroy();
                            bossIniciado = 0;
                            bossVida = 10;
                            pontuacao = 0;
                            faseAtual = 1;
                            novaFase = 1;
                            obstaculoAtual = 0;
                            contadorObstaculos = 0;
                            tempoEntreObstaculos = 60;
                            tempoinicial = 0;
                            tempoTiro = 0;
                            contadorTirosBoss = 0;
                            tiroAtual = 0;
                            tiroBossAtual = 0;
                            contadorMovimentoBoss = 0;

                            x_perso = MAXX * 0.5;
                            y_perso = MAXY * 0.5;
                            x_boss = MAXX + 1;
                            
                            for (int i = 0; i < maxObj; i++) 
                            {
                                printObj(obs[i], APAGAR);
                                obs[i].tamanho = 0;
                            }
                            
                            for (int i = 0; i < maxTiros; i++) 
                            {
                                printTiros(tiroPersonagem[i], APAGAR);
                                tiroPersonagem[i].ativo = 0;
                            }
                            
                            for (int i = 0; i < maxTirosBoss; i++) 
                            {
                                printTirosBoss(tiroBoss[i], APAGAR);
                                tiroBoss[i].ativo = 0;
                            }
                            
                            printGameOver(APAGAR);
                            printPontuacao(x_gameOver, y_gameOver + 1, APAGAR);
                            printMenuInicial(APAGAR);
                            printSetaMenu(y_setaMenu, APAGAR);
                            
                            jogoIniciado = 1;
                            printPersonagem(MAXX*0.5, MAXY*0.5, DESENHAR, avatar1);
                            timerInit(30);
                            screenInit(1);
                            break;
                        case 1:
                            screenClear();
                            screenInit(1);
                            ler_pontuacoes();
                            printf("\nPressione qualquer tecla para voltar...");
                            while (!keyhit());
                            readch(); 
                            screenClear();
                            screenInit(1);
                            break;
                        case 2:
                            screenClear();
                            screenInit(1);
                            printConfig(DESENHAR, avatar1);
                            printf("\nPressione qualquer tecla para voltar...");
                            int total_personagens = sizeof(personagens) / sizeof(personagens[0]);
                            while (1) 
                            {
                                if (keyhit()) 
                                {
                                    int ch = readch();
                                    if (ch == 0 || ch == 224) 
                                    {
                                        ch = readch();
                                    }
                                    switch (ch){

                                        //cima
                                        case 72:
                                            printPersonagem(x_config + 1, y_config + 1, APAGAR, avatar1);
                                            avatar1.cor = (avatar1.cor + 1) % COR_TOTAL;
                                            printPersonagem(x_config + 1, y_config + 1, DESENHAR, avatar1);
                                            break;

                                        //esquerda
                                        case 75:
                                            printPersonagem(x_config + 1, y_config + 1, APAGAR, avatar1);
                                            navePerso--;
                                            avatar1.personagem = personagens[navePerso];
                                            if (navePerso < 0)
                                            {
                                                navePerso = total_personagens-1;
                                                avatar1.personagem = personagens[navePerso];
                                            }
                                            printPersonagem(x_config + 1, y_config + 1, DESENHAR, avatar1);
                                            break;

                                        //baixo
                                        case 80:
                                            printPersonagem(x_config + 1, y_config + 1, APAGAR, avatar1);
                                            avatar1.cor = (avatar1.cor - 1 + COR_TOTAL) % COR_TOTAL;
                                            printPersonagem(x_config + 1, y_config + 1, DESENHAR, avatar1);                                          
                                            break;

                                        //direita
                                        case 77:
                                            printPersonagem(x_config + 1, y_config + 1, APAGAR, avatar1);
                                            navePerso++;
                                            avatar1.personagem = personagens[navePerso];
                                            if (navePerso >= total_personagens)
                                            {
                                                navePerso = 0;
                                                avatar1.personagem = personagens[navePerso];
                                            }
                                            printPersonagem(x_config + 1, y_config + 1, DESENHAR, avatar1);
                                            break;
                                    }
                                    if (!(ch == 72 || ch == 75 || ch == 80 || ch == 77))
                                    {
                                        break;
                                    }
                                }
                            }
                            readch();
                            screenClear();
                            screenInit(1);
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
                            printPersonagem(x_perso, y_perso, APAGAR, avatar1);
                            printPersonagem(x_perso, y_perso - 1, DESENHAR, avatar1);
                        }
                        break;

                    //esquerda
                    case 75:
                        if(x_perso - 1 >MINX)
                        {
                            printPersonagem(x_perso, y_perso, APAGAR, avatar1);
                            printPersonagem(x_perso - 2, y_perso, DESENHAR, avatar1);
                        }
                        break;

                    //baixo
                    case 80:
                        if(y_perso + 3 < MAXY)
                        {
                            printPersonagem(x_perso, y_perso, APAGAR, avatar1);
                            printPersonagem(x_perso, y_perso + 1, DESENHAR, avatar1);
                        }
                        break;

                    //direita
                    case 77:
                        if(x_perso + 4 < MAXX)
                        {
                            printPersonagem(x_perso, y_perso, APAGAR, avatar1);
                            printPersonagem(x_perso + 2, y_perso, DESENHAR, avatar1);
                        }
                        break;
                    case 32:
                        
                        if (jogoIniciado == 1 && faseAtual == 5 && tiroAtual < maxTiros && (tempoinicial - tempoTiro) >= intervalo)
                    {
                        tiroPersonagem[tiroAtual] = gerarNovoTiro();
                        tiroPersonagem[tiroAtual].ativo = 1;
                        printTiros(tiroPersonagem[tiroAtual], DESENHAR);
                        tiroAtual = (tiroAtual + 1) % maxTiros;
                        tempoTiro = tempoinicial;
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
                pontuacao++;

                printPontuacao(x_pont, y_pont, DESENHAR);
                if (faseAtual == 5)
                {
                    tempoinicial++;
                }
                

                //Atualização de fase
                if (obstaculoAtual >= 100)novaFase = 5;
                else if (obstaculoAtual >= 60)novaFase = 4;
                else if (obstaculoAtual >= 40)novaFase = 3;
                else if (obstaculoAtual >= 20)novaFase = 2;

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
                        case 5: timerUpdateTimer(50);break;
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
                    //FINALBOSS
                    case 5:
                        printKey(ch);                        
                        if (!bossIniciado) {
                            printMensagemBoss(DESENHAR);
                            screenUpdate();
                            
                            // Espera até que qualquer tecla seja pressionada
                            while (!keyhit()) {
                                Sleep(100); // Pequena pausa para não consumir muitos recursos
                            }
                            readch(); // Limpa o buffer do teclado
                            
                            printMensagemBoss(APAGAR);
                            bossIniciado = 1;
                            timerUpdateTimer(50);
                        }
                        contadorMovimentoBoss++;
                        
                        if (contadorMovimentoBoss > 2)
                        {
                            bossPronto = iniciarBossnaTela();
                            contadorMovimentoBoss = 0;
                        }  

                        moverTiros(tiroPersonagem, maxTiros);

                        if (bossPronto && (tempoEntreTirosBoss < tempoinicial - contadorTirosBoss)) {
                            for (int i = 0; i < maxTirosBoss; i++) {
                                if (!tiroBoss[i].ativo) {
                                    tiroBoss[i] = gerarNovoTiroBoss();
                                    tiroBoss[i].ativo = 1;
                                    printTirosBoss(tiroBoss[i], DESENHAR);
                                    contadorTirosBoss = tempoinicial;
                                    break;
                                }
                            }
                        }
                        
                        moverTirosBoss(tiroBoss, maxTirosBoss);

                        if ((faseAtual == 5 && tiroBossAtual < maxTirosBoss) && (intervalo < tempoinicial - contadorTirosBoss))
                        {
                            tiroBoss[tiroBossAtual] = gerarNovoTiroBoss();
                            tiroBossAtual++;
                            contadorTirosBoss = tempoinicial;
                        }

                        if (verificarColisaoTiroComBoss(tiroPersonagem, maxTiros)) 
                        {
                            bossVida--;
                            pontuacao+=100;
                            if (bossVida <= 0) 
                            {
                                printGameOver(DESENHAR);
                                salvar_pontuacao(pontuacao);
                                printPontuacao(x_gameOver, y_gameOver + 1, DESENHAR);
                                jogoIniciado = 0;
                            }
                        }

                        if (verificarColisaoTiroBossComPersonagem(tiroBoss, maxTirosBoss))
                         {
                            printGameOver(DESENHAR);
                            salvar_pontuacao(pontuacao);
                            printPontuacao(x_gameOver, y_gameOver + 1, DESENHAR);
                            jogoIniciado = 0;
                            faseAtual = 1;
                            novaFase = 1;
                        }
                         
                        
                        break;
                    default:
                        break;
                }
                
                printPersonagem(x_perso, y_perso, APAGAR, avatar1);
                printPersonagem(x_perso, y_perso, DESENHAR, avatar1);
                if (verificarColisaoFases(obs, maxObj))
                {   
                    printGameOver(DESENHAR);
                    salvar_pontuacao(pontuacao);
                    printPontuacao(x_gameOver, y_gameOver + 1, DESENHAR);
                    
                    timerDestroy();
                    jogoIniciado = 0;
                    faseAtual = 1;
                    novaFase = 1;

                    for (int i = 0; i < maxObj; i++) {
                        printObj(obs[i], APAGAR);
                        obs[i].tamanho = 0;
                    }

                    y_setaMenu = y_menuInicial;
                    printMenuInicial(DESENHAR);
                    printSetaMenu(y_setaMenu, DESENHAR);
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
    free(tiroPersonagem);
    free(tiroBoss);
    obs = NULL;

    return 0;
}
