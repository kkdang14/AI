# 1 : Ngửa
# 0 : Úp

class State:
    def __init__(self, orientations, parent=None):
        self.orientations = orientations
        self.parent = parent

    def is_goal_state(self):
        return all(orientation == '1' for orientation in self.orientations)

    def generate_successors(self):
        successors = []
        for i in range(len(self.orientations) - 2):
            new_orientations = self.orientations[:i] + ['1' if o == '0' else '0' for o in self.orientations[i:i+3]] + self.orientations[i+3:]
            successors.append(State(new_orientations, parent=self))
        return successors

def DFS(initial_state):
    open_list = [initial_state]
    closed_set = set()

    while open_list:
        current_state = open_list.pop()
        closed_set.add(tuple(current_state.orientations))

        if current_state.is_goal_state():
            return current_state

        successors = current_state.generate_successors()
        for successor in successors:
            if tuple(successor.orientations) not in closed_set:
                open_list.append(successor)

    return None

def BFS(initial_state):
    open_list = [initial_state]
    closed_set = set()

    while open_list:
        current_state = open_list.pop(0)
        closed_set.add(tuple(current_state.orientations))

        if current_state.is_goal_state():
            return current_state

        successors = current_state.generate_successors()
        for successor in successors:
            if tuple(successor.orientations) not in closed_set:
                open_list.append(successor)

    return None

def print_solution_path(solution_state):
    path = []
    while solution_state is not None:
        path.append(solution_state.orientations)
        solution_state = solution_state.parent
    path.reverse()
    return path

initial_orientations = ['1', '0', '1', '0', '1', '0']
print("Init state:")
initial_state = State(initial_orientations)

if __name__ == "__main__":
    result = BFS(initial_state)
    if result:
        solution_path = print_solution_path(result)
        for step, orientations in enumerate(solution_path):
            print(f"Action {step}:   {orientations}")
    else:
        print("Không tìm thấy cách để đạt được trạng thái mục tiêu.")
