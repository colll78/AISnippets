#include "pch.h"
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <tuple>

using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef pair<int, int> pii;

const int BLANK = 1;
const int RAY = 2;
const int NW_MIRROR = 3;
const int NE_MIRROR = 4;
const int SENSOR = 5;

void printGraph(const vvi &graph) {
	cout << "\nPrint:\n";
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			cout << graph[i][j] << " ";
		}
		cout << "\n";
	}
}

pii coord_move(const vvi &grid, pii coord, pii direction) {
	coord = { coord.first + direction.first, coord.second + direction.second };
	if (coord.first >= 0 && coord.first <= (grid.size() - 1) && coord.second >= 0 && coord.second <= (grid.size() - 1)) {
		return coord;
	}
	return { -1, -1 };
}

const int DOWN = 0, UP = 1, LEFT = 2, RIGHT = 3;
bool laser_detected(const vvi &grid, int laser_col, int sensor_row) {
	pii coords = { 0, laser_col-1 };
	vector<pii> directions = { { 1, 0 } , { -1, 0 }, { 0, -1 }, { 0, 1 } };
	int dir = DOWN;
	map<pii, bool> path;
	map<tuple<int,int,int>, bool> visited;
	while (true) {
		path[coords] = true;
		if (grid[coords.first][coords.second] == BLANK) {
			coords = coord_move(grid, coords, directions[dir]);
			if (coords.first == -1 || visited[{coords.first, coords.second, dir}] == true) {
				path[coords] = true;
				break;
			}
			visited[{coords.first, coords.second, dir}] = true;
			continue;
		}
		else if (grid[coords.first][coords.second] == NW_MIRROR ) { /* \\ */
			switch (dir) {
				case DOWN:
					dir = RIGHT;
					break;
				case UP:
					dir = LEFT;
					break;
				case RIGHT:
					dir = DOWN;
					break;
				case LEFT:
					dir = UP;
					break;
			}
			coords = coord_move(grid, coords, directions[dir]);
			if (coords.first == -1 || visited[{coords.first, coords.second, dir}] == true) {
				path[coords] = true;
				break;
			}
			visited[{coords.first, coords.second, dir}] = true;
			continue;
		}
		else if (grid[coords.first][coords.second] == NE_MIRROR) { /* / */
			switch (dir) {
				case DOWN:
					dir = LEFT;
					break;
				case UP:
					dir = RIGHT;
					break;
				case RIGHT:
					dir = UP;
					break;
				case LEFT:
					dir = DOWN;
					break;
			}
			coords = coord_move(grid, coords, directions[dir]);
			if (coords.first == -1 || visited[{coords.first, coords.second, dir}] == true) {
				path[coords] = true;
				break;
			}
			visited[{coords.first, coords.second, dir}] = true;
			continue;
		}
		else if (grid[coords.first][coords.second] == SENSOR) {
			return true;
		}
	}

	coords = { sensor_row, grid.size() - 1 };
	visited.clear();
	dir = LEFT;
	while (true) {
		if (grid[coords.first][coords.second] == BLANK || grid[coords.first][coords.second] == SENSOR) {
			if (path[coords]) {
				return true;
			}
			coords = coord_move(grid, coords, directions[dir]);
			if (coords.first == -1 || visited[{coords.first, coords.second, dir}] == true) {
				break;
			}
			visited[{coords.first, coords.second, dir}] = true;
			continue;
		}
		else if (grid[coords.first][coords.second] == NW_MIRROR) { /* \\ */
			switch (dir) {
			case DOWN:
				dir = RIGHT;
				break;
			case UP:
				dir = LEFT;
				break;
			case RIGHT:
				dir = DOWN;
				break;
			case LEFT:
				dir = UP;
				break;
			}
			coords = coord_move(grid, coords, directions[dir]);
			if (coords.first == -1 || visited[{coords.first, coords.second, dir}] == true) {
				break;
			}
			visited[{coords.first, coords.second, dir}] = true;
			continue;
		}
		else if (grid[coords.first][coords.second] == NE_MIRROR) { /* / */
			switch (dir) {
			case DOWN:
				dir = LEFT;
				break;
			case UP:
				dir = RIGHT;
				break;
			case RIGHT:
				dir = UP;
				break;
			case LEFT:
				dir = DOWN;
				break;
			}
			coords = coord_move(grid, coords, directions[dir]);
			if (coords.first == -1 || visited[{coords.first, coords.second, dir}] == true) {
				break;
			}
			visited[{coords.first, coords.second, dir}] = true;
			continue;
		}
	}
	return false;
}

int main(){
	int room_size, laser_col, sensor_row;
	cin >> room_size >> laser_col >> sensor_row;
	vvi grid(room_size+1, vi(room_size + 1,	1));
	grid[sensor_row][room_size] = SENSOR;
	char block;
	for (int i = 1; i < room_size + 1; i++) {
		for (int j = 0; j < room_size; j++) {
			cin >> block;
			switch (block) {
				case '.':
					grid[i][j] = BLANK;
					break;
				case '/':
					grid[i][j] = NE_MIRROR;
					break;
				case '\\':
					grid[i][j] = NW_MIRROR;
					break;
			}
		}
	}
	//printGraph(grid);
	if (laser_detected(grid, laser_col, sensor_row)) {
		cout << "yes";
	}
	else {
		cout << "no";
	}
}

