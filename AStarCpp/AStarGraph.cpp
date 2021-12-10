// AStarGraph.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "SimpleGraph.h"
#include "WeightedGrid.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <utility>

int main(){
	SimpleGraph g = SimpleGraph({
	{'A', {'B'}},
	{'B', {'C'}},
	{'C', {'B', 'D', 'F'}},
	{'D', {'C', 'E'}},
	{'E', {'F'}},
	{'F', {}},
	});
	std::cout << "Reachable from A:\n";
	g.breadth_first_search('A');
	std::cout << "Reachable from E:\n";
	g.breadth_first_search('E');
	WeightedGrid grid(10, 10, true);
	std::pair<int,int> start{ 1, 4 }, goal{ 8, 3 };
	grid.dijkstras_search(start, goal);
	grid.print(start, goal);
	std::cout << "\n";
	std::vector<std::pair<int,int>> path = grid.reconstruct_path(start, goal);
	grid.print(start, goal, &path);
}
