#using the A Star algorithm to create Anagrams out of the two words. 
global num_iterations


def longest_common_subsequence(X, Y):
    # use dynamic programming to find the longest common subsequence
    dp = [[0] * (len(X) + 1) for i in range(len(Y) + 1)]
    lcs = 0
    substr_ind = 0
    for i in range(1, len(X) + 1):
        for j in range(1, len(Y) + 1):
            if X[i - 1] == Y[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
                if dp[i][j] > lcs:
                    lcs = dp[i][j]
                    substr_ind = i
    return lcs, substr_ind


def heuristic(state, goal):
    # the heuristic uses the length of the longest common subsequence, along with
    # the distance between the common subsequence in the goal state and the current state.
    lcs, substr_ind = longest_common_subsequence(state, goal)
    substr = state[substr_ind-lcs:substr_ind]
    substr_ind = substr_ind - lcs
    goal_ind = goal.find(substr)
    num_moves = abs(goal_ind - substr_ind)
    if(goal_ind < substr_ind):
        num_moves = abs(substr_ind - len(goal)) + abs(goal_ind - 0)
    h = len(state) - lcs + num_moves
    return h


def anagram_expand(state, goal):
    node_list = []

    for pos in range(len(state) - 1):  # Create each possible state that can be created from the current one in a single step
        new_state = state[:pos] + state[-1:] + state[pos:-1]

        # Very simple h' function - please improve!
        if new_state == goal:
            score = 0
        else:
            score = heuristic(new_state, goal)

        node_list.append((new_state, score))

    return node_list


def a_star(start, goal, expand):
    global num_iterations

    open_list = [([start], -1)]
    while open_list:
        num_iterations += 1
        open_list.sort(key=lambda x: len(x[0]) + x[1])
        if open_list[0][1] == 0:
            return open_list[0][0]

        ancestors = open_list[0][0]
        open_list = open_list[1:]
        new_nodes = expand(ancestors[-1], goal)

        for (new_state, score) in new_nodes:
            append_new_node = True
            for ancestor in ancestors:
                if new_state == ancestor:  # Modified from Assignment #2 solutions to avoid numpy
                    append_new_node = False
                    break

            if append_new_node:
                open_list.append((ancestors + [new_state], score))
    return []


# Finds a solution, i.e., a placement of all rectangles within the given field, for a rectangle puzzle
def anagram_solver(start, goal):
    global num_iterations
    num_iterations = 0

    # Add code here to check in advance whether the problem is solvable
    if ''.join(sorted(start)) != ''.join(sorted(goal)):
        print('This is clearly impossible. I am not even trying to solve this.')
        return

    solution = a_star(start, goal, anagram_expand)

    if not solution:
        print('No solution found. This is weird, I should have caught this before even trying A*.')
        return

    print(str(len(solution) - 1) + ' steps from start to goal:')

    for step in solution:
        print(step)

    print(str(num_iterations) + ' A* iterations were performed to find this solution.')

anagram_solver('CRATE', 'TRACE')
anagram_solver("ALLERGY", "LARGELY")
anagram_solver('RESCUE', 'SECURE')

anagram_solver('PREDATOR', 'TEARDROP')

anagram_solver('SCHOOLMASTER', 'THECLASSROOM')
