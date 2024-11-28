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

// Definindo a estrutura do nó da árvore binária
typedef struct No {
    Despesa despesa;
    struct No* esq;
    struct No* dir;
} No;

// Função para criar um novo nó
No* criarNo(int id, const char* nome, float valor) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->despesa.id = id;
    strcpy(novoNo->despesa.nome, nome);
    novoNo->despesa.valor = valor;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    return novoNo;
}

// Função para inserir uma nova despesa na árvore binária
No* inserirDespesa(No* raiz, int id, const char* nome, float valor) {
    if (raiz == NULL) {
        return criarNo(id, nome, valor);
    }

    if (id < raiz->despesa.id) {
        raiz->esq = inserirDespesa(raiz->esq, id, nome, valor);
    } else if (id > raiz->despesa.id) {
        raiz->dir = inserirDespesa(raiz->dir, id, nome, valor);
    } else {
        printf("Erro: Despesa com ID %d já existe.\n", id);
    }
    return raiz;
}

// Função para buscar uma despesa pelo ID
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

// Função para exibir uma despesa (função 2) aqui vai mostrar todas informações solicitadas na função anterior 
void exibirDespesa(Despesa despesa) {
    printf("ID: %d\n", despesa.id);
    printf("Nome da despesa: %s\n", despesa.nome);
    printf("Valor: %.2f\n", despesa.valor);
}

// Função para exibir todas as despesas em tabela e o valor total ao final
void exibirDespesas(No* raiz, int* primeiro, float* total) {
    if (raiz != NULL) {
        // função para não ficar repetindo id, nome ....
        if (*primeiro) {
            printf("+------------+------------------------------+------------+\n");
            printf("| ID         | Nome                         | Valor      |\n");
            printf("+------------+------------------------------+------------+\n");
            *primeiro = 0;  // Marca que o cabeçalho já foi exibido
        }
        
        exibirDespesas(raiz->esq, primeiro, total);  // Exibe a subárvore esquerda
        printf("| %-10d | %-28s | %-10.2f |\n", raiz->despesa.id, raiz->despesa.nome, raiz->despesa.valor);  // Exibe a despesa
        *total += raiz->despesa.valor;  // aqui ira mostra o valor total das somas das despesas
        exibirDespesas(raiz->dir, primeiro, total);  // Exibe a subárvore direita
    }
}

// Função para procurar o nó com o menor valor (para exclusão)
No* procurarMenor(No* raiz) {
    while (raiz->esq != NULL) {
        raiz = raiz->esq;
    }
    return raiz;
}

// Função para excluir uma despesa pelo ID e retornar o nome da despesa excluída
No* excluirDespesa(No* raiz, int id, char* nomeExcluido) {
    if (raiz == NULL) {
        return raiz;
    }

    if (id < raiz->despesa.id) {
        raiz->esq = excluirDespesa(raiz->esq, id, nomeExcluido);
    } else if (id > raiz->despesa.id) {
        raiz->dir = excluirDespesa(raiz->dir, id, nomeExcluido);
    } else {
        // Caso o nó a ser excluído seja encontrado
        strcpy(nomeExcluido, raiz->despesa.nome);  // Armazena o nome da despesa excluída

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

// Função para salvar as despesas em um arquivo
void salvarDespesasEmArquivo(No* raiz, FILE* arquivo) {
    if (raiz != NULL) {
        salvarDespesasEmArquivo(raiz->esq, arquivo);
        fprintf(arquivo, "%d;%s;%.2f\n", raiz->despesa.id, raiz->despesa.nome, raiz->despesa.valor);
        salvarDespesasEmArquivo(raiz->dir, arquivo);
    }
}

// Função para carregar as despesas de um arquivo
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

// Função para limpar a tela (apenas o conteúdo abaixo do cabeçalho)
void limparTela() {
    system("cls"); 
}

// Função principal
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
                int primeiro = 1;  // Flag para controlar a exibição do cabeçalho (estava repetindo (solução)
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
                char nomeExcluido[MAX_STRING];  // Variável para armazenar o nome da despesa excluída
                printf("Digite o ID da despesa a ser excluida: ");
                if (scanf("%d", &id) != 1) {
                    printf("Entrada invalida!\n");
                    while (getchar() != '\n');
                    break;
                }

                raiz = excluirDespesa(raiz, id, nomeExcluido);  // Passa a variável nomeExcluido para armazenar o nome

                if (nomeExcluido[0] != '\0') {  // Verifica se a despesa foi excluída com sucesso
                    printf("Despesa '%s' excluida com sucesso!\n", nomeExcluido);
                } else {
                    printf("Despesa com ID %d não encontrada!\n", id);
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
                printf("Opção invalida! Tente novamente.\n");
        }

    } while (opcao != 5);

    return 0;
}

