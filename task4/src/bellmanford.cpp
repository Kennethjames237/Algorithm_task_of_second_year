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

// compilazione: g++ lezione17-grafi-dijkstra.cpp
//
// Obiettivo:
// 1) grafo con archi pesati
// 2) implementazione shortest path

#define INFTY 1000000

int ct_swap = 0;
int ct_cmp = 0;
int ct_op = 0; /// operazioni per la ricerca

int max_dim = 0;
int ntests = 1;
int ndiv = 1;
int details = 0;
int graph = 0;

/// file di output per grafo
ofstream output_graph;
int n_operazione = 0; /// contatore di operazioni per visualizzare i vari step

int ct_visit = 0; // contatore durante visita

//////////////////////////////////////////////////
/// Definizione della struttura dati lista
//////////////////////////////////////////////////

/// struct per il nodo della lista
typedef struct node {
    int val; /// prossimo nodo
    float w; /// peso dell'arco
    struct node* next;
} node_t;

/// struct per la lista
typedef struct list {
    node* head;
} list_t;


//////////////////////////////////////////////////
/// Fine Definizione della struttura dati lista
//////////////////////////////////////////////////

//////////////////////////////////////////////////
/// Definizione della struttura dati grafo
//////////////////////////////////////////////////

int* V;          // elenco dei nodi del grafo
int* V_visitato; // nodo visitato?
int* V_prev;     // nodo precedente dalla visita
float* V_dist;   // distanza da sorgente

// list_t* E;  /// array con le liste di adiacenza per ogni nodo
list_t** E; /// array di puntatori a le liste di adiacenza per ogni nodo
int n_nodi;
/////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////
/// Fine Definizione della struttura dati grafo
//////////////////////////////////////////////////

/// Questo e' un modo per stampare l'indirizzo node relativamente ad un altro di riferimento.
/// Permette di ottenere offset di piccola dimensione per essere facilmente visualizzati
/// Nota: il metodo non e' robusto e potrebbe avere comportamenti indesiderati su architetture diverse
/// L'alternativa corretta' e' utilizzare %p di printf: es. printf("%p\n",(void*) node);
/// con lo svantaggio di avere interi a 64 bit poco leggibili


list_t* global_ptr_ref = NULL; /// usato per memorizzare il puntatore alla prima lista allocata

int get_address(void* node) {
    return (int)((long)node - (long)global_ptr_ref);
}

void node_print(int n) {

    /// calcolo massima distanza (eccetto infinito)
    float max_d = 0;
    for (int i = 0; i < n_nodi; i++)
        if (max_d < V_dist[i] && V_dist[i] < INFTY)
            max_d = V_dist[i];

    output_graph << "node_" << n << "_" << n_operazione << endl;
    output_graph << "[ shape = oval; ";

    if (V_visitato[n] == 1)
        output_graph << "penwidth = 4; ";

    float col = V_dist[n] / max_d; /// distanza in scala 0..1
    output_graph << "fillcolor = \"0.0 0.0 " << col / 2 + 0.5 << "\"; style=filled; ";
    if (V_dist[n] < INFTY)
        output_graph << "label = "
        << "\"Idx: " << n << ", dist: " << V_dist[n] << "\" ];\n";
    else
        output_graph << "label = "
        << "\"Idx: " << n << ", dist: INF\" ];\n";

    node_t* elem = E[n]->head;
    while (elem != NULL) { /// disegno arco
        output_graph << "node_" << n << "_" << n_operazione << " -> ";
        output_graph << "node_" << elem->val << "_" << n_operazione << " [ label=\"" << elem->w << "\", len=" << elem->w / 100 * 10 << " ]\n";
        elem = elem->next;
    }

    if (V_prev[n] != -1) { // se c'e' un nodo precedente visitato -> disegno arco

        float len = 0;
        /*
        //cerco arco V_prev[n] --> V[n]
        node_t* elem=E[ V_prev[n] ]->head;
        while (elem!=NULL){
        int v=elem->val; /// arco u --> v
        if (v == V[n])
          len=elem->w;
        elem=elem->next;
          }
        */

        len = 1;
        output_graph << "node_" << n << "_" << n_operazione << " -> ";
        output_graph << "node_" << V_prev[n] << "_" << n_operazione << " [ color=blue, penwidth=5, len=" << len / 100 * 10 << " ]\n";
    }
}

void graph_print() {
    for (int i = 0; i < n_nodi; i++)
        node_print(i);
    n_operazione++;
}

void list_print(list_t* l) {
    printf("Stampa lista\n");

    if (l->head == NULL) {
        printf("Lista vuota\n");
    } else {
        node_t* current = l->head;

        while (current != NULL) {
            if (!details)
                printf("%d w:%f, ", current->val, current->w);
            else { /// stampa completa
                if (current->next == NULL)
                    printf("allocato in %d [Val: %d, W: %f, Next: NULL]\n",
                        get_address(current),
                        current->val,
                        current->w);
                else
                    printf("allocato in %d [Val: %d, W: %f, Next: %d]\n",
                        get_address(current),
                        current->val,
                        current->w,
                        get_address(current->next));
            }
            current = current->next;
        }
        printf("\n");
    }
}

list_t* list_new(void) {
    list_t* l = new list;
    if (details) {
        printf("Lista creata\n");
    }

    l->head = NULL; //// perche' non e' l.head ?
    if (details) {
        printf("Imposto a NULL head\n");
    }

    return l;
}

void list_insert_front(list_t* l, int elem, float w) {
    /// inserisce un elemento all'inizio della lista
    node_t* new_node = new node_t;
    new_node->next = NULL;

    new_node->val = elem;
    new_node->w = w;

    new_node->next = l->head;

    l->head = new_node;
}

void print_array(int* A, int dim) {
    for (int j = 0; j < dim; j++) {
        printf("%d ", A[j]);
    }
    printf("\n");
}

void print_array_graph(int* A, int n, string c, int a, int l, int m, int r) {
    /// prepara il disegno dell'array A ed il suo contenuto (n celle)
    /// a e' il codice del nodo e c la stringa
    /// l,m,r i tre indici della bisezione

    // return ;

    output_graph << c << a << " [label=<" << endl;

    /// tabella con contenuto array
    output_graph << "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" > " << endl;
    /// indici
    output_graph << "<TR  >";
    for (int j = 0; j < n; j++) {
        output_graph << "<TD ";
        output_graph << ">" << j << "</TD>" << endl;
    }
    output_graph << "</TR>" << endl;
    output_graph << "<TR>";
    // contenuto
    for (int j = 0; j < n; j++) {
        output_graph << "<TD BORDER=\"1\"";
        if (j == m)
            output_graph << " bgcolor=\"#00a000\""; /// valore testato
        else if (j >= l && j <= r)
            output_graph << " bgcolor=\"#80ff80\""; /// range di competenza
        output_graph << ">" << A[j] << "</TD>" << endl;
    }
    output_graph << "</TR>" << endl;
    output_graph << "</TABLE> " << endl;

    output_graph << ">];" << endl;
}

void resetAll() {
    for (int i = 0; i < n_nodi; i++) {
        V_visitato[i] = 0;
        V_prev[i] = -1;
        V_dist[i] = INFTY;
    }
}

bool bellman_ford(int n) {
    resetAll();
    V_dist[n] = 0;
    bool fixedPoint = false;
    for (int i = 0; i < n_nodi && !fixedPoint; ++i) {
        fixedPoint = true;
        for (int j = 0; j < n_nodi; ++j) {
            node_t* elem = E[j]->head;
            while (elem != nullptr && V_dist[j] != INFTY) {
                int v = elem->val;
                float alt = V_dist[j] + elem->w;
                if (alt < V_dist[v]) {
                    fixedPoint = false;
                    V_dist[v] = alt;
                    V_prev[v] = j;
                }
                elem = elem->next;
            }
        }
    }
    return fixedPoint;
}

void no_negative_cycle() {
    list_insert_front(E[0], 1, 8);
    list_insert_front(E[0], 2, 10);

    list_insert_front(E[1], 3, 1);
    list_insert_front(E[1], 8, 5);


    list_insert_front(E[2], 5, 2);

    list_insert_front(E[3], 5, -1);
    list_insert_front(E[3], 2, -4);

    list_insert_front(E[4], 2, 1);

    list_insert_front(E[5], 4, -2);

    list_insert_front(E[6], 4, 7);

    list_insert_front(E[7], 6, 9);
    list_insert_front(E[7], 4, 10);

    list_insert_front(E[8], 5, 5);

    list_insert_front(E[9], 4, 10);

}

void with_negative_cycle() {
    list_insert_front(E[0], 1, 8);
    list_insert_front(E[0], 2, 10);

    list_insert_front(E[1], 3, 1);
    list_insert_front(E[1], 8, 5);


    list_insert_front(E[2], 5, 2);

    list_insert_front(E[3], 5, -1);
    list_insert_front(E[3], 2, -4);

    list_insert_front(E[4], 2, 1);


    list_insert_front(E[5], 4, -2);

    list_insert_front(E[6], 9, -1);

    list_insert_front(E[7], 6, -1);

    list_insert_front(E[8], 5, 5);

    list_insert_front(E[4], 9, 10);
    list_insert_front(E[9], 7, -1);


}

void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
    /// aggiorno contatore globale di swap
    ct_swap++;
}

int parse_cmd(int argc, char** argv) {
    /// controllo argomenti
    int ok_parse = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][1] == 'v') {
            details = 1;
            ok_parse = 1;
        }
        if (argv[i][1] == 'g') {
            graph = 1;
            ok_parse = 1;
        }
    }

    if (argc > 1 && !ok_parse) {
        printf("Usage: %s [Options]\n", argv[0]);
        printf("Options:\n");
        printf("  -verbose: Abilita stampe durante l'esecuzione dell'algoritmo\n");
        printf("  -graph: creazione file di dot con il grafo dell'esecuzione (forza d=1 t=1)\n");
        return 1;
    }

    return 0;
}

int main(int argc, char** argv) {
    int i, test;

    // init random
    srand((unsigned)time(NULL));

    if (parse_cmd(argc, argv))
        return 1;

    if (graph) {
        output_graph.open("data/bellmanford.dot");
        /// preparo header
        output_graph << "digraph g" << endl;
        output_graph << "{ " << endl;
        output_graph << "node [shape=none]" << endl;
        output_graph << "rankdir=\"LR\"" << endl;
        ;
        //    output_graph << "edge[tailclip=false,arrowtail=dot];"<<endl;
    }



    n_nodi = 10;

    //// init nodi
    V = new int[n_nodi];
    V_visitato = new int[n_nodi];
    V_prev = new int[n_nodi];
    V_dist = new float[n_nodi];
    //// init archi
    E = new list_t * [n_nodi]; //(list_t**)malloc(n_nodi*sizeof(list_t*));

    // costruzione grafo
    for (int i = 0; i < n_nodi; i++) {
        V[i] = 2 * i;
        V_visitato[i] = 0; // flag = non visitato
        V_prev[i] = -1;    // non c'e' precedente
        V_dist[i] = INFTY; // infinito
        E[i] = list_new();
        if (i == 0)
            global_ptr_ref = E[i];
    }

    //no_negative_cycle();
    with_negative_cycle();

    graph_print();

    for (int i = 0; i < n_nodi; i++) {
        printf("Sono il nodo di indice %d nell'array\n", i);
        printf("Il valore del nodo e' %d\n", V[i]);
        printf("La lista di adiacenza e'\n");
        list_print(E[i]);

        if (!bellman_ford(0))
            cout << "Ciclo negativo" << endl;
        graph_print();

        if (graph) {
            /// preparo footer e chiudo file
            output_graph << "}" << endl;
            output_graph.close();
            cout << " File graph.dot scritto" << endl
                << "****** Creare il grafo con: neato graph.dot -Tpdf -o graph.pdf" << endl;
        }

        return 0;
    }
}