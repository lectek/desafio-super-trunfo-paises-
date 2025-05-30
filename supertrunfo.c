#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_CARDS 20         // número máximo de cartas no baralho
#define MAX_NAME 50          // comprimento máximo de nomes (jogadores e países)

// definição de uma carta com seus atributos
typedef struct {
    char nome[MAX_NAME];     // nome do país
    int populacao;           // população
    float pib;               // Produto Interno Bruto
    float idh;               // Índice de Desenvolvimento Humano
} Carta;

// baralhos dos dois jogadores e contadores
Carta deck[MAX_CARDS];
int totalCards = 0;

Carta p1Deck[MAX_CARDS];
int p1Count = 0;

Carta p2Deck[MAX_CARDS];
int p2Count = 0;

// função para embaralhar o deck usando Fisher–Yates
void shuffleDeck() {
    srand((unsigned) time(NULL));
    for(int i = totalCards - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Carta tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
}

// adiciona carta ao final do baralho do jogador (enqueue)
void enqueue(Carta deckArr[], int *count, Carta card) {
    deckArr[*count] = card;
    (*count)++;
}

// retira a primeira carta do baralho do jogador (dequeue), deslocando o restante
Carta dequeue(Carta deckArr[], int *count) {
    Carta first = deckArr[0];
    for(int i = 1; i < *count; i++) {
        deckArr[i - 1] = deckArr[i];
    }
    (*count)--;
    return first;
}

// lê string do teclado sem incluir o '\n'
void readString(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int main() {
    char nome1[MAX_NAME], nome2[MAX_NAME];
    int n;

    // boas-vindas e leitura de nomes dos jogadores
    printf("=== Super Trunfo: LekTecGames ===\n\n");
    printf("Jogador 1, digite seu nome: ");
    readString(nome1, MAX_NAME);
    printf("Jogador 2, digite seu nome: ");
    readString(nome2, MAX_NAME);

    // definição do número de cartas (par para divisão)
    do {
        printf("\nQuantas cartas deseja criar? (max %d, par): ", MAX_CARDS);
        scanf("%d", &n);
        getchar();  // consome '\n'
    } while(n <= 0 || n > MAX_CARDS || n % 2 != 0);

    totalCards = n;

    // cadastro das cartas pelo usuário
    for(int i = 0; i < totalCards; i++) {
        printf("\nCarta %d/%d\n", i + 1, totalCards);
        printf("Nome do país: ");
        readString(deck[i].nome, MAX_NAME);
        printf("População (int): ");
        scanf("%d", &deck[i].populacao);
        printf("PIB (float): ");
        scanf("%f", &deck[i].pib);
        printf("IDH (float): ");
        scanf("%f", &deck[i].idh);
        getchar();  // consome '\n'
    }

    // embaralha e divide as cartas entre os dois jogadores
    shuffleDeck();
    for(int i = 0; i < totalCards; i++) {
        if(i % 2 == 0) enqueue(p1Deck, &p1Count, deck[i]);
        else enqueue(p2Deck, &p2Count, deck[i]);
    }

    int vez = 1;  // 1 = jogador1, 2 = jogador2
    printf("\n--- Iniciando o jogo! ---\n");

    // loop principal do jogo
    while(p1Count > 0 && p2Count > 0) {
        Carta c1 = dequeue(p1Deck, &p1Count);
        Carta c2 = dequeue(p2Deck, &p2Count);

        // exibe cartas na vez do jogador atual
        if(vez == 1) {
            printf("\nVez de %s!\n", nome1);
            printf("Sua carta: %s\n", c1.nome);
        } else {
            printf("\nVez de %s!\n", nome2);
            printf("Sua carta: %s\n", c2.nome);
        }

        // escolha de atributo
        printf("Escolha o atributo para comparar:\n");
        printf("1 - População (%d vs %d)\n", c1.populacao, c2.populacao);
        printf("2 - PIB (%.2f vs %.2f)\n", c1.pib, c2.pib);
        printf("3 - IDH (%.3f vs %.3f)\n", c1.idh, c2.idh);

        int attr;
        do {
            printf("Opção: ");
            scanf("%d", &attr);
        } while(attr < 1 || attr > 3);

        // comparação dos valores
        float v1 = 0, v2 = 0;
        switch(attr) {
            case 1: v1 = c1.populacao; v2 = c2.populacao; break;
            case 2: v1 = c1.pib;       v2 = c2.pib;       break;
            case 3: v1 = c1.idh;       v2 = c2.idh;       break;
        }

        // determina vencedor do turno
        if(v1 > v2) {
            printf("%s vence esta rodada!\n", nome1);
            enqueue(p1Deck, &p1Count, c1);
            enqueue(p1Deck, &p1Count, c2);
            vez = 1;
        }
        else if(v2 > v1) {
            printf("%s vence esta rodada!\n", nome2);
            enqueue(p2Deck, &p2Count, c1);
            enqueue(p2Deck, &p2Count, c2);
            vez = 2;
        }
        else {
            // empate: cada um recupera sua carta
            printf("Empate! Cada jogador recupera sua carta.\n");
            enqueue(p1Deck, &p1Count, c1);
            enqueue(p2Deck, &p2Count, c2);
        }

        printf("Placar: %s [%d] x [%d] %s\n",
               nome1, p1Count, p2Count, nome2);
    }

    // fim de jogo e anúncio do campeão
    if(p1Count == 0) {
        printf("\n%s fica sem cartas. %s é o campeão!\n", nome1, nome2);
    } else {
        printf("\n%s fica sem cartas. %s é o campeão!\n", nome2, nome1);
    }

    return 0;
}
