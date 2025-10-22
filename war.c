// arquivo: war_combined.c
// Compilar: gcc -std=c11 -O2 -Wall -Wextra war_combined.c -o war_combined

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ========================
   CONFIGURAÇÕES / CONSTANTES
   ======================== */
#define MAX_NAME_SLOTS 5
#define MAX_NAME_LEN 32
#define BOARD_SLOTS 5
#define LINE_BUF 128

/* ========================
   ESTRUTURAS
   ======================== */
typedef struct {
    char names[MAX_NAME_SLOTS][MAX_NAME_LEN]; // nomes alternativos
    char color[16];                            // cor do exército
    int troops;                                // número de tropas
} Territory;

/* ========================
   PROTÓTIPOS
   ======================== */
static void strip_newline(char *s);
Territory *criar_mapa(int n);
void liberar_mapa(Territory *mapa);
void init_empty_territory(Territory *t);
const char* primary_name(const Territory *t);
void print_territory(const Territory *t, int index);
void exibir_mapa(const Territory *mapa, int n);
void input_phase(Territory *mapa, int n);
int ler_inteiro_prompt(const char *prompt, int min, int max);
void simular_ataque(Territory *mapa, int atacante_idx, int defensor_idx);

/* ========================
   IMPLEMENTAÇÕES
   ======================== */

static void strip_newline(char *s) {
    size_t l = strlen(s);
    if (l > 0 && s[l - 1] == '\n') s[l - 1] = '\0';
}

/* Aloca dinamicamente um vetor de territórios */
Territory *criar_mapa(int n) {
    Territory *mapa = malloc(sizeof(Territory) * n);
    if (!mapa) {
        fprintf(stderr, "Erro: falha ao alocar memória para o mapa.\n");
        exit(EXIT_FAILURE);
    }
    return mapa;
}

void liberar_mapa(Territory *mapa) {
    free(mapa);
}

void init_empty_territory(Territory *t) {
    for (int i = 0; i < MAX_NAME_SLOTS; ++i)
        t->names[i][0] = '\0';
    t->color[0] = '\0';
    t->troops = 0;
}

const char* primary_name(const Territory *t) {
    for (int i = 0; i < MAX_NAME_SLOTS; ++i)
        if (t->names[i][0] != '\0')
            return t->names[i];
    return "SemNome";
}

void print_territory(const Territory *t, int index) {
    printf("%d) %s - Cor: %s - Tropas: %d\n",
           index + 1,
           primary_name(t),
           (t->color[0] != '\0' ? t->color : "indefinida"),
           t->troops);
}

void exibir_mapa(const Territory *mapa, int n) {
    puts("\n--- MAPA ATUAL ---");
    for (int i = 0; i < n; ++i) {
        print_territory(&mapa[i], i);
    }
    puts("------------------\n");
}

/* Lê um inteiro entre min e max (inclusive), repetindo até ter entrada válida */
int ler_inteiro_prompt(const char *prompt, int min, int max) {
    char buf[LINE_BUF];
    int valor;
    while (1) {
        if (prompt && prompt[0] != '\0') printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) {
            printf("\nEntrada encerrada. Saindo.\n");
            exit(EXIT_FAILURE);
        }
        strip_newline(buf);
        if (sscanf(buf, "%d", &valor) == 1) {
            if (valor >= min && valor <= max) return valor;
        }
        printf("Entrada inválida. Digite um número entre %d e %d.\n", min, max);
    }
}

/* Fase de input: nome, cor e tropas para cada território (usuário fornece) */
void input_phase(Territory *mapa, int n) {
    char line[LINE_BUF];

    printf("========================================\n");
    printf("Vamos cadastrar os %d territorios iniciais do nosso mundo.\n\n", n);

    for (int i = 0; i < n; ++i) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);
        init_empty_territory(&mapa[i]);

        // NOME
        printf("Nome do Territorio: ");
        if (!fgets(line, sizeof(line), stdin)) exit(EXIT_FAILURE);
        strip_newline(line);
        if (strlen(line) == 0) {
            strncpy(mapa[i].names[0], "SemNome", MAX_NAME_LEN - 1);
            mapa[i].names[0][MAX_NAME_LEN - 1] = '\0';
        } else {
            strncpy(mapa[i].names[0], line, MAX_NAME_LEN - 1);
            mapa[i].names[0][MAX_NAME_LEN - 1] = '\0';
        }

        // COR
        printf("Cor do exército: ");
        if (!fgets(line, sizeof(line), stdin)) exit(EXIT_FAILURE);
        strip_newline(line);
        if (strlen(line) == 0) {
            strncpy(mapa[i].color, "indefinida", sizeof(mapa[i].color) - 1);
            mapa[i].color[sizeof(mapa[i].color) - 1] = '\0';
        } else {
            strncpy(mapa[i].color, line, sizeof(mapa[i].color) - 1);
            mapa[i].color[sizeof(mapa[i].color) - 1] = '\0';
        }

        // TROPAS
        while (1) {
            printf("Numero de tropas: ");
            if (!fgets(line, sizeof(line), stdin)) exit(EXIT_FAILURE);
            strip_newline(line);
            int troops;
            if (sscanf(line, "%d", &troops) == 1 && troops >= 0) {
                mapa[i].troops = troops;
                break;
            } else {
                printf("  Entrada invalida. Digite um numero inteiro >= 0.\n");
            }
        }
        printf("\n");
    }

    // mostra o tabuleiro inicial
    exibir_mapa(mapa, n);
}

/* Simula um ataque simples (um dado cada lado), atualiza tropas e imprime resultado */
void simular_ataque(Territory *mapa, int atacante_idx, int defensor_idx) {
    Territory *atk = &mapa[atacante_idx];
    Territory *def = &mapa[defensor_idx];

    puts("\n--- FASE DE ATAQUE ---");

    if (atacante_idx == defensor_idx) {
        printf("Você escolheu o mesmo território como atacante e defensor. Operação cancelada.\n");
        return;
    }

    if (atk->troops <= 1) {
        printf("Ataque inválido: %s não tem tropas suficientes para atacar (tropas atuais: %d).\n",
               primary_name(atk), atk->troops);
        return;
    }
    if (def->troops <= 0) {
        printf("Defensor %s não tem tropas para perder.\n", primary_name(def));
        return;
    }

    printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): %d\n", BOARD_SLOTS, atacante_idx + 1);
    printf("Escolha o territorio defensor (1 a %d): %d\n\n", BOARD_SLOTS, defensor_idx + 1);

    /* rolar um dado de 1 a 6 para cada lado (simples, como na imagem) */
    int dado_atk = (rand() % 6) + 1;
    int dado_def = (rand() % 6) + 1;

    puts("--- RESULTADO DA BATALHA ---");
    printf("O atacante %s rolou um dado e tirou: %d\n", primary_name(atk), dado_atk);
    printf("O defensor %s rolou um dado e tirou: %d\n", primary_name(def), dado_def);

    if (dado_atk > dado_def) {
        def->troops -= 1;
        if (def->troops < 0) def->troops = 0;
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
    } else if (dado_atk < dado_def) {
        atk->troops -= 1;
        if (atk->troops < 0) atk->troops = 0;
        printf("VITORIA DO DEFENSOR! O atacante perdeu 1 tropa.\n");
    } else {
        // empate -> atacante perde 1 (regra local)
        atk->troops -= 1;
        if (atk->troops < 0) atk->troops = 0;
        printf("EMPATE! Por regra local, atacante perde 1 tropa.\n");
    }
}

/* ========================
   FUNÇÃO MAIN
   ======================== */
int main(void) {
    srand((unsigned int)time(NULL));

    // aloca mapa dinamicamente
    Territory *mapa = criar_mapa(BOARD_SLOTS);

    // fase 1: input (nome, cor, tropas)
    input_phase(mapa, BOARD_SLOTS);

    // loop principal de turnos / ataques
    while (1) {
        exibir_mapa(mapa, BOARD_SLOTS);

        int opc_atk = ler_inteiro_prompt("Escolha o territorio atacante (1 a 5, ou 0 para sair): ", 0, BOARD_SLOTS);
        if (opc_atk == 0) {
            puts("Saindo do jogo. Obrigado por jogar!");
            break;
        }
        opc_atk -= 1; // converte para 0-based

        int opc_def = ler_inteiro_prompt("Escolha o territorio defensor (1 a 5): ", 1, BOARD_SLOTS);
        opc_def -= 1; // 0-based

        simular_ataque(mapa, opc_atk, opc_def);
    }

    // libera memória
    liberar_mapa(mapa);
    return 0;
}
