//Nna minkousse Kenneth James
//Belli Andrea
//Premori Samuele

#include <fstream>
#include <iostream>
using namespace std;

// copy the code below to generate the graphs both for the original graph and
// the MST dot -Tpdf mst_steps.dot -o mst_steps.pdf dot -Tpdf original_graph.dot
// -o original_graph.pdf
const int MAX = 5000;
int parent[MAX];
int n_nodi;

struct Edge {
	double u, v, w;
};

// Handles DOT operations
class DotWriter {
  private:
	std::ofstream fout;

  public:
	DotWriter(const char* filename) : fout(filename) {}

	~DotWriter() {
		if (fout.is_open()) {
			fout.close();
		}
	}

	void writeGraphHeader(const char* graph_name, const char* title) {
		fout << "graph " << graph_name << " {" << std::endl;
		fout << "    rankdir=TB;" << std::endl;
		fout << "    node [shape=circle];" << std::endl;
		fout << "    label=\"" << title << "\";" << std::endl;
		fout << "    labelloc=top;" << std::endl;
		fout << "    compound=true;" << std::endl;
	}

	void writeGraphFooter() { fout << "}" << std::endl; }

	void writeInitialStage(int stage, int n_nodi) {
		fout << "    subgraph cluster_stage" << stage << " {" << std::endl;
		fout << "        label=\"Stadio " << stage << " - Inizio\";"
			 << std::endl;
		fout << "        style=filled;" << std::endl;
		fout << "        color=lightgrey;" << std::endl;

		for (int i = 0; i < n_nodi; i++) {
			fout << "        s" << stage << "_" << i << " [label=\"" << i
				 << "\"];" << std::endl;
		}
		fout << "    }" << std::endl;
	}

	void writeStageHeader(int stage, int u, int v, double w, bool accepted) {
		fout << "    subgraph cluster_stage" << stage << " {" << std::endl;
		fout << "        label=\"Stadio " << stage << " - Arco (" << u << ","
			 << v << ") peso:" << w;

		if (accepted) {
			fout << " ACCETTATO\";" << std::endl;
			fout << "        style=filled;" << std::endl;
			fout << "        color=lightgreen;" << std::endl;
		} else {
			fout << " RIFIUTATO\";" << std::endl;
			fout << "        style=filled;" << std::endl;
			fout << "        color=lightcoral;" << std::endl;
		}
	}

	void writeClusterHeader(int stage, int cluster_id, int root) {
		fout << "        subgraph cluster_" << stage << "_" << cluster_id
			 << " {" << std::endl;
		fout << "            style=rounded;" << std::endl;
		fout << "            color=blue;" << std::endl;
		fout << "            label=\"Cluster " << root << "\";" << std::endl;
	}

	void writeClusterFooter() { fout << "        }" << std::endl; }

	void writeNode(int stage, int node) {
		fout << "            s" << stage << "_" << node << " [label=\"" << node
			 << "\"];" << std::endl;
	}

	void writeMSTEdge(int stage, int u, int v, double weight) {
		fout << "        s" << stage << "_" << u << " -- s" << stage << "_" << v
			 << " [color=red, penwidth=2, label=\"" << weight << "\"];"
			 << std::endl;
	}

	void writeStageFooter() { fout << "    }" << std::endl; }
};
// Find operation: returns the representative (root) of the set containing x
int find(int x) {
	// If parent[x] == -1, then x is the representative of its set
	if (parent[x] == -1)
		return x;
	// Otherwise, recursively find the representative and apply path compression
	parent[x] = find(parent[x]);
	return parent[x];
}

// Union operation
void union_sets(int x, int y) {
	int px = find(x);
	int py = find(y);
	if (px != py)
		parent[px] = py;
}

// Returns true if u and v are in the same set
bool check_cycle(int u, int v) { return find(u) == find(v); }

// Insertion sort for edges by weight so edges.w
void insertion_sort(Edge edges[], int m) {
	for (int i = 1; i < m; i++) {
		Edge key = edges[i];
		int j = i - 1;
		while (j >= 0 && edges[j].w > key.w) {
			edges[j + 1] = edges[j];
			j--;
		}
		edges[j + 1] = key;
	}
}

// Kruskal's algorithm using insertion sort, also collects MST edges
void kruskal(Edge edges[], int n_nodi, int n_archi, Edge mst_edges[],
			 int& mst_size) {
	// Initialize all parents to -1 (each node is its own set)
	for (int i = 0; i < n_nodi; i++)
		parent[i] = -1;

	insertion_sort(edges, n_archi);

	int cost = 0;
	mst_size = 0;
	int i = 0;		   // Index for edges
	int Mst_edges = 0; // Number of edges added to MST

	// Continue until MST has (n_nodi - 1) edges or all edges are checked
	while (Mst_edges < n_nodi - 1 && Mst_edges < n_archi) {
		int u = edges[i].u;
		int v = edges[i].v;
		int w = edges[i].w;

		// Check if adding this edge forms a cycle
		if (!check_cycle(u, v)) {
			// If not, add the edge to MST
			parent[find(u)] = find(v); // Union operation
			cost += w;
			mst_edges[mst_size++] = edges[i];
			Mst_edges++; // Increment number of edges in MST
		}
		i++; // Move to the next edge
	}
}

void print_graph(Edge edges[], int m) {
	cout << "Graph edges:" << endl;
	for (int i = 0; i < m; i++) {
		cout << "Nodes : " << edges[i].u << " -- " << edges[i].v
			 << " [weight = " << edges[i].w << "]" << endl;
	}
	cout << endl;
}

void write_graph_dot_to_file(Edge edges[], int m, const char* graph_name,
							 const char* filename) {
	std::ofstream fout(filename);
	fout << "graph " << graph_name << " {" << std::endl;
	for (int i = 0; i < m; i++) {
		fout << "    " << edges[i].u << " -- " << edges[i].v
			 << " [label=" << edges[i].w << "];" << std::endl;
	}
	fout << "}" << std::endl;
	fout.close();
}

void write_mst_steps_dot(Edge edges[], int n_edges, Edge mst_edges[],
						 int mst_size, const char* filename) {
	std::ofstream fout(filename);
	fout << "graph MST_Steps {" << std::endl;
	fout << "    label=\"Costruzione MST passo-passo\";" << std::endl;
	fout << "    labelloc=top;" << std::endl;

	// Disegna tutti i nodi (per evitare nodi isolati non visualizzati)
	for (int i = 0; i < n_edges; i++) {
		fout << "    " << edges[i].u << ";" << std::endl;
		fout << "    " << edges[i].v << ";" << std::endl;
	}

	// Step 0: grafo originale (tutti gli archi in grigio)
	for (int i = 0; i < n_edges; i++) {
		fout << "    " << edges[i].u << " -- " << edges[i].v
			 << " [label=" << edges[i].w << ", color=gray, style=dashed];"
			 << std::endl;
	}

	// Step-by-step: aggiungi un arco MST alla volta in rosso
	for (int step = 0; step < mst_size; step++) {
		fout << "    // Step " << (step + 1) << ": aggiungo "
			 << mst_edges[step].u << " -- " << (char)mst_edges[step].v
			 << std::endl;
		fout << "    " << mst_edges[step].u << " -- " << mst_edges[step].v
			 << " [label=" << mst_edges[step].w << ", color=red, penwidth=2];"
			 << std::endl;
	}

	fout << "}" << std::endl;
	fout.close();
}

int parseMatrixFile(const char* filename, Edge edges[], int maxSize) {
	FILE* file = fopen(filename, "r");

	if (!file) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return -1;
	}

	// Skip comment lines
	char line[1024];
	while (fgets(line, sizeof(line), file)) {
		if (line[0] != '%') {
			break;
		}
	}

	// Read dimensions from first non-comment line
	int rows, cols, entries;
	sscanf(line, "%d %d %d", &rows, &cols, &entries);

	n_nodi = (rows > cols) ? rows : cols;

	std::cout << "Matrix dimensions: " << rows << "x" << cols << " with "
			  << entries << " entries" << std::endl;
	std::cout << "Number of nodes set to: " << n_nodi << std::endl;

	// Check if the static array is large enough
	if (entries > maxSize) {
		std::cerr << "Error: File contains " << entries
				  << " entries, but array can only hold " << maxSize
				  << std::endl;
		fclose(file);
		return -1;
	}

	// Read the actual matrix data
	Edge edge;
	int linesRead = 0;
	while (linesRead < entries && linesRead < maxSize &&
		   fscanf(file, "%lf %lf %lf", &edge.u, &edge.v, &edge.w) == 3) {

		// Convert from 1-based to 0-based indexing if needed
		edge.u = edge.u - 1;
		edge.v = edge.v - 1;
		edge.w = edge.w;

		// Validate node indices
		if (edge.u >= 0 && edge.v >= 0 && edge.u < n_nodi && edge.v < n_nodi) {
			edges[linesRead] = edge;
			linesRead++;
		} else {
			std::cerr << "Warning: Invalid edge (" << (edge.u + 1) << ", "
					  << (edge.v + 1) << ") - skipping" << std::endl;
		}
	}

	fclose(file);

	std::cout << "Successfully parsed " << linesRead << " valid edges from file"
			  << std::endl;

	return linesRead;
}

// Helper function to write cluster nodes using the DotWriter
void writeClusterNodes(DotWriter& dot_writer, int stage, int n_nodi) {
	bool visited[MAX] = {false};
	int cluster_id = 0;

	// Debug: stampa stato parent array
	if (stage <= 5) {  // Solo per i primi stadi
		std::cout << "Stage " << stage << " - Parent array state:" << std::endl;
		for (int i = 0; i < n_nodi && i < 20; i++) {  // Solo primi 20 nodi
			std::cout << "Node " << i << " -> parent: " << parent[i] << ", root: " << find(i) << std::endl;
		}
		std::cout << std::endl;
	}

	for (int node = 0; node < n_nodi; node++) {
		if (!visited[node]) {
			int root = find(node);

			// Conta quanti nodi in questo cluster
			int cluster_size = 0;
			for (int k = 0; k < n_nodi; k++) {
				if (find(k) == root) {
					cluster_size++;
				}
			}

			// Debug: stampa informazioni cluster
			if (stage <= 5) {
				std::cout << "Cluster " << cluster_id << " - Root: " << root << ", Size: " << cluster_size << " - Nodes: ";
				for (int k = 0; k < n_nodi; k++) {
					if (find(k) == root) {
						std::cout << k << " ";
					}
				}
				std::cout << std::endl;
			}

			// Se il cluster ha più di un nodo, crea un sottogruppo
			if (cluster_size > 1) {
				dot_writer.writeClusterHeader(stage, cluster_id, root);
			}

			// Aggiungi tutti i nodi di questo cluster (anche se è un singolo nodo)
			for (int k = 0; k < n_nodi; k++) {
				if (find(k) == root) {
					dot_writer.writeNode(stage, k);
					visited[k] = true;
				}
			}

			// Chiudi il sottogruppo solo se era stato aperto
			if (cluster_size > 1) {
				dot_writer.writeClusterFooter();
			}
			cluster_id++;
		}
	}
}

// Helper function to write MST edges using the DotWriter
void writeMSTEdges(DotWriter& dot_writer, int stage, Edge mst_edges[],
				   int mst_size) {
	for (int j = 0; j < mst_size; j++) {
		dot_writer.writeMSTEdge(stage, (int)mst_edges[j].u, (int)mst_edges[j].v,
								mst_edges[j].w);
	}
}

void write_kruskal_stages_to_dot(Edge edges[], int n_nodi, int n_archi,
								 const char* filename) {
	// Copia gli archi e li ordina
	Edge sorted_edges[MAX];
	for (int i = 0; i < n_archi; i++) {
		sorted_edges[i] = edges[i];
	}
	insertion_sort(sorted_edges, n_archi);

	// Initialize DOT writer
	DotWriter dot_writer(filename);
	dot_writer.writeGraphHeader("KruskalStages",
								"Algoritmo di Kruskal - Stadi");

	// Array per MST temporaneo
	Edge mst_temp[MAX];
	int mst_temp_size = 0;

	// Inizializza parent array
	for (int i = 0; i < n_nodi; i++)
		parent[i] = -1;

	int stage = 0;
	int mst_edges_count = 0;

	// Stadio iniziale - tutti i nodi separati
	dot_writer.writeInitialStage(stage, n_nodi);

	// Processa ogni arco
	for (int i = 0; i < n_archi && mst_edges_count < n_nodi - 1; i++) {
		int u = sorted_edges[i].u;
		int v = sorted_edges[i].v;
		double w = sorted_edges[i].w;

		stage++;

		// Controlla se forma un ciclo
		bool accepted = !check_cycle(u, v);

		if (accepted) {
			// Aggiungi all'MST
			union_sets(u, v);
			mst_temp[mst_temp_size++] = sorted_edges[i];
			mst_edges_count++;
		}

		// Scrivi header dello stadio
		dot_writer.writeStageHeader(stage, u, v, w, accepted);

		// Raggruppa nodi per cluster
		writeClusterNodes(dot_writer, stage, n_nodi);

		// Aggiungi gli archi dell'MST corrente
		writeMSTEdges(dot_writer, stage, mst_temp, mst_temp_size);

		dot_writer.writeStageFooter();
	}

	dot_writer.writeGraphFooter();
}

int main() {
	int n_archi; // n_nodi declared globally

	// cout << "Enter the number of nodes and edges: ";
	// cin >> n_nodi >> n_archi;
	// Edge edges[MAX];
	// for (int i = 0; i < n_archi; i++)
	// 	cin >> edges[i].u >> edges[i].v >> edges[i].w;

	Edge edges[MAX];
	n_archi = parseMatrixFile("../src/test_clusters.mtx", edges, MAX);

	if (n_archi <= 0) {
		std::cerr << "Not enough edges" << n_archi << std::endl;
		return 1;
	}

	// Genera file DOT per visualizzare i cluster
	write_kruskal_stages_to_dot(edges, n_nodi, n_archi, "kruskal_stages.dot");

	// Prepare MST edges array
	Edge mst_edges[MAX];
	int mst_size = 0;

	// Run Kruskal and collect MST edges
	//kruskal(edges, n_nodi, n_archi, mst_edges, mst_size); // segfault here

	//print_graph(edges, n_archi);
	//print_graph(mst_edges, mst_size);

	// Write to files
	//write_graph_dot_to_file(edges, n_archi, "OriginalGraph",
	//						"original_graph.dot");
	// write_graph_dot_to_file(mst_edges, mst_size, "MST", "mst_graph.dot");
	//write_mst_steps_dot(edges, n_archi, mst_edges, mst_size, "mst_steps.dot");

	return 0;
}
