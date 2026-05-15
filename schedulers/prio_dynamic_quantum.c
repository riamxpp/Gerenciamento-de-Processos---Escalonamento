#include <stdio.h>
#include <stdlib.h> // Necessário para usar a função rand() de probabilidade

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
extern int QUANTUM;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected;
    if (current != NULL)
    {
        if (current->process_time < (0.5 * QUANTUM)) {
            current->queue = 0; // Vai para a primeira fila (ready)
        } else {
            current->queue = 1; // Vai para a segunda fila (ready2)
        }
        switch (current->state) 
        {
            case READY:
                // --- CORREÇÃO 2: ENVIAR PARA A FILA CORRETA ---
                // O processo sofreu preempção. Ele deve ir para a fila correspondente
                // à etiqueta que acabou de receber no if/else acima.
                if (current->queue == 0) {
                    enqueue(ready, current);
                } else {
                    enqueue(ready2, current);
                }
                break;

            case BLOCKED:
                // Regra 2: Saiu por E/S -> Quando voltar, deve ir para a Fila 2 (ready2)
                enqueue(blocked, current); // Vai para a fila de BLOQUEADOS agora!
                break;

            case FINISHED:
                enqueue(finished, current);
                break;
            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    if (isempty(ready) && isempty(ready2))
    {
        return NULL;
    }

    // --- MUDANÇA: SORTEIO ENTRE AS FILAS ---
    
    // rand() % 100 gera um número aleatório de 0 a 99.
    int probabilidade = rand() % 100; 

    if (probabilidade < 70) // Números de 0 a 69 (70% de chance)
    {
        // Tenta pegar da Fila 1
        if (!isempty(ready)) {
            selected = dequeue(ready);
        } else {
            // Mas se a Fila 1 estiver vazia, pega da Fila 2 para não ficar ocioso
            selected = dequeue(ready2);
        }
    } 
    else // Números de 70 a 99 (30% de chance)
    {
        // Tenta pegar da Fila 2
        if (!isempty(ready2)) {
            selected = dequeue(ready2);
        } else {
            // Mas se a Fila 2 estiver vazia, pega da Fila 1
            selected = dequeue(ready);
        }
    }

    // Alterando o estado do processo selecionado para executando
    selected->state = RUNNING;
        
    // Retornando o processo selecionado
    return selected;
}



