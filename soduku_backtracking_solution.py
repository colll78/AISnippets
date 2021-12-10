# A backtracking solution for the n-queens problem and soduku.

import copy
import numpy as np

example_1 = np.array([[1, 0, 8, 5, 3, 0, 7, 0, 0],
                      [2, 0, 6, 0, 0, 8, 0, 3, 0],
                      [0, 0, 0, 0, 0, 4, 2, 5, 8],
                      [0, 0, 0, 0, 0, 5, 8, 1, 0],
                      [4, 2, 0, 0, 0, 0, 0, 6, 3],
                      [0, 8, 3, 2, 0, 0, 0, 0, 0],
                      [3, 6, 2, 4, 0, 0, 0, 0, 0],
                      [0, 7, 0, 6, 0, 0, 3, 0, 9],
                      [0, 0, 4, 0, 7, 3, 6, 0, 5]])

example_2 = np.array([[0, 0, 0, 0, 3, 0, 2, 8, 0],
                      [7, 0, 0, 5, 0, 0, 0, 1, 0],
                      [3, 0, 0, 0, 6, 0, 0, 0, 0],
                      [0, 8, 0, 0, 0, 2, 0, 4, 0],
                      [1, 0, 0, 0, 5, 0, 0, 0, 2],
                      [0, 6, 0, 9, 0, 0, 0, 3, 0],
                      [0, 0, 0, 0, 2, 0, 0, 0, 4],
                      [0, 4, 0, 0, 0, 6, 0, 0, 1],
                      [0, 9, 2, 0, 7, 0, 0, 0, 0]])


# "Sanity check" function for the n-queens problem.
# For a given search state (node), it checks whether the next decision could possibly lead to a solution 
def queens_check(current_state, decision_info, next_option):
    for column in range(decision_info):
        row_distance = abs(current_state[column] - next_option)
        if row_distance == 0 or row_distance == decision_info - column:
            return (False, None)
    new_state = copy.deepcopy(current_state)
    new_state[decision_info] = next_option
    return (True, new_state)


def sudoku_check(current_state, decision_info, next_option):
    i, j = decision_info
    if not can_place(current_state, next_option, i, j):
        return (False, None)
    new_state = copy.deepcopy(current_state)
    new_state[decision_info[0]][decision_info[1]] = next_option
    return (True, new_state)

# Recursive backtracking function that receives the options for the problem's sequence of decisions,
# a reference to a "sanity check" function and, optionally, the current search path.
# It returns the result as a pair (success indicator, list of n row indices for queen placement).
def backtrack(decision_seq, check_func, current_state, depth=0):
    for next_option in decision_seq[depth][1]:
        (success, new_state) = check_func(current_state, decision_seq[depth][0], next_option)
        # print('  ' * depth + 'Check state ' + str(current_state) + ' with next option ' + str(next_option) + '-> ' + str(success))
        if success and depth < len(decision_seq) - 1:
            (success, new_state) = backtrack(decision_seq, check_func, new_state, depth + 1)
        if success:
            return (True, new_state)
    return (False, None)


# Finds a solution for the n-queens problem, i.e., a pair (success indicator, list of n row indices for queen placement)
def n_queens_solver(n):
    initial_state = [0 for i in range(n)]
    decision_seq = [(i, list(range(1, n + 1))) for i in range(n)]
    return backtrack(decision_seq, queens_check, initial_state)


def can_place(board, n, r, c):
    #checking in row
    for i in range(0, board.shape[0]):
        #there is a cell with same value
        if board[r][i] == n:
            return False
    #checking in column
    for i in range(0,board.shape[0]):
        #there is a cell with same value
        if board[i][c] == n:
            return False
    row_start = (r//3)*3
    col_start = (c//3)*3
    #checking box
    for i in range(row_start, row_start+3):
        for j in range(col_start, col_start+3):
            if board[i][j] == n:
                return False
    return True


def used_in_box(board, row, col, num):
    row_start = (row // 3) * 3
    col_start = (col // 3) * 3
    # checking box
    for i in range(row_start, row_start + 3):
        for j in range(col_start, col_start + 3):
            if board[i][j] == num:
                return True
    return False


def filter_possible(n, possible, row, column):
    filtered = []
    for x in possible:
        if x not in n[:,column] and x not in n[row,:] and not used_in_box(n, row, column, x):
                filtered.append(x)
    return filtered


def sudoku_solver(n):
    decision_seq = []
    for i in range(0, n.shape[0]):
        for j in range(0, n.shape[1]):
            if n[i][j] == 0:
                possible = [x+1 for x in range(0, n.shape[0])]
                possible = filter_possible(n, possible, i, j)
                if len(possible) == 1:
                    n[i][j] = possible[0]
                else:
                    decision_seq.append(((i, j), possible))
    # for x in decision_seq:
    #     print(x)
    return backtrack(decision_seq, sudoku_check, n)

# Solves the n-queens problem and prints the solution in ASCII format 
def n_queens_ascii(n):
    (success, solution) = n_queens_solver(n)
    if not success:
        print('Sorry, there is no solution to the %d-queens problem.'%(n))
    else:
        print('Solution: ' + str(solution))
        for y in range(1, n + 1):
            for x in range(0, n):
                if solution[x] == y:
                    print('Q', end=' ')
                else:
                    print('.', end=' ')
            print('')

result = sudoku_solver(example_2)
print(result)
