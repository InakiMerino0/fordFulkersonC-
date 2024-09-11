#include <iostream>
#include <limits.h>
#include <queue>
#include <string.h>
#include <vector>

using namespace std;

/* Retorna true si existe un path entre fuente (s) a
sumidero (t) en el grafo residual. Tambien rellena parent[]
para almacenar el path */

bool bfs(vector<vector<int>>& rGraph, int s, int t, vector<int>& parent, int V)
{
	// Resize segun opcion ingresada por el usuario para procesar grafo de 6 o de 8
	rGraph.resize(V);
	parent.resize(V);
	// Declaracion array visited, y marcar todos los nodos como no visitados
	vector<bool> visited(V, false);

	// Declaracion de cola q, push de nodo fuente y marcarlo como visited = true
	queue<int> q;
	q.push(s);
	visited[s] = true;
	parent[s] = -1;

	// BFS
	while (!q.empty()) {
		int u = q.front();
		q.pop();

		for (int v = 0; v < V; v++) {
			if (visited[v] == false && rGraph[u][v] > 0) {
				// Si se encuentra una conexion al sumidero
				// entonces BFS ya no tiene sentido
				// se define la conexion y se retorna true
				if (v == t) {
					parent[v] = u;
					return true;
				}
				q.push(v);
				parent[v] = u;
				visited[v] = true;
			}
		}
	}

	// No se alcanzo sumidero en BFS comenzando desde source, entonces
	// return false
	return false;
}

// Retorna el flujo maximo desde source a sink en el grafo dado
int fordFulkerson(vector<vector<int>>& graph, int s, int t, int V)
{
	int u, v;

	// Crea un grafo residual de tamaño V y lo rellena con las capacidades dadas del grafo original
	vector<vector<int>> rGraph(V, vector<int>(V, 0));
	for (u = 0; u < V; u++)
		for (v = 0; v < V; v++)
			rGraph[u][v] = graph[u][v];


	vector<int> parent;// Este array es rellenado por el BFS y para almacenar el path
	parent.resize(V);

	int max_flow = 0; // Al comenzar no hay flujo

	// El flujo se aumenta siempre que exista un path desde fuente a sumidero
	while (bfs(graph, s, t, parent, V)) {

		// Encuentra la capacidad minima residual de los vertices a lo largo del path rellenado por BFS.
		// Es decir, se busca el flujo maximo a traves del path encontrado.
		int path_flow = INT_MAX;
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			path_flow = min(path_flow, graph[u][v]);
		}

		// Actualiza capacidad residual de las aristas y aristas invertidas
		// a lo largo del path
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			graph[u][v] -= path_flow;
			graph[v][u] += path_flow;
		}

		// Añadir flujo del path al flujo general
		max_flow += path_flow;
	}

	// Retorna el flujo maximo
	return max_flow;
}

int main()
{
	// Grafo de 6 nodos
	vector<vector<int>> graph(6, vector<int>(6, 0));
	graph = {
		// O   A   B   C   D   S
		{  0, 12, 15,  0,  0,  0},  // O
		{  0,  0,  0,  4,  9,  0},  // A
		{  0,  0,  0,  7,  0,  9},  // B
		{  0,  0,  0,  0,  0, 11},  // C
		{  0,  0,  0,  0,  0,  8},  // D
		{  0,  0,  0,  0,  0,  0}   // S
	};

	// Grafo de 8 nodos
	vector<vector<int>> graph2(8, vector<int>(8, 0));
	graph2 = {
		// S   A   B   C   D   E   F   T
		{  0, 10,  6, 15,  0,  0,  0,  0},  // S
		{  0,  0,  0,  0, 20,  0,  0,  0},  // A
		{  0, 12,  0,  0,  0,  0,  0,  0},  // B
		{  0,  0,  0,  0,  7,  0,  5,  0},  // C
		{  0,  0,  6,  0,  0, 30,  5,  0},  // D
		{  0,  0,  0,  0,  0,  0,  0, 19},  // E
		{  0,  0,  0,  0,  5,  0,  0, 24},  // F
		{  0,  0,  0,  0,  0,  0,  0,  0}   // T 
	};

	// Menu de seleccion para el usuario
	int opcion;
	cout << "1. Para grafo con 6 nodos " << endl;
	cout << "2. Para grafo con 8 nodos" << endl;
	cin >> opcion;
	switch (opcion)
	{
	case 1:
		cout << "El flujo maximo posible es " << fordFulkerson(graph, 0, 5, 6);
	break;
	case 2:
		cout << "El flujo maximo posible es " << fordFulkerson(graph2, 0, 5, 8);
	break;
	default:
		cout << "Opcion ingresada no valida" << endl;
	}
	
	return 0;
}
