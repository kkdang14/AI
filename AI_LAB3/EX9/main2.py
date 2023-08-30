N = 9
EMPTY = 0
exploredCounter = 0

def read_input(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    return [[int(num) for num in line.split()] for line in lines]
def print_sudoku(board):
    print("Sudoku:")
    print("----------------------")
    for i in range(N):
        if i % 3 == 0 and i != 0:
            print("----------------------")
        for j in range(N):
            if j % 3 == 0 and j != 0:
                print("|", end=" ")
            print(board[i][j], end=" ")
        print()
    print("----------------------")

def is_valid(board, row, col, num):
    for i in range(N):
        if board[row][i] == num or board[i][col] == num:
            return False
    start_row, start_col = 3 * (row // 3), 3 * (col // 3)
    for i in range(3):
        for j in range(3):
            if board[start_row + i][start_col + j] == num:
                return False
    return True

def solve_sudoku(board):
    global exploredCounter  # Use the global counter
    for row in range(N):
        for col in range(N):
            if board[row][col] == EMPTY:
                for num in range(1, N + 1):
                    if is_valid(board, row, col, num):
                        board[row][col] = num
                        exploredCounter += 1  # Increment the counter
                        if solve_sudoku(board):
                            return True
                        board[row][col] = EMPTY
                return False
    return True

if __name__ == '__main__':
    input_filename = "input.txt"
    input1 = read_input(input_filename)

    board = input1.copy()

    if solve_sudoku(board):
        print("SOLVED")
    else:
        print("CAN NOT SOLVE")

    print("Explored states:", exploredCounter)
    print_sudoku(board)
