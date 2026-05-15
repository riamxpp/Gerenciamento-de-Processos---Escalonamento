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

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;
extern int QUANTUM;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected; 

    /*
     * 1. Tratando o processo que está atualmente executando
     * (EXATAMENTE IGUAL AO FIFO)
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
     * 2. Estratégia de seleção de um novo processo (LJF)
     */

    // Se a fila de aptos está vazia, nao há o que fazer
    if (isempty(ready))
    {
        return NULL;
    }

    // --- MUDANÇA PARA O LJF COMEÇA AQUI ---

    int maior_tempo = -1;
    int pid_escolhido = -1;

    // Precisamos percorrer a fila 'ready' para encontrar o maior remaining_time.
    // NOTA: Como não tenho o arquivo queue.h, estou assumindo que a fila aponta 
    // para um 'first' ou 'head' e os processos usam 'next'.
    struct proc * p = ready->head; 

    while (p != NULL) 
    {
        // Se o tempo restante desse processo for maior que o nosso 'maior_tempo' atual,
        // atualizamos as nossas variáveis com os dados dele.
        if (p->remaining_time > maior_tempo) 
        {
            maior_tempo = p->remaining_time;
            pid_escolhido = p->pid;
        }
        
        // Pula para o próximo processo da fila para continuar a verificação
        p = p->next; 
    }

    // Agora que sabemos exatamente qual PID tem o maior tempo restante,
    // usamos a função disponibilizada pelo professor para tirá-lo do meio da fila
    selected = dequeue_bypid(ready, pid_escolhido);

    // --- MUDANÇA PARA O LJF TERMINA AQUI ---

    // Alterando o estado do processo selecionado para executando
    selected->state = RUNNING;
        
    // Retornando o processo selecionado
    return selected;
}

