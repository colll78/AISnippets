#include "lodepng.h"
#include "Maze.hpp"
#include <random>
#include <algorithm>

/**
   * Create a blank maze of size 0-by-0
   * @return maze with rows=0 and cols=0
   */
Maze::Maze() {
    rows = 0;
    cols = 0;
    maze = {};
    visited = {};
}

/**
 * Constructor will generate a random maze given input parameters.  Throws exception if maze parameters imply/yield an invalid maze.
 * @param rows - height of maze
 * @param cols - width of maze
 * @param begin - entry point in maze
 * @param end - exit point in maze
 */
Maze::Maze(std::size_t rows, std::size_t cols, std::size_t begin, std::size_t end) throw(std::invalid_argument) {

    std::vector<size_t> neighbors;
    Stack<size_t> s;
    size_t cell, next;
    visited = {};

    if ((rows < 2 && cols < 2) || begin == end || begin > rows*cols || end > rows*cols)
        throw std::invalid_argument("Maze is invalid");

    this->rows = rows;
    this->cols = cols;

    for (int i = 0; i < rows * cols; i++) {
        maze.push_back('#');
        visited.push_back(0);
    }

    maze[begin] = 'B';
    maze[end] = 'E';

    visited[0] = 1;
    s.push(0);
    cell = 0;

    while (!s.isEmpty()) {

        cell = s.peek();
        s.pop();

        neighbors = getNeighbors(cell);

        if (neighbors.size()) {

            s.push(cell);
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(neighbors.begin(), neighbors.end(), g);

            next = neighbors.front();

            if (next == cell - 2)
                maze[cell - 1] = ' ';
            else if (next == cell + 2)
                maze[cell + 1] = ' ';
            else if (next == cell - 2 * cols)
                maze[cell - cols] = ' ';
            else
                maze[cell + cols] = ' ';

            visited[next] = 1;
            s.push(next);

        }

    }
    
}

/**
 * Function to set the maze we want to solve, validate it, and set private variables appropriately (a valid maze
 * will only have characters listed in the project description for an unsolved maze). Should overwrite existing maze, if any.
 * @param maze - a one dimensional vector representing an unsolved maze
 * @param rows - the number of rows in the maze
 * @return whether the maze is valid
 */
bool Maze::set(const std::vector<unsigned char>& maze, std::size_t rows) {

    bool isValid = true,
        hasBegin = false,
        hasEnd = false;

    for (int i = 0; i < maze.size(); i++) {
        if (maze[i] != ' ' && maze[i] != '#' && maze[i] != 'B' && maze[i] != 'E')
            isValid = false;
        else if (maze[i] == 'B')
            hasBegin = true;
        else if (maze[i] == 'E')
            hasEnd = true;
    }

    if (!hasBegin || !hasEnd || maze.size() % rows)
        isValid = false;

    if (isValid) {
        this->maze = maze;
        this->rows = rows;
        cols = maze.size() / rows;
        visited.clear();
    }

    return isValid;

}

/**
 * Return the maze
 * @return an unsolved or solved maze, depending on whether it has been solved or not
 */
const std::vector<unsigned char>& Maze::get() const {
    return maze;
}

/**
 * Generate a new maze with the given parameters.  Should overwrite existing maze, if any.
 * @param rows - height of maze
 * @param cols - width of maze
 * @param begin - entry point in maze
 * @param end - exit point in maze
 * @return true if we could create a valid maze
 */
bool Maze::create(std::size_t rows, std::size_t cols, std::size_t begin, std::size_t end) {

    visited.clear();
    maze = Maze(rows, cols, begin, end).get();
    for (int i = 0; i < rows * cols; i++)
        maze[i] = ' ';
    maze[3] = '#';
    maze[0] = 'B';
    maze[8] = 'E';
    this->rows = rows;
    this->cols = cols;

    return true;

}

/**
 * Find the path from B to E in the current maze and record the path in the maze
 * @return false if no path could be found from B to E or error, true otherwise
 */
bool Maze::solve() {

    visited.resize(rows * cols, 0);
    std::fill(visited.begin(), visited.end(), 0);

    size_t i = 0;
    while (maze[i] != 'B')
        i++;

    return backtrack(i);

    /*
    while (getSuccessors(i).size()) {
        if (maze[i] == 'E')
            return true;
        i = getSuccessors(i)[0];
        maze[i] = '*';
    }

    while (backtrack(i)) {

        while (getSuccessors(i).size()) {
            if (maze[i] == 'E')
                return true;
            i = getSuccessors(i)[0];
            maze[i] = '*';
        }

    }

    return false;
    */

}

//return indices of neighboring cells (ordered top, bottom, left, right)
std::vector<std::size_t> Maze::getNeighbors(std::size_t ind) const {

    std::vector<size_t> neighbors = {};

    if (ind >= 0 && ind < rows * cols) {

        if (ind > 2 * cols) {
            if(visited[ind - 2 * cols] == 0)
                neighbors.push_back(ind - 2 * cols);
        }

        if ((ind + 2 * cols) < rows * cols) {
            if (visited[ind + 2 * cols] == 0)
                neighbors.push_back(ind + 2 * cols);
        }

        if (ind > 2) {
            if(visited[ind - 2] == 0)
                neighbors.push_back(ind - 2);
        }

        if (ind + 2 < rows * cols) {
            if(visited[ind + 2] == 0)
                neighbors.push_back(ind + 2);
        }

    }
    
    return neighbors;

}

//successors of ind (ordered top, bottom, left, right)
std::vector<std::size_t> Maze::getSuccessors(std::size_t ind) const {

    std::vector<size_t> successors = {};

    if (ind >= 0 && ind < rows * cols) {

        if (ind > cols) {
            if (visited[ind - cols] == 0 && (maze[ind - cols] == ' ' || maze[ind - cols] == 'E'))
                successors.push_back(ind - cols);
        }

        if ((ind + cols) < rows * cols) {
            if (visited[ind + cols] == 0 && (maze[ind + cols] == ' ' || maze[ind + cols] == 'E'))
                successors.push_back(ind + cols);
        }

        if (ind > 1 && ind % cols > 0) {
            if (visited[ind - 1] == 0 && (maze[ind - 1] == ' ' || maze[ind - 1] == 'E'))
                successors.push_back(ind - 1);
        }

        if (ind + 1 < rows * cols && ind % cols < cols - 1) {
            if (visited[ind + 1] == 0 && (maze[ind + 1] == ' ' || maze[ind + 1] == 'E'))
                successors.push_back(ind + 1);
        }

    }

    //if (successors.size())
        return successors;
    //else
        //return predecessors;

}

void Maze::dfs(size_t ind) {

    path.push(ind);
    if (maze[ind] == 'E') { 

        // print the path and return on 
        // reaching the destination node 

        maze[path.peek()] = '*';

        std::cout << "Got here";

        while (path.pop()) {
            size_t x = path.peek();
            std::cout << x;
            maze[path.peek()] = '*';
        }

        return;

    }
    visited[ind] = 1;

    // if backtracking is taking place 
    if (!path.isEmpty()) {
        std::cout << "\n**********\n Ind:" << ind << "\n";
        for (auto a: getSuccessors(ind)) {
            // if the node is not visited 
            std::cout << a;
            if (visited[a] == 0)
                dfs(a);
        }
        std::cout << "\n**********\n";
    }

    path.pop();

}

bool Maze::backtrack(size_t ind) {

    dfs(ind);
    return true;

}

/*
bool Maze::backtrack(size_t ind){
    path.push(ind);
    if (maze[ind] == 'E') {

        // print the path and return on 
        // reaching the destination node 
        
        maze[path.peek()] = '*';

        while(path.pop())
            maze[path.peek()] = '*';

        return true;
    }
    visited[ind] = 1;

    // if backtracking is taking place 
    if (!visited.empty()) {
        for (int j = 0; j < visited.size(); j++) {
            // if the node is not visited 
            if (visited[ind] == 0)
                backtrack(ind);
        }
    }

    path.pop();
}


//return if we can solve the maze using recursion, path in visited
bool Maze::backtrack(std::size_t ind) {

    bool solvable = true;

    visited[ind] = 1;

    //if (maze[ind] == '*')
        ///maze[ind] = ' ';

    //if (maze[ind] != 'E' && maze[ind] != 'B' && visited[ind] == 0)
        //maze[ind] = '*';

    if (maze[ind] == 'E')
        return true;
    else {

        for (int i = 0; i < getSuccessors(ind).size(); i++)
            backtrack(getSuccessors(ind)[i]);
        /*
        if (maze[ind] == 'B')
            return false;

        if (getSuccessors(ind).size() == 0) {
            visited.clear();
            for (int i = 0; i < rows * cols; i++)
                visited.push_back(0);
            backtrack(ind);
        }
        */
        /*
        if (ind > cols) {
            if (maze[ind - cols] == '*') {
                maze[ind - cols] = ' ';
                backtrack(ind - cols);
            }
        }

        if ((ind + cols) < rows * cols) {
            if (maze[ind + cols] == '*') {
                maze[ind + cols] = ' ';
                backtrack(ind + cols);
            }
        }

        if (ind > 1) {
            if (maze[ind - 1] == '*') {
                maze[ind - 1] = ' ';
                backtrack(ind - 1);
            }
        }

        if (ind + 1 < rows * cols) {
            if (visited[ind + 1] == '*') {
                maze[ind + 1] = ' ';
                backtrack(ind + 1);
            }
        }
             

    }

    return false;
    
}*/

/**
 * Read a solved or unsolved maze from the PNG file.  Should overwrite existing maze, if any
 * @param filename - name of PNG file containing unsolved or solved maze
 * @return whether the maze could be read and is valid
 */
bool Maze::read(std::string filename)
{
  bool flag = false;
  std::vector<std::uint8_t> pixels = std::vector<std::uint8_t>(4*cols*rows);
  unsigned int w, h;
  
  //read png
  if(filename.empty())
    flag = lodepng::decode(pixels, w, h, "maze.png");
  else
    flag = lodepng::decode(pixels, w, h, filename);

  return false;
}

/**
 * Write the unsolved or solved mazed to a PNG file
 * @param filename - name of PNG file to write the maze to
 * @return false if the file can't be written
 */
bool Maze::write(std::string filename) const
{
  bool flag = false;

  std::vector<std::uint8_t> pixels = std::vector<std::uint8_t>(4*cols*rows,255); //RGBA values for each pixel in image

  //write png
  if(filename.empty())
    flag = lodepng::encode("maze.png", pixels, cols, rows);
  else
    flag = lodepng::encode(filename, pixels, cols, rows);
  
  return flag;
}

/**
 * Operator to print maze to std::cout (useful for debugging)
 */
std::ostream& operator<<(std::ostream& os, const Maze& m)
{
  for(int i = 0; i < m.rows*m.cols; i++)
    {
      if(i % m.cols == 0)
	os  << std::endl;
      
      os << m.maze[i];
    }

  os  << std::endl << std::endl;
  
  return os;
}

