#include "tarefas.h"

Tarefa tarefas[MAX_TASKS];
int numTarefas = 0;

void salvarTarefas() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fprintf(file, "%d\n", numTarefas);
    for (int i = 0; i < numTarefas; i++) {
        fprintf(file, "%s;%d;%d;%s;%s;%s;%s\n", tarefas[i].titulo, tarefas[i].prioridade, tarefas[i].concluida, tarefas[i].data, tarefas[i].hora, tarefas[i].dataEntrega, tarefas[i].horaEntrega);
    }
    fclose(file);
}

void carregarTarefas() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return;
    }
    fscanf(file, "%d\n", &numTarefas);
    for (int i = 0; i < numTarefas; i++) {
        fscanf(file, "%99[^;];%d;%d;%10[^;];%5[^;];%10[^;];%5[^\n]\n", tarefas[i].titulo, &tarefas[i].prioridade, &tarefas[i].concluida, tarefas[i].data, tarefas[i].hora, tarefas[i].dataEntrega, tarefas[i].horaEntrega);
    }
    fclose(file);
}

int validarData(const char* data) {
    int dia, mes, ano;
    if (sscanf(data, "%02d/%02d/%04d", &dia, &mes, &ano) != 3) return 0;

    if (ano < 1900 || ano > 2100) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (dia < 1 || dia > 31) return 0;

    if (mes == 2) {
        if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
            if (dia > 29) return 0;
        } else {
            if (dia > 28) return 0;
        }
    }

    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        if (dia > 30) return 0;
    }

    return 1;
}

int validarHora(const char* hora) {
    if (strlen(hora) != 5) return 0;
    if (hora[2] != ':') return 0;

    int h, m;
    if (sscanf(hora, "%02d:%02d", &h, &m) != 2) return 0;

    if (h < 0 || h > 23) return 0;
    if (m < 0 || m > 59) return 0;

    return 1;
}

int normalizarData(const char* dataInput, char* dataNormalizada) {
    int dia, mes, ano;
    if (sscanf(dataInput, "%d-%d-%d", &dia, &mes, &ano) == 3 ||
        sscanf(dataInput, "%d/%d/%d", &dia, &mes, &ano) == 3 ||
        sscanf(dataInput, "%d%d%d", &dia, &mes, &ano) == 3) {
        sprintf(dataNormalizada, "%02d-%02d-%04d", dia, mes, ano);
        return 1;
    }
    return 0;
}

void adicionarTarefa() {
    if (numTarefas >= MAX_TASKS) {
        printf("\nLista de tarefas esta cheia!\n\n");
        return;
    }

    printf("Digite o titulo da tarefa: ");
    getchar();
    fgets(tarefas[numTarefas].titulo, MAX_TITLE_LENGTH, stdin);
    tarefas[numTarefas].titulo[strcspn(tarefas[numTarefas].titulo, "\n")] = 0;

    printf("Digite a prioridade da tarefa (1 - Baixa, 2 - Media, 3 - Alta): ");
    scanf("%d", &tarefas[numTarefas].prioridade);

    tarefas[numTarefas].concluida = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(tarefas[numTarefas].data, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    sprintf(tarefas[numTarefas].hora, "%02d:%02d", tm.tm_hour, tm.tm_min);

    char dataInput[MAX_DATE_LENGTH];
    char horaInput[MAX_TIME_LENGTH];

    int dataValida = 0;
    int horaValida = 0;

    while (!dataValida) {
        printf("Digite a data de entrega da tarefa (DD/MM/YYYY): ");
        getchar();  // Consumir o caractere de nova linha restante
        fgets(dataInput, MAX_DATE_LENGTH, stdin);
        dataInput[strcspn(dataInput, "\n")] = 0;

        if (validarData(dataInput)) {
            int diaCriacao, mesCriacao, anoCriacao;
            sscanf(tarefas[numTarefas].data, "%02d/%02d/%04d", &diaCriacao, &mesCriacao, &anoCriacao);

            int diaEntrega, mesEntrega, anoEntrega;
            sscanf(dataInput, "%02d/%02d/%04d", &diaEntrega, &mesEntrega, &anoEntrega);

            if (anoEntrega < anoCriacao || (anoEntrega == anoCriacao && mesEntrega < mesCriacao) || (anoEntrega == anoCriacao && mesEntrega == mesCriacao && diaEntrega < diaCriacao)) {
                printf("A data de entrega nao pode ser anterior a data de criacao da tarefa! Tente novamente.\n");
            } else {
                strcpy(tarefas[numTarefas].dataEntrega, dataInput);
                dataValida = 1;
            }
        } else {
            printf("Data de entrega invalida! Tente novamente.\n");
        }
    }

    while (!horaValida) {
        printf("Digite a hora de entrega da tarefa (HH:MM): ");
        fgets(horaInput, MAX_TIME_LENGTH, stdin);
        horaInput[strcspn(horaInput, "\n")] = 0;

        if (validarHora(horaInput)) {
            int diaCriacao, mesCriacao, anoCriacao;
            sscanf(tarefas[numTarefas].data, "%02d/%02d/%04d", &diaCriacao, &mesCriacao, &anoCriacao);

            int diaEntrega, mesEntrega, anoEntrega;
            sscanf(tarefas[numTarefas].dataEntrega, "%02d/%02d/%04d", &diaEntrega, &mesEntrega, &anoEntrega);

            int horaCriacao, minutoCriacao;
            sscanf(tarefas[numTarefas].hora, "%02d:%02d", &horaCriacao, &minutoCriacao);

            int horaEntrega, minutoEntrega;
            sscanf(horaInput, "%02d:%02d", &horaEntrega, &minutoEntrega);

            if (anoEntrega > anoCriacao || mesEntrega > mesCriacao || diaEntrega > diaCriacao || (horaEntrega > horaCriacao || (horaEntrega == horaCriacao && minutoEntrega >= minutoCriacao))) {
                strcpy(tarefas[numTarefas].horaEntrega, horaInput);
                horaValida = 1;
            } else {
                printf("A hora de entrega nao pode ser anterior a hora de criacao da tarefa se for no mesmo dia! Tente novamente.\n");
            }
        } else {
            printf("Hora de entrega invalida! Tente novamente.\n");
        }
    }

    numTarefas++;
    salvarTarefas();

    printf("\nTarefa adicionada com sucesso!\n");
}


const char* getPrioridadeString(int prioridade) {
    switch (prioridade) {
        case 1: return "Baixa";
        case 2: return "Media";
        case 3: return "Alta";
        default: return "Desconhecida";
    }
}

void listarTarefas() {
    if (numTarefas == 0) {
        printf("\nLista de tarefas esta vazia!\n\n");
        return;
    }

    printf("\nLista de Tarefas:\n");
    for (int i = 0; i < numTarefas; i++) {
        printf("Tarefa %d:\n", i + 1);
        printf("Titulo: %s\n", tarefas[i].titulo);
        printf("Prioridade: %s\n", getPrioridadeString(tarefas[i].prioridade));
        printf("Status: %s\n", tarefas[i].concluida ? "Concluida" : "Nao concluida");
        printf("Data de criacao: %s\n", tarefas[i].data);
        printf("Hora de criacao: %s\n", tarefas[i].hora);
        printf("Data de entrega: %s\n", tarefas[i].dataEntrega);
        printf("Hora de entrega: %s\n", tarefas[i].horaEntrega);
        printf("\n");
    }
}

void alterarStatusTarefa() {
    if (numTarefas == 0) {
        printf("\nLista de tarefas esta vazia!\n\n");
        return;
    }

    int index;
    printf("Digite o numero da tarefa para alterar o status: ");
    scanf("%d", &index);
    index--;

    if (index < 0 || index >= numTarefas) {
        printf("Numero de tarefa invalido!\n");
        return;
    }

    tarefas[index].concluida = !tarefas[index].concluida;
    salvarTarefas();

    printf("\nStatus da tarefa %d alterado com sucesso!\n", index + 1);
}

// Funcao para editar uma tarefa existente, incluindo a data e hora de entrega
void editarTarefa() {
    if (numTarefas == 0) {
        printf("\nLista de tarefas esta vazia!\n\n");
        return;
    }

    int numTarefa;
    printf("Digite o numero da tarefa que deseja editar: ");
    scanf("%d", &numTarefa);

    if (numTarefa >= 1 && numTarefa <= numTarefas) {
        printf("Digite o novo titulo da tarefa: ");
        getchar();
        fgets(tarefas[numTarefa - 1].titulo, MAX_TITLE_LENGTH, stdin);
        tarefas[numTarefa - 1].titulo[strcspn(tarefas[numTarefa - 1].titulo, "\n")] = 0;

        printf("Digite a nova prioridade da tarefa (1 - Baixa, 2 - Media, 3 - Alta): ");
        scanf("%d", &tarefas[numTarefa - 1].prioridade);

        // Oferece a opção de alterar a data e a hora de entrega da tarefa
        char opcao;
        printf("Deseja alterar a data e hora de entrega da tarefa? (S/N): ");
        scanf(" %c", &opcao);
        if (toupper(opcao) == 'S') {
            char dataInput[MAX_DATE_LENGTH];
            char horaInput[MAX_TIME_LENGTH];

            do {
                printf("Digite a nova data de entrega da tarefa (DD/MM/YYYY): ");
                getchar();
                fgets(dataInput, MAX_DATE_LENGTH, stdin);
                dataInput[strcspn(dataInput, "\n")] = 0;

                if (validarData(dataInput)) {
                    strcpy(tarefas[numTarefa - 1].dataEntrega, dataInput);
                    break;
                } else {
                    printf("Data de entrega invalida! Tente novamente.\n");
                }
            } while (1);

            do {
                printf("Digite a nova hora de entrega da tarefa (HH:MM): ");
                fgets(horaInput, MAX_TIME_LENGTH, stdin);
                horaInput[strcspn(horaInput, "\n")] = 0;

                if (validarHora(horaInput)) {
                    strcpy(tarefas[numTarefa - 1].horaEntrega, horaInput);
                    break;
                } else {
                    printf("Hora de entrega invalida! Tente novamente.\n");
                }
            } while (1);
        }

        printf("\nTarefa editada com sucesso!\n");
        salvarTarefas();
    } else {
        printf("\nNumero de tarefa invalido!\n");
    }
}

void deletarTarefa() {
    if (numTarefas == 0) {
        printf("\nLista de tarefas esta vazia!\n\n");
        return;
    }

    int index;
    printf("Digite o numero da tarefa para deletar: ");
    scanf("%d", &index);
    index--;

    if (index < 0 || index >= numTarefas) {
        printf("Numero de tarefa invalido!\n");
        return;
    }

    for (int i = index; i < numTarefas - 1; i++) {
        tarefas[i] = tarefas[i + 1];
    }
    numTarefas--;
    salvarTarefas();

    printf("\nTarefa %d deletada com sucesso!\n", index + 1);
}
