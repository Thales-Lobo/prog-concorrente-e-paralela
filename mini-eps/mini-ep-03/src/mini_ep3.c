#include <stdio.h>
#include <stdlib.h>

// Tamanho máximo para memoization de Fibonacci (ajuste conforme necessário)
#define MAX_FIB 100000

// Tabela global para memoization de Fibonacci (evita realocação em cada chamada)
long long fib_memo[MAX_FIB];

// Função auxiliar para trocar dois elementos de posição em um array
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Implementação do algoritmo Quick Sort (mantida por ser eficiente)
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[high]);

        int pi = (i + 1);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Implementação do cálculo de Fibonacci com memoization (usando tabela global)
long long fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    if (fib_memo[n] != -1) {
        return fib_memo[n];
    }
    fib_memo[n] = fibonacci(n - 1) + fibonacci(n - 2);
    return fib_memo[n];
}

int main() {
    const int TAMANHO = 200;
    const int SEED = 42;
    int vetor[TAMANHO];
    long long soma = 0;

    srand(SEED);

    // Gera números aleatórios dentro do intervalo desejado (evita overflow)
    for (int i = 0; i < TAMANHO; i++) {
        vetor[i] = (rand() % ((1 << 5) - (1 << 2) + 1)) + (1 << 2); 
    }

    quicksort(vetor, 0, TAMANHO - 1);

    // Inicializa a tabela de memoization de Fibonacci
    for (int i = 0; i < MAX_FIB; i++) {
        fib_memo[i] = -1;
    }

    // Pré-calcula Fibonacci para todos os números pares até o maior valor no vetor
    // Evita recalcular Fibonacci várias vezes dentro do loop principal
    int max_valor = vetor[TAMANHO - 1]; 
    for (int i = 2; i <= max_valor; i += 2) {
        fibonacci(i); 
    }

    // Calcula a soma dos números de Fibonacci pares (acessando diretamente a tabela)
    for (int i = 0; i < TAMANHO; i++) {
        soma += fib_memo[vetor[i]]; 
    }

    printf("Soma dos números de Fibonacci pares: %lld\n", soma);

    return 0;
}