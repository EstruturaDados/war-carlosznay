#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 32
#define MAX_COLOR_LEN 16
#define BOARD_SLOTS 5

typedef struct {
    char nome[MAX_NAME_LEN];
    char cor[MAX_COLOR_LEN];
    int tropas;
} Territorio;

void cadastrarTerritorios(Territorio *mapa, int tamanho) {
    char linha[128];

    printf("========================================\n");
    printf("Vamos cadastrar os %d territorios iniciais do nosso mundo.\n\n", tamanho);

    for (int i = 0; i < tamanho; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        // ====== NOME ======
        printf("Nome do Territorio: ");
        fgets(linha, sizeof(linha), stdin);
        linha[strcspn(linha, "\n")] = '\0';  // remove o \n
        if (strlen(linha) == 0)
            strcpy(mapa[i].nome, "SemNome");
        else
            strcpy(mapa[i].nome, linha);

        // ====== COR ======
        printf("Cor do exército: ");
        fgets(linha, sizeof(linha), stdin);
        linha[strcspn(linha, "\n")] = '\0';
        if (strlen(linha) == 0)
            strcpy(mapa[i].cor, "indefinida");
        else
            strcpy(mapa[i].cor, linha);

        // ====== TROPAS ======
        int tropas = 0;
        while (1) {
            printf("Número de tropas: ");
            fgets(linha, sizeof(linha), stdin);
            linha[strcspn(linha, "\n")] = '\0';
            if (sscanf(linha, "%d", &tropas) == 1 && tropas >= 0) {
                mapa[i].tropas = tropas;
                break;
            } else {
                printf("  Entrada inválida. Digite um número inteiro >= 0.\n");
            }
        }

        printf("\n");
    }
}

void mostrarMapa(Territorio *mapa, int tamanho) {
    printf("\n=== MAPA DE TERRITÓRIOS ===\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d) %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

int main() {
    Territorio mapa[BOARD_SLOTS];

    cadastrarTerritorios(mapa, BOARD_SLOTS);
    mostrarMapa(mapa, BOARD_SLOTS);

    return 0;
}
