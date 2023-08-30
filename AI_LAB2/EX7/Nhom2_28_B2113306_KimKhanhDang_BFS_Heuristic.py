ROW = 3
COL = 3
GOAL = ((0, 1, 2), (3, 4, 5), (6, 7, 8))
FIRST_STATE = [[3, 4, 5], [1, 0, 2], [6, 7, 8]]
dr = (0, 1, 0, -1)
dc = (1, 0, -1, 0)

action = [
    "move cell empty to Right", "move cell empty to Down",
    "move cell empty to Left", "move cell empty to Up",
    "first state"
]


class State:
    def __init__(self, data = FIRST_STATE, row=None, col=None):
        self.data = [list(row) for row in data]
        self.row = row
        self.col = col

    def as_tuple(self):
        return tuple(tuple(row) for row in self.data)

    def goal_check(self):
        return tuple(tuple(row) for row in self.data) == GOAL

    def update_position_cell_empty(self):
        for r, row in enumerate(self.data):
            for c, col in enumerate(row):
                if col == 0:
                    self.row = r
                    self.col = c


def print_state(state):
    for row in state.data:
        print(row)


def heuristic(state):
    cost = 0
    for r in range(ROW):
        for c in range(COL):
            value = state.data[r][c]
            goal_row, goal_col = divmod(value, COL)
            cost += abs(r - goal_row) + abs(c - goal_col)
    return cost


def move(state, r, c):
    row, col = state.row, state.col
    rnew, cnew = row + r, col + c
    if 0 <= rnew < ROW and 0 <= cnew < COL:
        data = [list(row) for row in state.data]
        data[row][col], data[rnew][cnew] = data[rnew][cnew], data[row][col]
        new_state = State(data, rnew, cnew)
        return new_state
    return None


class Node:
    def __init__(self, state, parent, noFunction, cost, id):
        self.state = state
        self.parent = parent
        self.noFunction = noFunction
        self.cost = cost
        self.id = id

    def priority(self):
        return heuristic(self.state)

    def __lt__(self, other):
        return self.priority() < other.priority() or (self.priority() == other.priority() and self.id < other.id)

def best_first_search(state):
    open_set, close_set = [Node(state, None, -1, 0, 0)], set()
    close_set.add(state.as_tuple())
    id_counter = 1
    while open_set:
        open_set.sort(key=lambda node: node.priority())
        node = open_set.pop(0)
        if node.state.goal_check():
            return node
        for i, (r, c) in enumerate(zip(dr, dc)):
            if node.noFunction == (i + 2) % 4:
                continue
            new_state = move(node.state, r, c)
            if new_state is None:
                continue
            data_tuple = new_state.as_tuple()
            if data_tuple not in close_set:
                open_set.append(Node(new_state, node, i, node.cost + 1, id_counter))
                close_set.add(data_tuple)
                id_counter += 1
    return None


def print_path(node):
    path = []
    while node:
        path.append(node)
        node = node.parent
    path.reverse()
    for i, item in enumerate(path):
        print("\nAction {}: {}".format(i, action[item.noFunction]))
        print_state(item.state)


if __name__ == "__main__":
    state = State()
    state.update_position_cell_empty()
    solution_node = best_first_search(state)
    if solution_node:
        print_path(solution_node)
    else:
        print("No solution found.")
