#include <stdio.h>

#include "queue.h" // contem funções uteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatisticas 
#include "utils.h" // possui funções uteis 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para
// as estatisticas finais

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
struct proc * selected = NULL; 

    /*
     * 1. Tratando o processo que está atualmente executando (current)
     */
    if (current != NULL)
    {
        switch (current->state) 
        {
            case READY:
                enqueue(ready, current);
                break;

            case BLOCKED:
                enqueue(blocked, current);
                break;

            case FINISHED:
                enqueue(finished, current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    /*
     * 2. Estratégia de seleção de um novo processo para executar (SJF)
     */

    // Se a fila de aptos está vazia, nao há o que escalonar
    if (isempty(ready))
    {
        return NULL;
    }

    // Variáveis auxiliares para percorrer a fila e achar o menor job
    struct proc * aux = ready->head;
    struct proc * shortest_job = aux;

    // Percorre a fila inteira para encontrar o processo com o menor remaining_time
    while (aux != NULL)
    {
        // Se o processo atual tiver um tempo restante menor que o nosso menor conhecido
        if (aux->remaining_time < shortest_job->remaining_time)
        {
            shortest_job = aux;
        }
        aux = aux->next; // Avança para o próximo da fila
    }

    // Remove especificamente o processo encontrado da fila usando o PID dele
    selected = dequeue_bypid(ready, shortest_job->pid);

    // Altera o estado do processo selecionado para executando
    selected->state = RUNNING;
        
    // Retorna o processo selecionado
    return selected;
}

