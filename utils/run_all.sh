#!/bin/bash

# Nome do arquivo onde os dados serão salvos
ARQUIVO_SAIDA="resultados_tme.csv"

# Cria o cabeçalho do arquivo CSV para abrir bonito no Excel
echo "ALGORITMO,NPROC,QUANTUM,RODADA,TME" > $ARQUIVO_SAIDA

# Definindo os parâmetros pedidos na atividade
# AJUSTE OS NOMES ABAIXO PARA OS EXECUTÁVEIS CORRETOS DO SEU GRUPO!
ALGORITMOS=("../main_fifo" "../main_ljf" "../main_prio_static" "../main_prio_dynamic_quantum")
QUANTUMS=(10 20 30)
NPROCS=(10 20 30 40 50 60 70 80 90 100)
RODADAS=30

echo "Iniciando as 4500 simulações..."

# 1º Loop: Varia os Algoritmos
for alg in "${ALGORITMOS[@]}"; do
    
    # 2º Loop: Varia os valores de Quantum
    for q in "${QUANTUMS[@]}"; do
        
        # 3º Loop: Varia a quantidade de Processos
        for n in "${NPROCS[@]}"; do
            
            # 4º Loop: Executa as 30 rodadas exigidas
            for r in $(seq 1 $RODADAS); do
                
                # Executa o simulador com os parâmetros da vez
                # O 'grep' pega a linha do TME e o 'awk' recorta apenas o número limpo
                VALOR_TME=$(./$alg -n $n -q $q | grep TME | awk '{print $2}')
                
                # Escreve a linha de resultado no arquivo CSV
                echo "$alg,$n,$q,$r,$VALOR_TME" >> $ARQUIVO_SAIDA
                
            done
            # Dá um feedback visual para você saber que não travou
            echo "Concluído -> Algoritmo: $alg | Quantum: $q | NPROC: $n"
        done
    done
done

echo "=== SUCESSO! Todas as simulações terminaram. ==="
echo "Resultados salvos em: $ARQUIVO_SAIDA"