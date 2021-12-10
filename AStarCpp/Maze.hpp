#ifndef _MAZE_HPP_
#define _MAZE_HPP_

#include "Stack.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

class Maze
{
private:
  //size of maze
  std::size_t rows;
  std::size_t cols;
  //maze is stored internally as one-dimensional array
  std::vector<unsigned char> maze; //we'll use unsigned char instead of uint8_t to imply that we're storing chars for printing
  //whether we have already visited a cell (useful for both generating and solving mazes)
  std::vector<uint8_t> visited; //std::vector<bool> is not a standard container...may get odd behavior

  Stack<size_t> path;

  //BEGIN: OPTIONAL
  std::vector<std::size_t> getNeighbors(std::size_t ind) const; //return indices of neighboring cells (ordered top, bottom, left, right)
  int getWall(std::size_t ind1, std::size_t ind2) const; //return index of wall between neighboring cells, -1 if no wall
  std::vector<std::size_t> getSuccessors(std::size_t ind) const; //successors of ind (ordered top, bottom, left, right)
  bool backtrack(std::size_t ind); //return if we can solve the maze using recursion, path in visited
  //END: OPTIONAL

  void dfs(size_t ind);
  
public:
  /**
   * Create a blank maze of size 0-by-0
   * @return maze with rows=0 and cols=0
   */ 
  Maze();

  /**
   * Constructor will generate a random maze given input parameters.  Throws exception if maze parameters imply/yield an invalid maze.
   * @param rows - height of maze
   * @param cols - width of maze
   * @param begin - entry point in maze
   * @param end - exit point in maze
   */ 
  Maze(std::size_t rows, std::size_t cols, std::size_t begin, std::size_t end) throw(std::invalid_argument);
  
  /**
   * Function to set the maze we want to solve, validate it, and set private variables appropriately (a valid maze
   * will only have characters listed in the project description for an unsolved maze). Should overwrite existing maze, if any.
   * @param maze - a one dimensional vector representing an unsolved maze
   * @param rows - the number of rows in the maze
   * @return whether the maze is valid
   */ 
  bool set(const std::vector<unsigned char>& maze, std::size_t rows);
  
  /**
   * Return the maze
   * @return an unsolved or solved maze, depending on whether it has been solved or not
   */
  const std::vector<unsigned char>& get() const;

  /**
   * Generate a new maze with the given parameters.  Should overwrite existing maze, if any.
   * @param rows - height of maze
   * @param cols - width of maze
   * @param begin - entry point in maze
   * @param end - exit point in maze
   * @return true if we could create a valid maze
   */
  bool create(std::size_t rows, std::size_t cols, std::size_t begin, std::size_t end);

  /**
   * Find the path from B to E in the current maze and record the path in the maze
   * @return false if no path could be found from B to E or error, true otherwise
   */
  bool solve();

  /**
   * Read a solved or unsolved maze from the PNG file.  Should overwrite existing maze, if any
   * @param filename - name of PNG file containing unsolved or solved maze 
   * @return whether the maze could be read and is valid
   */
  bool read(std::string filename);

  /**
   * Write the unsolved or solved mazed to a PNG file
   * @param filename - name of PNG file to write the maze to
   * @return false if the file can't be written
   */
  bool write(std::string filename) const;
  
  /**
   * Operator to print maze to std::cout (useful for debugging)
   */
  friend std::ostream& operator<<(std::ostream& os, const Maze& m);
};
#endif
