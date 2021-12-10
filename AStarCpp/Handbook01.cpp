#include "pch.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <random>
#define PB push_back 


using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int, int> pii;

class Graph {
public:
	int V;
	vector<vector<int>> adj_list;
	vi getNeighbors(int v);
	Graph(int n_nodes);
	bool connected();
	int get_V();
	void dfs(int s);
	void addEdge(int u, int v);
	~Graph();

private:
	void dfs_h(int s, vector<bool> visited);
	bool connected_h(int s, vector<bool> visited);
};

Graph::Graph(int n_nodes) {
	this->V = n_nodes;
	for (int i = 0; i < n_nodes; i++) {
		adj_list.PB(vi());
	}
}

vi Graph::getNeighbors(int v) {
	return adj_list[v];
}

bool Graph::connected() {
	vector<bool> visited(get_V());
	return connected_h(0, visited);
}

bool Graph::connected_h(int v, vector<bool> visited) {
	return true;
}

void Graph::addEdge(int u, int v) {
	adj_list[u].PB(v);
}

Graph::~Graph()
{
}

int Graph::get_V() {
	return this->V;
}

class WeightedGraph {
public:
	int V;
	vector<vector<pii>> roads;
	WeightedGraph(int n_nodes);
	int get_V();
	void addEdge(int u, int v, int w);
	vi shortestPath(int src);
	~WeightedGraph();
	void deleteWeightedGraph();
};

WeightedGraph::WeightedGraph(int n_nodes) {
	this->V = n_nodes;
	for (int i = 0; i < n_nodes; i++) {
		roads.PB(vector<pii>());
	}
}

void WeightedGraph::addEdge(int u, int v, int w) {
	roads[u].PB({v, w});
}

int WeightedGraph::get_V() {
	return this->V;
}

bool compareDouble(double a, double b) {
	return (abs(a - b) < 1e-9);
}


void Graph::dfs(int s) {
	vector<bool> visited(get_V());
	dfs_h(s, visited);

}

void Graph::dfs_h(int s, vector<bool> visited) {
	if (visited[s]) return;
	visited[s] = true;
	cout << "Visited: " << s << "\n";
	for (auto u : getNeighbors(s)){
		dfs_h(u, visited);
	}                                  
}

void bfs_h(int s, vector<bool> visited, vi distance, queue<int> bfs_queue, Graph g) {
	visited[s] = true;
	distance[s] = 0;
	bfs_queue.push(s);
	while (!bfs_queue.empty()) {
		int s = bfs_queue.front(); bfs_queue.pop();
		for (auto u : g.getNeighbors(s)) {
			if (visited[u]) continue;
			visited[u] = true;
			cout << "Visited: " << u << "\n";
			distance[u] = distance[s] + 1;
			bfs_queue.push(u);
		}
	}
}

void bfs(int s, Graph g) {
	vector<bool> visited(g.get_V());
	vi distance(g.get_V());
	queue<int> bfs_queue;
	bfs_h(s, visited, distance, bfs_queue, g);
}
/*
int main() {

	int n_tests, n_nodes, n_edges;
	
	cin >> n_tests;
	cout << n_tests << "\n";
	vector<int> nodes;
	
	for (int i = 0; i < n_tests; i++) {


		;
	}
}
*/

static std::unordered_map<int, int> tnum_memo;
int get_tnum(int n) {
	if (n == 1) {
		return 1;
	}
	if (n % 2 == 0) {
		if (tnum_memo.count(n) == 0) {
			tnum_memo[n] = 1 + get_tnum(n / 2);
		}
		return tnum_memo[n];
	}
	else {
		if (tnum_memo.count(n) == 0) {
			tnum_memo[n] = 1 + get_tnum((n * 3) + 1);
		}
		return tnum_memo[n];
	}
}
int main3() {
	std::vector<int> tnumbers(1000,0);
	int x, y, cycle_length = 0, max_cycle_length = 0;
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distro(1, 1000);

	int inp;
	/*while (std::cin >> inp) {
		std::cout << get_tnum(inp) << "\n";

		int t = inp;
		long long times = 1;
		while (t != 1)
		{
			times++;
			if (t % 2)
				t = 3 * t + 1;
			else
				t = t / 2;
		}
		std::cout << times;
	}*/
	std::ifstream input_file("cs411_problem1.in");
	std::ofstream output_file("cs411_problem1.out");
	
	/*for (int i = 1; i < 400; i++) {
		x = distro(rng);
		y = distro(rng);
		if (x < y) {
			input_file << x << " " << y << '\n';
		}
		else if (x > y) {
			input_file << y << " " << x << '\n';
		}
	}*/
	int c_index = 0;
	while(input_file >> x >> y){
	//while (std::cin >> x >> y) {
		//output_file << x << " " << y;
		max_cycle_length = 0;
		for (c_index = x; c_index <= y; c_index++) {
			cycle_length = get_tnum(c_index);
			if (cycle_length > max_cycle_length) {
				max_cycle_length = cycle_length;
			}
		}
		output_file << x << " " << y << " " << max_cycle_length << "\n";
	}
	
	return 0;
}