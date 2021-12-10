# A General Backtracking Function (improved through dynamic programming) applied to the halloween puzzle.

# "Sanity check" function for the Halloween problem.
def halloween_check(current_state, decision_info, next_option):
    print(next_option)
    (state_ella, state_kingston) = current_state
    print(state_ella)
    if next_option == 0:
        if sum(state_ella[0]) + decision_info > state_ella[1]:
            return (False, None)
        else:
            return (True, ((state_ella[0] + [decision_info], state_ella[1]), state_kingston))
    else:
        if sum(state_kingston[0]) + decision_info > state_kingston[1]:
            return (False, None)
        else:
            return (True, (state_ella, (state_kingston[0] + [decision_info], state_kingston[1])))


# Recursive backtracking function that receives the options for the problem's sequence of decisions,
# a reference to a "sanity check" function, the current state of the problem, and, optionally, the current search depth.
# It returns (True, solution) if it finds a solution, i.e., a final problem state, or (False, None) if there is no solution.
def backtrack(decision_seq, check_func, current_state, depth=0):
    for next_option in decision_seq[depth][1]:
        (success, new_state) = check_func(current_state, decision_seq[depth][0], next_option)
        if success and depth < len(decision_seq) - 1:
            (success, new_state) = backtrack(decision_seq, check_func, new_state, depth + 1)
        if success:
            return (True, new_state)
    return (False, None)


# Finds a solution, i.e., a list of decisions, for a given halloween puzzle.
def halloween_solver(candy_list, capacity_ella, capacity_kingston):
    if sum(candy_list) > capacity_ella + capacity_kingston:
        print('Too much candy to carry for Ella and Kingston! PLease revise treats to avoid tricks.')
        return

    decision_seq = [(candy_item, [0, 1]) for candy_item in candy_list]
    (success, solution) = backtrack(decision_seq, halloween_check, (([], capacity_ella), ([], capacity_kingston)))

    if not success:
        print('There is no way for Ella and Kingston to divide and carry their candy. What a haunting experience!')
        return

    print("Problem solved:")
    print("Candy items in Ella's bag:     " + str(solution[0]))
    print("Candy items in Kingston's bag: " + str(solution[1]))
    print("Happy Halloween!")


halloween_solver([6.1, 1.3, 0.7, 2.9, 4.1, 2.7, 0.9, 3.6, 0.5, 1.2, 1.0], 15, 10)


def halloween_solver(candy_list, total_weight):
    candy_list = [int(x * 10) for x in candy_list]
    total_weight = 10 * total_weight
    #print(total_weight)
    n = len(candy_list)
    #dp = [[0 for i in range(n)] for j in range(total_weight)]
    dp = [[0 for i in range(total_weight)] for j in range(n)]
    for i in range(1, n):
        for j in range(0, total_weight):
            if candy_list[i] > j:
                #print(j)
                dp[i][j] = dp[i-1][j]
            else:
                #print(j - candy_list[i])
                dp[i][j] = max(dp[i-1][j], candy_list[i] + dp[i-1][j - candy_list[i]])
    print(dp)
    return dp[n-1][total_weight-1]

print(halloween_solver([6.1, 1.3, 0.7, 2.9, 4.1, 2.7, 0.9, 3.6, 0.5, 1.2, 1.0], 15))
