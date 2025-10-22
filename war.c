// war_like_board_input.c
#include <stdio.h>   // Biblioteca para entrada e saída (printf, fgets, etc)
#include <string.h>  // Biblioteca para manipular strings (strlen, strcpy, etc)
#include <ctype.h>   // Biblioteca para funções de caracteres (não usada muito aqui, mas útil)


// ========================
// DEFINIÇÕES E CONSTANTES
// ========================

#define MAX_NAME_SLOTS 5   // Máximo de nomes alternativos que um território pode ter
#define MAX_NAME_LEN 32    // Tamanho máximo de cada nome
#define BOARD_SLOTS 5      // Quantos territórios existem no tabuleiro
#define LINE_BUF 128       // Tamanho do buffer usado para leitura de linhas


// ========================
// ESTRUTURA DE DADOS
// ========================

// Estrutura que representa um território do jogo
typedef struct {
    char names[MAX_NAME_SLOTS][MAX_NAME_LEN]; // até 5 nomes possíveis para o território
    char color[16];                           // cor do exército (ex: "vermelho", "azul")
    int troops;                               // número de tropas naquele território
} Territory;


// ========================
// FUNÇÕES AUXILIARES
// ========================

// Remove o '\n' do final da string lida com fgets()
static void strip_newline(char *s) {
    size_t l = strlen(s);
    if (l > 0 && s[l - 1] == '\n') s[l - 1] = '\0';
}


// Inicializa um território vazio (zera tudo)
void init_empty_territory(Territory *t) {
    for (int i = 0; i < MAX_NAME_SLOTS; ++i)
        t->names[i][0] = '\0'; // zera cada nome
    t->color[0] = '\0';        // zera a cor
    t->troops = 0;             // começa com 0 tropas
}


// Retorna o "nome principal" do território (o primeiro nome não vazio)
const char* primary_name(const Territory *t) {
    for (int i = 0; i < MAX_NAME_SLOTS; ++i)
        if (t->names[i][0] != '\0') // se o nome não está vazio
            return t->names[i];
    return ""; // se não tiver nenhum nome, retorna vazio
}


// Imprime os dados de um território na tela
void print_territory(const Territory *t, int index) {
    printf("Espaço %d:\n", index + 1);
    for (int i = 0; i < MAX_NAME_SLOTS; ++i) {
        if (t->names[i][0] != '\0')
            printf("  Nome[%d]: %s\n", i + 1, t->names[i]);
    }
    printf("  Cor do exército: %s\n", t->color);
    printf("  Tropas: %d\n\n", t->troops);
}


// Imprime todos os territórios do tabuleiro
void print_board(const Territory board[], int size) {
    printf("\n=== Tabuleiro (%d espaços) ===\n\n", size);
    for (int i = 0; i < size; ++i)
        print_territory(&board[i], i);
}


// ========================
// FUNÇÃO PRINCIPAL (main)
// ========================

int main(void) {
    Territory board[BOARD_SLOTS];  // Cria um vetor de 5 territórios
    for (int i = 0; i < BOARD_SLOTS; ++i)
        init_empty_territory(&board[i]); // Inicializa cada um vazio

    char line[LINE_BUF]; // buffer para ler as entradas do usuário

    printf("========================================\n");
    printf("Vamos cadastrar os %d territorios iniciais do nosso mundo.\n\n", BOARD_SLOTS);


    // Loop para cadastrar os 5 territórios
    for (int i = 0; i < BOARD_SLOTS; ++i) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        // ====== NOME ======
        printf("Nome do Territorio: ");
        if (!fgets(line, sizeof(line), stdin)) return 1; // lê do teclado
        strip_newline(line); // remove o '\n'
        if (strlen(line) == 0) {
            strcpy(board[i].names[0], "SemNome"); // se o jogador não digitar nada
        } else {
            strncpy(board[i].names[0], line, MAX_NAME_LEN - 1);
            board[i].names[0][MAX_NAME_LEN - 1] = '\0';
        }

        // ====== COR ======
        printf("Cor do exército: ");
        if (!fgets(line, sizeof(line), stdin)) return 1;
        strip_newline(line);
        if (strlen(line) == 0)
            strcpy(board[i].color, "indefinida"); // cor padrão
        else {
            strncpy(board[i].color, line, sizeof(board[i].color) - 1);
            board[i].color[sizeof(board[i].color) - 1] = '\0';
        }

        // ====== TROPAS ======
        int troops = 0;
        while (1) {
            printf("Numero de tropas: ");
            if (!fgets(line, sizeof(line), stdin)) return 1;
            strip_newline(line);
            // tenta converter o texto em número inteiro
            if (sscanf(line, "%d", &troops) == 1 && troops >= 0) {
                board[i].troops = troops;
                break; // sai do loop se for válido
            } else {
                printf("  Entrada invalida. Digite um numero inteiro >= 0.\n");
            }
        }

        printf("\n");
    }


    // ====== MOSTRA TABULEIRO ======
    print_board(board, BOARD_SLOTS);


    // ====== MOVIMENTO DE EXEMPLO ======
    // move 3 tropas do 1º território para o 3º (só pra testar)
    if (board[0].troops >= 3) {
        board[0].troops -= 3;
        board[2].troops += 3;
        printf("Movidos 3 tropas de %s para %s.\n",
               primary_name(&board[0]),
               primary_name(&board[2]));
        print_board(board, BOARD_SLOTS);
    }

    return 0;
}
