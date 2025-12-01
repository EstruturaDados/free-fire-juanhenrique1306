#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    char nome[50];
    char tipo[30];
    int quantidade;
    int prioridade; 
} Item;


typedef enum {
    POR_NOME,
    POR_TIPO,
    POR_PRIORIDADE
} CriterioOrdenacao;

Item mochila[10];
int numItens = 0;
int comparacoes = 0;
bool ordenadaPorNome = false;

void limparTela();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(CriterioOrdenacao criterio);
void buscaBinariaPorNome();
void exibirItem(Item item);

// Função principal
int main() {
    int opcao;
    
    do {
        limparTela();
        exibirMenu();
        
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); 
        
        switch(opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("\nSaindo do programa... Ate a proxima missao!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
        
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (opcao != 0);
    
    return 0;
}

void limparTela() {
    for (int i = 0; i < 50; i++) {
        printf("\n");
    }
}

void exibirMenu() {
    printf("========================================\n");
    printf("   CODIGO DA ILHA - EDICAO FREE FIRE    \n");
    printf("========================================\n");
    printf("Nivel: MESTRE - Ordenacao e Busca Binaria\n");
    printf("========================================\n");
    printf("Itens na mochila: %d/10\n", numItens);
    printf("Status ordenacao: %s\n", ordenadaPorNome ? "ORDENADA POR NOME" : "NAO ORDENADA");
    printf("========================================\n");
    printf("[1] Adicionar um item\n");
    printf("[2] Remover um item\n");
    printf("[3] Listar todos os itens\n");
    printf("[4] Ordenar itens por criterio\n");
    printf("[5] Busca binaria por nome\n");
    printf("[0] Sair\n");
    printf("========================================\n");
}

void exibirItem(Item item) {
    printf("| %-20s | %-15s | %-10d | %-10d |\n", 
           item.nome, item.tipo, item.quantidade, item.prioridade);
}

void inserirItem() {
    if (numItens >= 10) {
        printf("\nERRO: Mochila cheia! Maximo 10 itens.\n");
        return;
    }
    
    Item novoItem;
    
    printf("\n=== ADICIONAR NOVO ITEM ===\n");
    
    printf("Nome do item: ");
    fgets(novoItem.nome, sizeof(novoItem.nome), stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0';
    
    printf("Tipo (arma, municao, cura, equipamento, etc.): ");
    fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';
    
    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    
    do {
        printf("Prioridade (1 a 5, onde 1 = mais importante): ");
        scanf("%d", &novoItem.prioridade);
        if (novoItem.prioridade < 1 || novoItem.prioridade > 5) {
            printf("Prioridade deve ser entre 1 e 5!\n");
        }
    } while (novoItem.prioridade < 1 || novoItem.prioridade > 5);
    
    mochila[numItens] = novoItem;
    numItens++;
    
    if (ordenadaPorNome) {
        ordenadaPorNome = false;
    }
    
    printf("\nItem '%s' adicionado com sucesso!\n", novoItem.nome);
}

void removerItem() {
    if (numItens == 0) {
        printf("\nERRO: Mochila vazia! Nenhum item para remover.\n");
        return;
    }
    
    char nomeBusca[50];
    printf("\n=== REMOVER ITEM ===\n");
    printf("Nome do item a remover: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    int posicao = -1;
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            posicao = i;
            break;
        }
    }
    
    if (posicao == -1) {
        printf("\nItem '%s' nao encontrado na mochila.\n", nomeBusca);
        return;
    }
    
    printf("\nItem encontrado e removido:\n");
    exibirItem(mochila[posicao]);
    
    for (int i = posicao; i < numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    
    numItens--;
    
    if (ordenadaPorNome) {
        ordenadaPorNome = false;
    }
    
    printf("\nItem removido com sucesso! Itens restantes: %d\n", numItens);
}

void listarItens() {
    if (numItens == 0) {
        printf("\nMochila vazia! Nenhum item para mostrar.\n");
        return;
    }
    
    printf("\n=== ITENS NA MOCHILA (%d/10) ===\n", numItens);
    printf("+----------------------+-----------------+------------+------------+\n");
    printf("| Nome                 | Tipo            | Quantidade | Prioridade |\n");
    printf("+----------------------+-----------------+------------+------------+\n");
    
    for (int i = 0; i < numItens; i++) {
        exibirItem(mochila[i]);
    }
    
    printf("+----------------------+-----------------+------------+------------+\n");
    
    int totalItens = 0;
    int maiorPrioridade = 5;
    int menorPrioridade = 1;
    
    for (int i = 0; i < numItens; i++) {
        totalItens += mochila[i].quantidade;
        if (mochila[i].prioridade < maiorPrioridade) {
            maiorPrioridade = mochila[i].prioridade;
        }
        if (mochila[i].prioridade > menorPrioridade) {
            menorPrioridade = mochila[i].prioridade;
        }
    }
    
    printf("\nESTATISTICAS:\n");
    printf("- Total de unidades: %d\n", totalItens);
    printf("- Maior prioridade encontrada: %d\n", maiorPrioridade);
    printf("- Menor prioridade encontrada: %d\n", menorPrioridade);
}

void insertionSort(CriterioOrdenacao criterio) {
    comparacoes = 0;
    
    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;
        
        while (j >= 0) {
            comparacoes++;
            int comparacao = 0;
            
            switch(criterio) {
                case POR_NOME:
                    comparacao = strcmp(mochila[j].nome, chave.nome);
                    break;
                case POR_TIPO:
                    comparacao = strcmp(mochila[j].tipo, chave.tipo);
                    break;
                case POR_PRIORIDADE:
                    comparacao = mochila[j].prioridade - chave.prioridade;
                    break;
            }
            
            if (criterio == POR_PRIORIDADE) {
                if (comparacao > 0) {
                    mochila[j + 1] = mochila[j];
                    j--;
                } else {
                    break;
                }
            } else {
                if (comparacao > 0) {
                    mochila[j + 1] = mochila[j];
                    j--;
                } else {
                    break;
                }
            }
        }
        mochila[j + 1] = chave;
    }
    
    if (criterio == POR_NOME) {
        ordenadaPorNome = true;
    } else {
        ordenadaPorNome = false;
    }
}

void menuDeOrdenacao() {
    if (numItens < 2) {
        printf("\nERRO: E necessario pelo menos 2 itens para ordenar.\n");
        return;
    }
    
    int opcao;
    CriterioOrdenacao criterio;
    
    printf("\n=== ORDENAR ITENS ===\n");
    printf("Escolha o criterio de ordenacao:\n");
    printf("[1] Por nome (ordem alfabetica)\n");
    printf("[2] Por tipo (ordem alfabetica)\n");
    printf("[3] Por prioridade (1 a 5, 1 = mais importante)\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    
    switch(opcao) {
        case 1:
            criterio = POR_NOME;
            printf("\nOrdenando por nome...\n");
            break;
        case 2:
            criterio = POR_TIPO;
            printf("\nOrdenando por tipo...\n");
            break;
        case 3:
            criterio = POR_PRIORIDADE;
            printf("\nOrdenando por prioridade...\n");
            break;
        default:
            printf("\nOpcao invalida!\n");
            return;
    }
    
    insertionSort(criterio);
    
    printf("\nOrdenacao concluida!\n");
    printf("Numero de comparacoes realizadas: %d\n", comparacoes);
    
    printf("\nLista ordenada:\n");
    printf("+----------------------+-----------------+------------+------------+\n");
    printf("| Nome                 | Tipo            | Quantidade | Prioridade |\n");
    printf("+----------------------+-----------------+------------+------------+\n");
    
    for (int i = 0; i < numItens; i++) {
        exibirItem(mochila[i]);
    }
    
    printf("+----------------------+-----------------+------------+------------+\n");
    
    if (criterio == POR_NOME) {
        printf("\n✅ A mochila agora esta ordenada por nome.\n");
        printf("Voce pode usar a busca binaria!\n");
    }
}

void buscaBinariaPorNome() {
    if (numItens == 0) {
        printf("\nERRO: Mochila vazia! Nenhum item para buscar.\n");
        return;
    }
    
    if (!ordenadaPorNome) {
        printf("\n⚠️ ATENCAO: A mochila nao esta ordenada por nome!\n");
        printf("A busca binaria requer ordenacao por nome.\n");
        printf("Deseja ordenar a mochila por nome agora? (s/n): ");
        
        char resposta;
        scanf(" %c", &resposta);
        getchar();
        
        if (tolower(resposta) == 's') {
            insertionSort(POR_NOME);
            printf("Mochila ordenada por nome com %d comparacoes.\n", comparacoes);
        } else {
            printf("Busca binaria cancelada. Use a opcao 4 para ordenar.\n");
            return;
        }
    }
    
    char nomeBusca[50];
    printf("\n=== BUSCA BINARIA POR NOME ===\n");
    printf("Digite o nome do item: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    int inicio = 0;
    int fim = numItens - 1;
    int meio;
    int encontrado = -1;
    int comparacoesBusca = 0;
    
    printf("\nIniciando busca binaria...\n");
    
    while (inicio <= fim) {
        comparacoesBusca++;
        meio = (inicio + fim) / 2;
        
        printf("Comparacao %d: verificando '%s' na posicao %d\n", 
               comparacoesBusca, mochila[meio].nome, meio);
        
        int resultado = strcmp(mochila[meio].nome, nomeBusca);
        
        if (resultado == 0) {
            encontrado = meio;
            break;
        } else if (resultado < 0) {
            inicio = meio + 1;
            printf("  -> '%s' esta alfabeticamente depois de '%s'\n", 
                   nomeBusca, mochila[meio].nome);
        } else {
            fim = meio - 1;
            printf("  -> '%s' esta alfabeticamente antes de '%s'\n", 
                   nomeBusca, mochila[meio].nome);
        }
    }
    
    printf("\n========================================\n");
    printf("RESULTADO DA BUSCA BINARIA\n");
    printf("========================================\n");
    
    if (encontrado != -1) {
        printf("✅ ITEM ENCONTRADO!\n");
        printf("Posicao na mochila: %d\n", encontrado);
        printf("Detalhes do item:\n");
        printf("+----------------------+-----------------+------------+------------+\n");
        printf("| Nome                 | Tipo            | Quantidade | Prioridade |\n");
        printf("+----------------------+-----------------+------------+------------+\n");
        exibirItem(mochila[encontrado]);
        printf("+----------------------+-----------------+------------+------------+\n");
    } else {
        printf("❌ ITEM NAO ENCONTRADO!\n");
        printf("O item '%s' nao esta na mochila.\n", nomeBusca);
    }
    
    printf("Comparacoes realizadas na busca: %d\n", comparacoesBusca);
    printf("========================================\n");
}