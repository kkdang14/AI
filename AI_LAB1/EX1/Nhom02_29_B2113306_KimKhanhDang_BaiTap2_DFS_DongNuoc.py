# tankcapacityX = 9 
# tankcapacityY = 4 
# empty = 0
# goal = 6 
# Maxlength = 100  

# action = [
#     "First State",
#     "pour Water Full X",
#     "pour Water Full Y",
#     "pour Water Empty X",
#     "pour Water Empty Y",
#     "pour Water X to Y",
#     "pour Water Y to X"
# ]

# class State:
#     def __init__(self, x, y):
#         self.x = x
#         self.y = y

# def makeNullState():
#     return State(0, 0)

# def printState(state):
#     print("\n X:", state.x, "--- Y:", state.y)

# def goalCheck(state):
#     return state.x == goal or state.y == goal

# def pourWaterFullX(curState, result):
#     if curState.x < tankcapacityX:
#         result.x = tankcapacityX
#         result.y = curState.y
#         return 1
#     return 0

# def pourWaterFullY(curState, result):
#     if curState.y < tankcapacityY:
#         result.y = tankcapacityY
#         result.x = curState.x
#         return 1
#     return 0


# def pourWaterEmptyX(curState, result):
#     if curState.x > 0:
#         result.x = 0
#         result.y = curState.y
#         return 1
#     return 0


# def pourWaterEmptyY(curState, result):
#     if curState.y > 0:
#         result.y = 0
#         result.x = curState.x
#         return 1
#     return 0


# def pourWaterXY(curState, result):
#     if curState.x > 0 and curState.y < tankcapacityY:
#         result.x = max(curState.x - (tankcapacityY - curState.y), empty)
#         result.y = min(curState.y + curState.x, tankcapacityY)
#         return 1
#     return 0


# def pourWaterYX(curState, result):
#     if curState.y > 0 and curState.x < tankcapacityX:
#         result.x = min(curState.x + curState.y, tankcapacityX)
#         result.y = max(curState.y - (tankcapacityX - curState.x), empty)
#         return 1
#     return 0


# def callOperator(curState, result, option):
#     if option == 1:
#         return pourWaterFullX(curState, result)
#     elif option == 2:
#         return pourWaterFullY(curState, result)
#     elif option == 3:
#         return pourWaterEmptyX(curState, result)
#     elif option == 4:
#         return pourWaterEmptyY(curState, result)
#     elif option == 5:
#         return pourWaterXY(curState, result)
#     elif option == 6:
#         return pourWaterYX(curState, result)
#     else:
#         print("\n--- Error calls operators ---")
#         return 0


# class Node:
#     def __init__(self, state, parent, noFunction):
#         self.state = state
#         self.parent = parent
#         self.noFunction = noFunction


# def compareStates(a, b):
#     return a.x == b.x and a.y == b.y


# def findState(state, stack):
#     for node in stack:
#         if compareStates(node.state, state):
#             return 1
#     return 0



# def DFS_Algorithm(state):
#     openStack, closeStack = [], []
#     root = Node(state, None, 0)
#     openStack.append(root)

#     while len(openStack):
#         node = openStack[-1]
#         openStack.pop() #openQueue.pop(0)-- BFS
#         closeStack.append(node)
#         if goalCheck(node.state):
#             return node
#         for opt in range(1, 7):
#             newState = makeNullState()
#             if callOperator(node.state, newState, opt):
#                 if findState(newState, closeStack) or findState(newState, openStack):
#                     continue
#                 newNode = Node(newState, node, opt)
#                 openStack.append(newNode)
#     return None

# def printWaysToGetGoal(node):
#     stackPrint = []
#     while node is not None:
#         stackPrint.append(node)
#         node = node.parent

#     noAction = 0
#     while len(stackPrint):
#         print("\nAction", noAction, ":", action[stackPrint[-1].noFunction])
#         printState(stackPrint[-1].state)
#         stackPrint.pop()
#         noAction += 1


# def main():
#     curState = State(0, 0)
#     p = DFS_Algorithm(curState)
#     printWaysToGetGoal(p)


# if __name__ == "__main__":
#     main()

TANK_CAPACITY_X = 9
TANK_CAPACITY_Y = 4
GOAL = 6

ACTIONS = [
    "Initial State",
    "Fill X",
    "Fill Y",
    "Empty X",
    "Empty Y",
    "Pour X to Y",
    "Pour Y to X"
]

class State:
    def __init__(self, x, y):
        self.x = x
        self.y = y

def pour_water(source, target, cur_state):
    max_pour = min(cur_state[source], TANK_CAPACITY_Y - cur_state[target])
    new_state = cur_state[:]
    new_state[source] -= max_pour
    new_state[target] += max_pour
    return State(*new_state)

POUR_OPERATORS = [
    lambda state: State(TANK_CAPACITY_X, state.y),                  # Fill X
    lambda state: State(state.x, TANK_CAPACITY_Y),                  # Fill Y
    lambda state: State(0, state.y),                                 # Empty X
    lambda state: State(state.x, 0),                                 # Empty Y
    lambda state: pour_water(0, 1, state),                          # Pour X to Y
    lambda state: pour_water(1, 0, state)                           # Pour Y to X
]

class Node:
    def __init__(self, state, parent, action_index):
        self.state = state
        self.parent = parent
        self.action_index = action_index

def dfs_algorithm(initial_state):
    open_stack, close_stack = [], []
    root = Node(initial_state, None, 0)
    open_stack.append(root)

    while open_stack:
        node = open_stack.pop()
        close_stack.append(node)
        if node.state.x == GOAL or node.state.y == GOAL:
            return node
        for action_index, operator in enumerate(POUR_OPERATORS, start=1):
            new_state = operator(node.state)
            if new_state in [n.state for n in close_stack]:
                continue
            new_node = Node(new_state, node, action_index)
            open_stack.append(new_node)
    return None

def print_ways_to_get_goal(node):
    stack_print = []
    while node is not None:
        stack_print.append(node)
        node = node.parent

    action_count = 0
    while stack_print:
        print(f"Action {action_count}: {ACTIONS[stack_print[-1].action_index]}")
        print(f"X: {stack_print[-1].state.x} --- Y: {stack_print[-1].state.y}")
        stack_print.pop()
        action_count += 1

def main():
    initial_state = State(0, 0)
    goal_node = dfs_algorithm(initial_state)
    if goal_node:
        print_ways_to_get_goal(goal_node)
    else:
        print("No solution found.")

if __name__ == "__main__":
    main()
