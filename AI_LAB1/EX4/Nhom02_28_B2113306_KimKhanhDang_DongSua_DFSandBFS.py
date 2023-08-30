from queue import Queue

default_X = 10
default_Y = 5
default_Z = 6
empty_tank = 0

class State:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

def makeNullState(state):
    state.x = 0
    state.y = 0
    state.z = 0

def printState(state):
    print(f"\n     X: {state.x} --- Y: {state.y} --- Z: {state.z}")

def checkGoal(state):
    return state.x == 8

def pourMilkXtoY(cur_state, result):
    if cur_state.x > 0 and 0 <= cur_state.y < default_Y:
        result.x = max(cur_state.x - (default_Y - cur_state.y), empty_tank)
        result.y = min(cur_state.x + cur_state.y, default_Y)
        result.z = cur_state.z
        return 1
    return 0

def pourMilkXtoZ(cur_state, result):
    if cur_state.x > 0 and 0 <= cur_state.z < default_Z:
        result.x = max(cur_state.x - (default_Z - cur_state.z), empty_tank)
        result.z = min(cur_state.x + cur_state.z, default_Z)
        result.y = cur_state.y
        return 1
    return 0

def pourMilkYtoZ(cur_state, result):
    if cur_state.y > 0 and 0 <= cur_state.z < default_Z:
        result.y = max(cur_state.y - (default_Z - cur_state.z), empty_tank)
        result.z = min(cur_state.y + cur_state.z, default_Z)
        result.x = cur_state.x
        return 1
    return 0

def pourMilkZtoY(cur_state, result):
    if cur_state.z > 0 and 0 <= cur_state.y < default_Y:
        result.z = max(cur_state.z - (default_Y - cur_state.y), empty_tank)
        result.y = min(cur_state.z + cur_state.y, default_Y)
        result.x = cur_state.x
        return 1
    return 0

def pourMilkYtoX(cur_state, result):
    if cur_state.y > 0 and 0 <= cur_state.x < default_X:
        result.y = max(cur_state.y - (default_X - cur_state.x), empty_tank)
        result.x = min(cur_state.y + cur_state.x, default_X)
        result.z = cur_state.z
        return 1
    return 0

def pourMilkZtoX(cur_state, result):
    if cur_state.z > 0 and 0 <= cur_state.x < default_X:
        result.z = max(cur_state.z - (default_X - cur_state.x), empty_tank)
        result.x = min(cur_state.z + cur_state.x, default_X)
        result.y = cur_state.y
        return 1
    return 0

action = [
    "First State",
    "Pour Milk X to Y",
    "Pour Milk X to Z",
    "Pour Milk Y to Z",
    "Pour Milk Z to Y",
    "Pour Milk Y to X",
    "Pour Milk Z to X"
]

def call_operator(cur_state, result, option):
    if option == 1: return pourMilkXtoY(cur_state, result)
    if option == 2: return pourMilkXtoZ(cur_state, result)
    if option == 3: return pourMilkYtoZ(cur_state, result)
    if option == 4: return pourMilkZtoY(cur_state, result)
    if option == 5: return pourMilkYtoX(cur_state, result)
    if option == 6: return pourMilkZtoX(cur_state, result)
    print("Error when call operator!!!")
    return 0

def compareStates(openStack, state):
    return openStack.x == state.x and openStack.y == state.y and openStack.z == state.z

class Node:
    def __init__(self, state, parent, no_func):
        self.state = state
        self.Parent = parent
        self.no_func = no_func

def find_State(state, openStack):
    for node in openStack:
        if compareStates(node.state, state):
            return True
    return False

def BFS(state):
    BFS_Open = Queue()
    BFS_Close = []
    root = Node(state, None, 0)
    BFS_Open.put(root)

    while not BFS_Open.empty():
        node = BFS_Open.get()
        BFS_Close.append(node)

        if checkGoal(node.state):
            return node

        for opt in range(1, 7):
            newstate = State(0, 0, 0)
            makeNullState(newstate)

            if call_operator(node.state, newstate, opt) and not (find_State(newstate, BFS_Close) or find_State(newstate, BFS_Open.queue)):
                new_node = Node(newstate, node, opt)
                BFS_Open.put(new_node)

    return None

def DFS(state):
    DFS_Open = []
    DFS_Close = []
    root = Node(state, None, 0)
    DFS_Open.append(root)

    while DFS_Open:
        node = DFS_Open.pop()
        DFS_Close.append(node)

        if checkGoal(node.state):
            return node

        for opt in range(6, 0, -1):
            newstate = State(0, 0, 0)
            makeNullState(newstate)

            if call_operator(node.state, newstate, opt) and not (find_State(newstate, DFS_Close) or find_State(newstate, DFS_Open)):
                new_node = Node(newstate, node, opt)
                DFS_Open.append(new_node)

    return None

def print_WaysToGetGoal(node):
    printStack = []
    while node.Parent is not None:
        printStack.append(node)
        node = node.Parent
    printStack.append(node)
    no_act = 0
    for node in reversed(printStack):
        print(f"Action: {no_act}. {action[node.no_func]}")
        printState(node.state)
        no_act += 1

def main():
    cur_state = State(10, 0, 0)
    print("DFS_Algorithm\n")
    p = DFS(cur_state)
    print_WaysToGetGoal(p)
    print("/-------------------------------------------------------/\n")
    print("BFS_Algorithm\n")
    q = BFS(cur_state)
    print_WaysToGetGoal(q)

if __name__ == "__main__":
    main()
