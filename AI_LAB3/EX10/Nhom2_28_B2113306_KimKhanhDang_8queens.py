# In ra mot trong cac trang thai kha thi

def is_safe(board, row, col):
    for i in range(row):
        if board[i][col] == 1:
            return False

    for i, j in zip(range(row, -1, -1), range(col, -1, -1)):
        if board[i][j] == 1:
            return False

    for i, j in zip(range(row, -1, -1), range(col, len(board))):
        if board[i][j] == 1:
            return False

    return True

def solve_n_queens(board, row):
    if row == len(board):
        print_solution(board)
        return True

    for col in range(len(board)):
        if is_safe(board, row, col):
            board[row][col] = 1
            if solve_n_queens(board, row + 1):
                return True
            board[row][col] = 0

def print_solution(board):
    for row in board:
        print(" ".join(["Q" if cell == 1 else "." for cell in row]))
    print("\n")
n = 8
chessboard = [[0 for _ in range(n)] for _ in range(n)]

if __name__ == '__main__':
    if not solve_n_queens(chessboard, 0):
        print('NONE SOLUTION')


# # In ra tat ca cac trang thai kha thi
# def is_safe(board, row, col):
#     for i in range(row):
#         if board[i][col] == 1:
#             return False
#
#     for i, j in zip(range(row, -1, -1), range(col, -1, -1)):
#         if board[i][j] == 1:
#             return False
#
#     for i, j in zip(range(row, -1, -1), range(col, len(board))):
#         if board[i][j] == 1:
#             return False
#
#     return True
#
# def solve_n_queens(board, row):
#     if row == len(board):
#         print_solution(board)
#         return True
#
#     for col in range(len(board)):
#         if is_safe(board, row, col):
#             board[row][col] = 1
#             solve_n_queens(board, row + 1)
#             board[row][col] = 0
#
# def print_solution(board):
#     for row in board:
#         print(" ".join(["Q" if cell == 1 else "." for cell in row]))
#     print("\n")
# n = 8
# chessboard = [[0 for _ in range(n)] for _ in range(n)]
# solve_n_queens(chessboard, 0)