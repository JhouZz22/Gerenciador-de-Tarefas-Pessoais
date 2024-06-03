#include "tarefas.h"

int main() {
    carregarTarefas();

    int opcao;
    do {
        printf("Menu de opcoes:\n");
        printf("1. Adicionar Tarefa\n");
        printf("2. Listar Tarefas\n");
        printf("3. Alterar Status de Tarefa\n");
        printf("4. Editar Tarefa\n");
        printf("5. Deletar Tarefa\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarTarefa();
                break;
            case 2:
                listarTarefas();
                break;
            case 3:
                alterarStatusTarefa();
                break;
            case 4:
                editarTarefa();
                break;
            case 5:
                deletarTarefa();
                break;
            case 6:
                salvarTarefas();
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 6);

    return 0;
}
