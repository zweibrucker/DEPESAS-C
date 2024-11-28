#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 100

// estrutura da  Despesa
typedef struct Despesa {
    int id;
    char nome[MAX_STRING];
    float valor;
} Despesa;

// Definindo a estrutura do n� da �rvore bin�ria
typedef struct No {
    Despesa despesa;
    struct No* esq;
    struct No* dir;
} No;

// Fun��o para criar um novo n�
No* criarNo(int id, const char* nome, float valor) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->despesa.id = id;
    strcpy(novoNo->despesa.nome, nome);
    novoNo->despesa.valor = valor;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    return novoNo;
}

// Fun��o para inserir uma nova despesa na �rvore bin�ria
No* inserirDespesa(No* raiz, int id, const char* nome, float valor) {
    if (raiz == NULL) {
        return criarNo(id, nome, valor);
    }

    if (id < raiz->despesa.id) {
        raiz->esq = inserirDespesa(raiz->esq, id, nome, valor);
    } else if (id > raiz->despesa.id) {
        raiz->dir = inserirDespesa(raiz->dir, id, nome, valor);
    } else {
        printf("Erro: Despesa com ID %d j� existe.\n", id);
    }
    return raiz;
}

// Fun��o para buscar uma despesa pelo ID
No* buscarDespesa(No* raiz, int id) {
    if (raiz == NULL || raiz->despesa.id == id) {
        return raiz;
    }

    if (id < raiz->despesa.id) {
        return buscarDespesa(raiz->esq, id);
    } else {
        return buscarDespesa(raiz->dir, id);
    }
}

// Fun��o para exibir uma despesa (fun��o 2) aqui vai mostrar todas informa��es solicitadas na fun��o anterior 
void exibirDespesa(Despesa despesa) {
    printf("ID: %d\n", despesa.id);
    printf("Nome da despesa: %s\n", despesa.nome);
    printf("Valor: %.2f\n", despesa.valor);
}

// Fun��o para exibir todas as despesas em tabela e o valor total ao final
void exibirDespesas(No* raiz, int* primeiro, float* total) {
    if (raiz != NULL) {
        // fun��o para n�o ficar repetindo id, nome ....
        if (*primeiro) {
            printf("+------------+------------------------------+------------+\n");
            printf("| ID         | Nome                         | Valor      |\n");
            printf("+------------+------------------------------+------------+\n");
            *primeiro = 0;  // Marca que o cabe�alho j� foi exibido
        }
        
        exibirDespesas(raiz->esq, primeiro, total);  // Exibe a sub�rvore esquerda
        printf("| %-10d | %-28s | %-10.2f |\n", raiz->despesa.id, raiz->despesa.nome, raiz->despesa.valor);  // Exibe a despesa
        *total += raiz->despesa.valor;  // aqui ira mostra o valor total das somas das despesas
        exibirDespesas(raiz->dir, primeiro, total);  // Exibe a sub�rvore direita
    }
}

// Fun��o para procurar o n� com o menor valor (para exclus�o)
No* procurarMenor(No* raiz) {
    while (raiz->esq != NULL) {
        raiz = raiz->esq;
    }
    return raiz;
}

// Fun��o para excluir uma despesa pelo ID e retornar o nome da despesa exclu�da
No* excluirDespesa(No* raiz, int id, char* nomeExcluido) {
    if (raiz == NULL) {
        return raiz;
    }

    if (id < raiz->despesa.id) {
        raiz->esq = excluirDespesa(raiz->esq, id, nomeExcluido);
    } else if (id > raiz->despesa.id) {
        raiz->dir = excluirDespesa(raiz->dir, id, nomeExcluido);
    } else {
        // Caso o n� a ser exclu�do seja encontrado
        strcpy(nomeExcluido, raiz->despesa.nome);  // Armazena o nome da despesa exclu�da

        if (raiz->esq == NULL) {
            No* temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            No* temp = raiz->esq;
            free(raiz);
            return temp;
        }

        No* temp = procurarMenor(raiz->dir);
        raiz->despesa = temp->despesa;
        raiz->dir = excluirDespesa(raiz->dir, temp->despesa.id, nomeExcluido);
    }
    return raiz;
}

// Fun��o para salvar as despesas em um arquivo
void salvarDespesasEmArquivo(No* raiz, FILE* arquivo) {
    if (raiz != NULL) {
        salvarDespesasEmArquivo(raiz->esq, arquivo);
        fprintf(arquivo, "%d;%s;%.2f\n", raiz->despesa.id, raiz->despesa.nome, raiz->despesa.valor);
        salvarDespesasEmArquivo(raiz->dir, arquivo);
    }
}

// Fun��o para carregar as despesas de um arquivo
No* carregarDespesasDeArquivo(No* raiz) {
    FILE* arquivo = fopen("despesas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para carregar!\n");
        return raiz;
    }

    int id;
    char nome[MAX_STRING];
    float valor;

    while (fscanf(arquivo, "%d;%99[^;];%f\n", &id, nome, &valor) == 3) {
        raiz = inserirDespesa(raiz, id, nome, valor);
    }

    fclose(arquivo);
    return raiz;
}

// Fun��o para limpar a tela (apenas o conte�do abaixo do cabe�alho)
void limparTela() {
    system("cls"); 
}

// Fun��o principal
int main() {
    No* raiz = NULL;
    raiz = carregarDespesasDeArquivo(raiz);

    int opcao;
    do {
        limparTela();

        printf("Bem-vindo ao Controle de Despesas\n");
        printf("1. Cadastrar Despesa\n");
        printf("2. Buscar Despesa\n");
        printf("3. Exibir Despesas\n");
        printf("4. Excluir Despesa\n");
        printf("5. Salvar e Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Tente novamente.\n");
            while (getchar() != '\n'); 
            continue;
        }

        getchar();  // Limpar o buffer

        switch (opcao) {
            case 1: {
                int id;
                char nome[MAX_STRING];
                float valor;

                printf("Digite o ID da despesa: ");
                if (scanf("%d", &id) != 1) {
                    printf("Entrada invalida para o ID.\n");
                    getchar();
                    break;
                }

                getchar();  // Limpar o buffer

                printf("Digite o nome da despesa: ");
                fgets(nome, MAX_STRING, stdin);
                nome[strcspn(nome, "\n")] = '\0';  // Remover nova linha

                printf("Digite o valor da despesa: ");
                if (scanf("%f", &valor) != 1) {
                    printf("Entrada invalida para o valor.\n");
                    break;
                }

                raiz = inserirDespesa(raiz, id, nome, valor);
                printf("Despesa cadastrada com sucesso!\n");
                printf("\nPressione qualquer tecla para continuar...");
                getchar();  
                break;
            }
            case 2: {
                int id;
                printf("Digite o ID da despesa a ser buscada: ");
                
                if (scanf("%d", &id) != 1) {
                    printf("Entrada invalida para o ID.\n");
                    getchar();
                    break;
                }

                No* despesa = buscarDespesa(raiz, id);
                if (despesa != NULL) {
                    exibirDespesa(despesa->despesa);
                } else {
                    printf("Despesa com ID %d nao encontrada!\n", id);
                }

                printf("\nPressione qualquer tecla para continuar...");
                getchar();  
                getchar();  
                break;
            }
            case 3: {
                int primeiro = 1;  // Flag para controlar a exibi��o do cabe�alho (estava repetindo (solu��o)
                float total = 0.0f;  // Inicializa o valor total
                exibirDespesas(raiz, &primeiro, &total);
                printf("+------------+------------------------------+------------+\n");
                printf("Valor Total: %.2f\n", total);  // Exibe o valor total
                printf("\nPressione qualquer tecla para continuar...");
                getchar(); 
                break;
            }
            case 4: {
                int id;
                char nomeExcluido[MAX_STRING];  // Vari�vel para armazenar o nome da despesa exclu�da
                printf("Digite o ID da despesa a ser excluida: ");
                if (scanf("%d", &id) != 1) {
                    printf("Entrada invalida!\n");
                    while (getchar() != '\n');
                    break;
                }

                raiz = excluirDespesa(raiz, id, nomeExcluido);  // Passa a vari�vel nomeExcluido para armazenar o nome

                if (nomeExcluido[0] != '\0') {  // Verifica se a despesa foi exclu�da com sucesso
                    printf("Despesa '%s' excluida com sucesso!\n", nomeExcluido);
                } else {
                    printf("Despesa com ID %d n�o encontrada!\n", id);
                }

                printf("\nPressione qualquer tecla para continuar...");
                getchar();  
                getchar();  
                break;
            }
            case 5: {
                FILE* arquivo = fopen("despesas.txt", "w");
                if (arquivo == NULL) {
                    printf("Erro ao abrir o arquivo para salvar!\n");
                    break;
                }
                salvarDespesasEmArquivo(raiz, arquivo);
                fclose(arquivo);
                printf("Despesas salvas com sucesso!\n");
                printf("\nPressione qualquer tecla para continuar...");
                getchar();  
                break;
            }
            default:
                printf("Op��o invalida! Tente novamente.\n");
        }

    } while (opcao != 5);

    return 0;
}

