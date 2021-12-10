//#include "pch.h"
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <map>
#include <stack>
#define PB push_back

using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef pair<int, int> pii;
typedef vector<vector<int>> vvi;

void add_edge(vvi &adj_list, int u, int v) {
	adj_list[u].PB(v);
	adj_list[v].PB(u);
}

bool is_bipartite(vvi adj_list) {
	const int red = 0, blue = 1;
	stack<int> dfs_stack;
	vi colors(adj_list.size());
	vector<bool> visited(adj_list.size());
	dfs_stack.push(0);
	colors[0] = red;

	while (!dfs_stack.empty()) {
		int cur_node = dfs_stack.top(); dfs_stack.pop();
		visited[cur_node] = true;

		for (auto node : adj_list[cur_node]) {
			if (!visited[node]) {
				colors[node] = (colors[cur_node] == red) ? blue : red;
				dfs_stack.push(node);
			}
			else if(colors[cur_node] == colors[node]){
				return false;
			}
		}
	}

	return (visited.size() == adj_list.size());
}

bool is_neighbor(const vvi &graph, int i, int j) {
	return (find(graph[i].begin(), graph[i].end(), j) != graph[i].end());
}

int main(){
	map<int, int> resource_map;

	int num_islands, num_resources;
	cin >> num_islands >> num_resources;
	vvi island_graph(num_islands);
	
	for (int i = 0; i < num_islands; i++) {
		int resource_i = -1;
		while (cin >> resource_i) {
			if (resource_i == 0) break;
			if (resource_map.count(resource_i) > 0) {
				if (!is_neighbor(island_graph, resource_map[resource_i], i)) {
					add_edge(island_graph, i, resource_map[resource_i]);
				}
			} else {
				resource_map[resource_i] = i;
			}
		}
	}

	if (is_bipartite(island_graph)) {
		cout << "yes";
	}
	else {
		cout << "no";
	}
}

