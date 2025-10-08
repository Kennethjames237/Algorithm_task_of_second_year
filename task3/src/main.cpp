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

// compilazione: g++ lezione13-grafi-DFS.cpp
//
// Obiettivo:
// 1) rappresentazione grafo con lista di adiacenza
// 2) visita recursive (DFS)
int max_DFS_cycle = 0;
int passo = 0;
int ct_swap = 0;
int ct_cmp = 0;
int ct_op = 0; /// operazioni per la ricerca

int max_dim = 0;
int ntests = 1;
int ndiv = 1;
int details = 0;
int graph = 0;

int n = 0;				  /// dimensione dell'array
int max_cycle_length = 0; // used in longest_cycle(int n)

/// file di output per grafo
ofstream output_graph;
int n_operazione = 0; /// contatore di operazioni per visualizzare i vari step

int ct_visit = 0; // contatore durante visita

//////////////////////////////////////////////////
/// Definizione della struttura dati lista
//////////////////////////////////////////////////

/// struct per il nodo della lista
typedef struct node {
	int val;
	struct node* next;
} node_t;

/// struct per la lista
typedef struct list {
	node* head;
	// node* tail; /// per lista doubly linked
} list_t;

//////////////////////////////////////////////////
/// Fine Definizione della struttura dati lista
//////////////////////////////////////////////////

//////////////////////////////////////////////////
/// Definizione della struttura dati grafo
//////////////////////////////////////////////////

int* V;			 // elenco dei nodi del grafo
int* V_visitato; // nodo visitato?
int* depth;

// list_t* E;  /// array con le liste di adiacenza per ogni nodo
list_t** E; /// array di puntatori a le liste di adiacenza per ogni nodo
int n_nodi;

//////////////////////////////////////////////////
/// Fine Definizione della struttura dati grafo
//////////////////////////////////////////////////

/// Questo e' un modo per stampare l'indirizzo node relativamente ad un altro di
/// riferimento. Permette di ottenere offset di piccola dimensione per essere
/// facilmente visualizzati Nota: il metodo non e' robusto e potrebbe avere
/// comportamenti indesiderati su architetture diverse L'alternativa corretta'
/// e' utilizzare %p di printf: es. printf("%p\n",(void*) node); con lo
/// svantaggio di avere interi a 64 bit poco leggibili

list_t* global_ptr_ref =
	NULL; /// usato per memorizzare il puntatore alla prima lista allocata

int get_address(void* node) { return (int)((long)node - (long)global_ptr_ref); }

void node_print(int n) {

	output_graph << "node_" << n << "_" << n_operazione << endl;
	output_graph << "[ shape = oval; ";
	if (V_visitato[n] == 1)
		output_graph << "fillcolor = \"#ff0000\"; style=filled; ";
	if (V_visitato[n] == 2)
		output_graph << "fillcolor = \"#555555\"; style=filled; ";
	output_graph << "label = "
				 << "\"Idx: " << n << ", val: " << V[n] << "\" ];\n";

	node_t* elem = E[n]->head;
	while (elem != NULL) { /// disegno arco
		output_graph << "node_" << n << "_" << n_operazione << " -> ";
		output_graph << "node_" << elem->val << "_" << n_operazione
					 << " [  color=gray ]\n";
		elem = elem->next;
	}
}

void graph_print() {
	for (int i = 0; i < n_nodi; i++)
		node_print(i);
	n_operazione++;
}

void list_print(list_t* l) {
	if (l->head == NULL) {
		printf("Lista vuota\n");
	} else {
		node_t* current = l->head;

		while (current != NULL) {
			if (!details)
				printf("%d, ", current->val);
			else { /// stampa completa
				if (current->next == NULL)
					printf("allocato in %d [Val: %d, Next: NULL]\n",
						   get_address(current), current->val);
				else
					printf("allocato in %d [Val: %d, Next: %d]\n",
						   get_address(current), current->val,
						   get_address(current->next));
			}
			current = current->next;
		}
		printf("\n");
	}
}

list_t* list_new() {
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

void list_delete(list_t* l) {
	//// implementare rimozione dal fondo della lista
	//// deallocazione struct list
}

void list_insert_front(list_t* l, int elem) {
	/// inserisce un elemento all'inizio della lista
	node_t* new_node = new node_t;
	new_node->next = NULL;

	new_node->val = elem;

	new_node->next = l->head;

	l->head = new_node;
}

void list_pop_front(list_t* l) {
	if (l == nullptr || l->head == nullptr)
		return;
	node_t* temp = l->head;
	l->head = temp->next;
	delete temp;
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
	output_graph << "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" > "
				 << endl;
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

int DFS(int n) {

	if (details)
		printf("DFS: lavoro sul nodo %d (visitato %d)\n", n, V_visitato[n]);

	if (V_visitato[n]) // se gia' visitato o scoperto --> esco
		return 0;

	V_visitato[n] = 1; // prima volta che incontro questo nodo

	if (details)
		printf("Visito il nodo %d (val %d)\n", n, V[n]);

	/// esploro la lista di adiacenza
	node_t* elem = E[n]->head;
	while (elem != NULL) { /// elenco tutti i nodi nella lista

		graph_print();

		/// espando arco  n --> elem->val
		/// quindi DFS(elem->val)
		output_graph << "dfs_" << n << " -> dfs_" << elem->val;
		if (V_visitato[elem->val])
			output_graph << "[color=gray, label = \"" << ct_visit++ << "\"]";
		else
			output_graph << "[color=red, label = \"" << ct_visit++ << "\"]";
		output_graph << endl;

		DFS(elem->val);

		elem = elem->next;
	}

	V_visitato[n] = 2; // abbandono il nodo per sempre
	graph_print();
	return 0;
}

// funtion to reach disconnected parts
void disconnected_part(int number_of_nodes) {
	// seach from the array flag if it is not visited
	for (int i = 0; i < number_of_nodes; i++) {
		if (V_visitato[i] == 0) {
			DFS(i);
		}
	}
}

int longest_cycle(const int n = 0) {
	std::cout << "Nodo " << n << " visitato: " << V_visitato[n] << " depth: " << depth[n] << std::endl;
	V_visitato[n] = 1;
	int max = 0;
	node_t* elem = E[n]->head;
	while (elem != nullptr) {
		int v = elem->val;
		int l = 0;
		if (V_visitato[v] != 1) {
			depth[v] = depth[n] + 1;
			l = longest_cycle(v);
		} else {
			l = depth[n] - depth[v] + 1;
		}
		std::cout << "Nodo " << n << " visitato: " << V_visitato[n] << " depth: " << depth[n] << " l: " << l << std::endl;
		max = (l > max) ? l : max;
		elem = elem->next;
	}
	V_visitato[n] = 2;
	return max;
}

void init() {
	V = new int[n_nodi];		  //(int*)malloc(n_nodi*sizeof(int));
	V_visitato = new int[n_nodi]; //(int*)malloc(n_nodi*sizeof(int));
	depth = new int[n_nodi];
	E = new list_t * [n_nodi];	  //(list_t**)malloc(n_nodi*sizeof(list_t*));

	// inizializzazione
	for (int i = 0; i < n_nodi; i++) {
		V[i] = i;
		depth[i] = 0;
		V_visitato[i] = 0; // flag = non visitato
		E[i] = list_new();
	}
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
		printf(
			"  -verbose: Abilita stampe durante l'esecuzione dell'algoritmo\n");
		printf("  -graph: creazione file di dot con il grafo dell'esecuzione "
			   "(forza d=1 t=1)\n");
		return 1;
	}

	return 0;
}

void graph1() {
	n_nodi = 13;
	init();

	list_insert_front(E[0], 1);
	list_insert_front(E[1], 2);
	list_insert_front(E[2], 0);

	// Ciclo B (2-3-4-2)
	list_insert_front(E[2], 3);
	list_insert_front(E[3], 4);
	list_insert_front(E[4], 2);

	// Ciclo C (4-5-6-7-4)
	list_insert_front(E[4], 5);
	list_insert_front(E[5], 6);
	list_insert_front(E[6], 7);
	list_insert_front(E[7], 4);

	// Ciclo D (7-8-9-7)
	list_insert_front(E[7], 8);
	list_insert_front(E[8], 9);
	list_insert_front(E[9], 7);

	// Ciclo E (0-10-11-12-0)
	list_insert_front(E[0], 10);
	list_insert_front(E[10], 11);
	list_insert_front(E[11], 12);
	list_insert_front(E[12], 0);

	// Collegamento 12→3 e ciclo F
	list_insert_front(E[12], 3);

}

void graph2() {
	n_nodi = 14;
	init();

	list_insert_front(E[8], 12);
	list_insert_front(E[12], 13);
	list_insert_front(E[13], 9);

	list_insert_front(E[0], 1);
	list_insert_front(E[1], 2);
	list_insert_front(E[2], 3);
	list_insert_front(E[3], 4);
	list_insert_front(E[4], 5);
	list_insert_front(E[5], 6);
	list_insert_front(E[6], 7);
	list_insert_front(E[7], 8);
	list_insert_front(E[8], 9);
	list_insert_front(E[9], 0);

	list_insert_front(E[6], 10);
	list_insert_front(E[10], 3);

	list_insert_front(E[7], 11);
	list_insert_front(E[11], 6);

}

void graph3() {
	n_nodi = 17;
	init();
	// Lungo ciclo principale: 0->1->2->3->4->5->6->7->8->9->0 (10 nodi)
	list_insert_front(E[0], 1);
	list_insert_front(E[1], 2);
	list_insert_front(E[2], 3);
	list_insert_front(E[3], 4);
	list_insert_front(E[4], 5);
	list_insert_front(E[5], 6);
	list_insert_front(E[6], 7);
	list_insert_front(E[7], 8);
	list_insert_front(E[8], 9);
	list_insert_front(E[9], 0);

	// Percorso alternativo per nodo 3: 2->10->11->3
	list_insert_front(E[2], 10);
	list_insert_front(E[10], 11);
	list_insert_front(E[11], 3);

	// Collegamento critico: 3->12->13->9
	list_insert_front(E[3], 12);
	list_insert_front(E[12], 13);
	list_insert_front(E[13], 9);

	// Cicli aggiuntivi per complessità
	list_insert_front(E[6], 14);
	list_insert_front(E[14], 15);
	list_insert_front(E[15], 3);
	list_insert_front(E[7], 16);
	list_insert_front(E[16], 6);
}

void graph4() {
	n_nodi = 13;
	init();
	// Ciclo principale: 0-1-2-3-4-0
	list_insert_front(E[0], 1);
	list_insert_front(E[1], 2);
	list_insert_front(E[2], 3);
	list_insert_front(E[3], 4);
	list_insert_front(E[4], 0);

	// Auto-loop critico
	list_insert_front(E[2], 2);  // Auto-loop su nodo 2

	// Percorsi alternativi
	list_insert_front(E[3], 5);  // 3→5→6→4
	list_insert_front(E[5], 6);
	list_insert_front(E[6], 4);

	list_insert_front(E[4], 7);  // 4→7→8→9→0
	list_insert_front(E[7], 8);
	list_insert_front(E[8], 9);
	list_insert_front(E[9], 0);

	// Ciclo interno che si auto-interseca
	list_insert_front(E[6], 10);  // 6→10→11→12→6
	list_insert_front(E[10], 11);
	list_insert_front(E[11], 12);
	list_insert_front(E[12], 6);

	// Collegamento che crea un ciclo "impossibile"
	list_insert_front(E[12], 2);  // 12→2 (collegamento al nodo 2)
	list_insert_front(E[11], 0);  // 11→0 (collegamento diretto)
	

}
int main(int argc, char** argv) {
	int i, test;
	srand((unsigned)time(NULL));
	if (parse_cmd(argc, argv))
		return 1;

	if (graph) {
		output_graph.open("graph.dot");
		/// preparo header
		output_graph << "digraph g" << endl;
		output_graph << "{ " << endl;
		output_graph << "node [shape=none]" << endl;
		output_graph << "rankdir=\"LR\"" << endl;
		;
		//    output_graph << "edge[tailclip=false,arrowtail=dot];"<<endl;
	}

	// int* V; // elenco dei nodi del grafo
	// list_t* E;  /// array con le liste di adiacenza per ogni nodo
	// // init random
	srand((unsigned)time(NULL));

	//graph1();
	//graph2();
	//graph3();
	graph4();


	for (int i = 0; i < n_nodi; i++) {
		// if (i == 0)
		global_ptr_ref = E[i];

		for (int j = 0; j < n_nodi; j++) {
			// for (int j = (n_nodi - 1); j >= 0; j--) {
			// for (int j = n_nodi-1; j >= 0; j--) {
			// if (rand() % 2 == 0)
			// if (i < j)//per generare un DAG
			//if ((i + 1) % n_nodi == j)
			//if (j == (i + 1) % n_temp) ///per generare un grafo con l'ultimo
			// element che punta al primo quindi un ciclo
			// if ((j + 1) / 2 == i + 1) //per fare uscire un albero binario
			//  if (j == (i+1) % n_nodi )
			//if (j == i + 1) // per fare uscire una lista
				//list_insert_front(E[i], j);
		}
	}


	graph_print();

	for (int i = 0; i < n_nodi; i++) {
		printf("Sono il nodo di indice %d nell'array\n", i);
		printf("Il valore del nodo e' %d\n", V[i]);
		printf("La lista di adiacenza e': ");
		list_print(E[i]);
	}

	//funziona solo per grafo fortemente connesso, ciclo for partendo da ogni nodo per generalizzare
	cout << "La lungheza del ciclo piu' lungo e' " << longest_cycle() << endl;
	//disconnected_part(n_nodi);

	graph_print();
	if (graph) {
		/// preparo footer e chiudo file
		output_graph << "}" << endl;
		output_graph.close();
		cout << " File graph.dot scritto" << endl
			 << "Creare il grafo con: dot graph.dot -Tpdf -o graph.pdf" << endl;
	}

	return 0;
}
