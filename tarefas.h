#ifndef TAREFAS_H
#define TAREFAS_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_TASKS 100          // Define o numero maximo de tarefas
#define MAX_TITLE_LENGTH 100   // Define o comprimento maximo do titulo de uma tarefa
#define MAX_DATE_LENGTH 11     // Define o comprimento maximo da data (incluindo o terminador nulo)
#define MAX_TIME_LENGTH 6      // Define o comprimento maximo da hora (incluindo o terminador nulo)
#define FILENAME "tarefas.txt" // Nome do arquivo para armazenar as tarefas

// Estrutura que representa uma tarefa
typedef struct {
    char titulo[MAX_TITLE_LENGTH];
    int prioridade;
    int concluida;
    char data[MAX_DATE_LENGTH];
    char hora[MAX_TIME_LENGTH];
    char dataEntrega[MAX_DATE_LENGTH];
    char horaEntrega[MAX_TIME_LENGTH];
} Tarefa;

void salvarTarefas();
void carregarTarefas();
int validarData(const char* data);
int validarHora(const char* hora);
int normalizarData(const char* dataInput, char* dataNormalizada);
void adicionarTarefa();
const char* getPrioridadeString(int prioridade);
void listarTarefas();
void alterarStatusTarefa();
void editarTarefa();
void deletarTarefa();

#endif // TAREFAS_H
