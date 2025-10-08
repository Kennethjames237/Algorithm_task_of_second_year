//Nna minkousse Kenneth James
//Belli Andrea
//Premori Samuele

#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

// compilazione: g++ consegna1-loader.c
//

// Il programma carica il file data.txt contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0;

int max_dim = 0;
int ntests = 100;
int ndiv = 1;
int details = 0;
int graph = 0;

int n = 0; /// dimensione dell'array

void print_array(int* A, int dim) {
    for (int j = 0; j < dim; j++) {
        printf("%d ", A[j]);
    }
    printf("\n");
}

void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
    ct_swap++;
}

int partition(int* A, int p, int r) {

    ct_read++;
    int x = A[r];
    int i = p - 1;

    for (int j = p; j < r; j++) {
        ct_cmp++;
        ct_read++;
        if (A[j] <= x) {
            i++;
            ct_read++;
            ct_read++;
            swap(A[i], A[j]);
        }
    }
    ct_read++;
    ct_read++;
    swap(A[i + 1], A[r]);

    return i + 1;
}

void quick_sort(int* A, int p, int r) {
    if (p < r) {
        int q = partition(A, p, r);
        quick_sort(A, p, q - 1);
        quick_sort(A, q + 1, r);
    }
}

void insertion_sort(int* A, int p, int r) {
    for (int i = p + 1; i < r; ++i) {
        ct_read++;
        int key = A[i];
        int j = i - 1;
        while (j >= p && A[j] > key) {
            ct_read += 3;
            A[j + 1] = A[j];
            --j;
        }
        ct_read++;
        A[j + 1] = key;
    }
}

void merge(int* A, int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;

    // Arrays tempornei
    int* L = new int[n1];
    int* R = new int[n2];

    // Copia i dati negli array tempoarnei L e R
    for (int i = 0; i < n1; i++) {
        ct_read += 2; // Lettura da A[p+i] e L[i]
        L[i] = A[p + i];
    }
    for (int j = 0; j < n2; j++) {
        ct_read += 2; // Lettura da A[q+1+j] e R[j]
        R[j] = A[q + 1 + j];
    }

    // Merge dei due array temporanei in A
    int i = 0;
    int j = 0;
    int k = p;

    while (i < n1 && j < n2) {
        ct_read += 2;
        if (L[i] <= R[j]) {
            ct_read += 2; // Lettura da L[i] e A[k]
            A[k] = L[i];
            i++;
        } else {
            ct_read += 2; // Lettura da R[j] e A[k]
            A[k] = R[j];
            j++;
        }
        k++;
    }

    // Copia gli elementi rimanenti di L[], se ce ne sono
    while (i < n1) {
        ct_read += 2; // Lettura da L[i] e A[k]
        A[k] = L[i];
        i++;
        k++;
    }

    // Copia gli elementi rimanenti di R[], se ce ne sono
    while (j < n2) {
        ct_read += 2; // Lettura da R[j] e A[k]
        A[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

struct Bucket {
    int* values;
    int count;
    int capacity;
};

void bucket_sort(int* A, int p, int r) {
    if (p >= r) return;

    // Trova il valore minimo e massimo nell'intervallo [p, r]
    ct_read++; // Lettura di A[p]
    int min_val = A[p];
    int max_val = min_val;
    for (int i = p + 1; i < r; ++i) {
        ct_read += 2; // Lettura di A[i]
        if (A[i] < min_val) {
            ct_read++;
            min_val = A[i];
        }
        if (A[i] > max_val) {
            ct_read++;
            max_val = A[i];
        }
    }

    // Calcola il range per i bucket
    int range = max_val - min_val + 1;
    int bucket_size = 10;
    const int capacity = 50;
    int bucket_count = (range + bucket_size - 1) / bucket_size;

    
    // Inizializza i bucket
    Bucket* buckets = new Bucket[bucket_count];

    for (int i = 0; i < bucket_count; i++) {
        ct_read += 3;
        buckets[i].capacity = capacity;
        buckets[i].values = new int[capacity];
        buckets[i].count = 0;
    }

    // Riempie i bucket
    for (int i = p; i < r; i++) {
        ct_read++; // Lettura di A[i]
        int bucket_idx = (A[i] - min_val) / bucket_size; //tounderstand
        ct_read += 2;

        if (buckets[bucket_idx].count == buckets[bucket_idx].capacity) {
            ct_read++;
            int new_capacity = buckets[bucket_idx].capacity * 2;
            int* new_values = new int[new_capacity];
            ct_read++;
            // Copia i valori esistenti
            for (int j = 0; j < buckets[bucket_idx].count; j++) {
                ct_read += 3; 
                new_values[j] = buckets[bucket_idx].values[j];
            }

            // Libera la vecchia memoria e aggiorna il bucket
            ct_read += 3;
            delete[] buckets[bucket_idx].values;
            buckets[bucket_idx].values = new_values;
            buckets[bucket_idx].capacity = new_capacity;
        }

        // Aggiunge l'elemento al bucket
        ct_read += 2; // Rilettura di A[i] per metterlo nel bucket
        int val_idx = buckets[bucket_idx].count++;
        buckets[bucket_idx].values[val_idx] = A[i];
    }

    // Ordina ogni bucket con insertion sort e unisci i risultati
    int idx = p;
    for (int i = 0; i < bucket_count; i++) {
        // Ordina il bucket usando insertion sort
        ct_read++;
        insertion_sort(buckets[i].values, 0, buckets[i].count);

        // Copia gli elementi ordinati nel bucket di nuovo nell'array originale
        ct_read++;
        for (int j = 0; j < buckets[i].count; j++) {
            ct_read += 4; // Lettura di buckets[i].values[j]
            A[idx++] = buckets[i].values[j];
        }
    }

    // Libera la memoria allocata
    for (int i = 0; i < bucket_count; i++) {
        ct_read++;
        delete[] buckets[i].values;
    }
    delete[] buckets;
}

void sort(int* A, int n) {
    
    insertion_sort(A, 0, 250);

    bucket_sort(A, 250, n);

    //insertion_sort(A, 750, n);

    merge(A, 0, 249, n - 1);

   // merge(A, 0, 749, n - 1);
}

bool is_sorted(const int* A, const int n) {
    for (size_t i = 0; i < n - 1; ++i) {
        if (A[i] > A[i + 1]) return false;
    }
    return true;
}

int parse_cmd(int argc, char** argv) {

    /// parsing argomento
    max_dim = 1000;

    for (int i = 1; i < argc; i++) {
        if (argv[i][1] == 'd')
            ndiv = atoi(argv[i] + 3);
        if (argv[i][1] == 't')
            ntests = atoi(argv[i] + 3);
        if (argv[i][1] == 'v')
            details = 1;
        if (argv[i][1] == 'g') {
            graph = 1;
            ndiv = 1;
            ntests = 1;
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    int i, test;
    int* A;
    int* B; /// buffer per visualizzazione algoritmo

    if (parse_cmd(argc, argv))
        return 1;

    /// allocazione array
    A = new int[max_dim];

    n = max_dim;

    // srand((unsigned)time(NULL));
    // //creazione file input: NON USARE PIU' --> il file data.txt ufficiale è stato allegato, per permettere confronti equivalenti
    //  FILE *f = fopen("data.txt", "w+");
    //  int size=100;
    //  for (int j = 0; j < size; j++) {
    //      for (int i = 0; i < n; i++) {
    //          int v = 0;             
    //         //  v=(int)(100000*exp(-(0.0+i-n/2)*(0.0+i-n/2)/n/n*64));
    //          v=(int)(5000*(1+sin(3.1415*(i/(n/2.0)))));
    //          v+=rand()%(1+(int)(n*pow(((i+0.0)/n),2)));
    //          fprintf(f, "%d,", v);
    //      }
    //      fprintf(f, "\n");
    //  }
    //  fclose(f);

    ifstream input_data;
    input_data.open("data.txt");

    int read_min = -1;
    int read_max = -1;
    long read_avg = 0;

    bool all_sorted = true;

    //// lancio ntests volte per coprire diversi casi di input random
    for (test = 0; test < ntests; test++) {

        /// inizializzazione array: numeri random con range dimensione array
        for (i = 0; i < n; i++) {
            char comma;
            input_data >> A[i];
            input_data >> comma;
        }

        if (details) {
            printf("caricato array di dimensione %d\n", n);
            print_array(A, n);
        }

        ct_swap = 0;
        ct_cmp = 0;
        ct_read = 0;    

        sort(A, n);

        all_sorted = all_sorted && is_sorted(A, n);


        if (details) {
            printf("Output:\n");
            print_array(A, n);
        }

        /// statistiche
        read_avg += ct_read;
        if (read_min < 0 || read_min > ct_read)
            read_min = ct_read;
        if (read_max < 0 || read_max < ct_read)
            read_max = ct_read;
        printf("Test %d %d\n", test, ct_read);
    }
    printf("All sorted: %s\n", all_sorted ? "SI" : "NO");


    printf("N test: %d, Min: %d, Med: %.1f, Max: %d\n",
        ntests,
        read_min, (0.0 + read_avg) / ntests, read_max);

    delete[] A;

    return 0;
}
