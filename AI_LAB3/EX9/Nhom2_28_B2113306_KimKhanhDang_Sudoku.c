#include <stdio.h>
#include <stdlib.h>
#define EMPTY 0
#define SQUARE 3
#define INF 999999
#define MAX_VALUE 10
#define Maxlength 100
#define NB_ROWS 9
#define NB_COLS 9

typedef struct {
    int x, y;
} Coord;

typedef struct {
    Coord data[Maxlength];
    int size;
} listCoord;

typedef struct {
    int data[NB_ROWS * NB_COLS][NB_ROWS * NB_COLS];
    int n;
} Constrains;

typedef struct {
    int cells[NB_ROWS][NB_COLS];
    Constrains cons;
} Sudoku;

void initListCoord(listCoord *list) {
    list->size = 0;
}

void appendListCoord(listCoord *list, Coord coord) {
    list->data[list->size++] = coord;
}

int indexOf(Coord coord) {
    return (NB_ROWS * coord.x + coord.y);
}

Coord posOfVertex(int vertex) {
    Coord coord;
    coord.x = vertex / NB_ROWS;
    coord.y = vertex % NB_ROWS;
    return coord;
}

int addConstrain(Constrains *Constrains, Coord source, Coord target) {
    int u = indexOf(source);
    int v = indexOf(target);
    if (Constrains->data[u][v] == 0) {
        Constrains->data[u][v] = 1;
        Constrains->data[v][u] = 1;
        return 1;
    }
    return 0;
}

listCoord getConstrains(Constrains Constrains, Coord coord) {
    int i;
    int v = indexOf(coord);
    listCoord result;
    initListCoord(&result);
    for (i = 0; i < Constrains.n; i++) {
        if (Constrains.data[v][i] == 1) {
            appendListCoord(&result, posOfVertex(i));
        }
    }
    return result;
}

void initConstrains(Constrains *Constrains) {
    int i, j;
    for (i = 0; i < NB_ROWS * NB_COLS; i++) {
        for (j = 0; j < NB_ROWS * NB_COLS; j++) {
            Constrains->data[i][j] = 0;
        }
    }
    Constrains->n = NB_ROWS * NB_COLS;
}

void initSudoku(Sudoku *sudoku) {
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLS; j++) {
            sudoku->cells[i][j] = EMPTY;
        }
    }
    initConstrains(&sudoku->cons);
}

void initSudokuWithValues(Sudoku *sudoku, int inputs[NB_ROWS][NB_COLS]) {
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLS; j++) {
            sudoku->cells[i][j] = inputs[i][j];
        }
    }
    initConstrains(&sudoku->cons);
}

void printSudoku(Sudoku sudoku) {
    int i, j;
    printf("Sudoku:\n");
    printf("----------------------\n");
    for (i = 0; i < NB_ROWS; i++) {
        if (i % SQUARE == 0 && i != 0) {
            printf("----------------------\n");
        }
        for (j = 0; j < NB_COLS; j++) {
            if (j % SQUARE == 0 && j != 0) {
                printf("| ");
            }
            printf("%d ", sudoku.cells[i][j]);
        }
        printf("\n");
    }
    printf("----------------------\n");
}

int isFull(Sudoku sudoku) {
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLS; j++) {
            if (sudoku.cells[i][j] == EMPTY) {
                return 0;
            }
        }
    }
    return 1;
}

void spreadConstrainsFrom(Coord position, Constrains *cons, listCoord *changed) {
    int row = position.x, col = position.y;
    int i, j;

    for (i = 0; i < NB_ROWS; i++) {
        if (i != row) {
            Coord pos = {i, col};
            if (addConstrain(cons, position, pos)) {
                appendListCoord(changed, pos);
            }
        }
    }

    for (j = 0; j < NB_COLS; j++) {
        if (j != col) {
            Coord pos = {row, j};
            if (addConstrain(cons, position, pos)) {
                appendListCoord(changed, pos);
            }
        }
    }

    for (i = 0; i < SQUARE; i++) {
        for (j = 0; j < SQUARE; j++) {
            int areaX = (row / SQUARE) * SQUARE;
            int areaY = (col / SQUARE) * SQUARE;
            if (areaX + i != row || areaY + j != col) {
                Coord pos = {areaX + i, areaY + j};
                if (addConstrain(cons, position, pos)) {
                    appendListCoord(changed, pos);
                }
            }
        }
    }
}

listCoord getAvailableValues(Coord position, Sudoku sudoku) {
    listCoord posList = getConstrains(sudoku.cons, position);
    int availables[MAX_VALUE];
    int i;
    for (i = 1; i < MAX_VALUE; i++) {
        availables[i] = 1;
    }
    for (i = 0; i < posList.size; i++) {
        Coord pos = posList.data[i];
        if (sudoku.cells[pos.x][pos.y] != EMPTY) {
            availables[sudoku.cells[pos.x][pos.y]] = 0;
        }
    }

    listCoord result;
    initListCoord(&result);
    for (i = 1; i < MAX_VALUE; i++) {
        if (availables[i]) {
            appendListCoord(&result, posOfVertex(i));
        }
    }

    return result;
}

// Coord getNextEmptyCell(Sudoku sudoku) {
//     int i, j;
//     for (i = 0; i < NB_ROWS; i++) {
//         for (j = 0; j < NB_COLS; j++) {
//             Coord pos = {i, j};
//             if (sudoku.cells[i][j] == EMPTY) {
//                 return pos;
//             }
//         }
//     }
//     Coord emptyPos = {-1, -1}; // Indicate no empty cell
//     return emptyPos;
// }

Coord getNextMinDomainCell(Sudoku sudoku) {
    int minlength = INF;
    Coord result = {-1, -1}; // Indicate no empty cell
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLS; j++) {
            if (sudoku.cells[i][j] == EMPTY) {
                Coord pos = {i, j};
                int availablesLength = getAvailableValues(pos, sudoku).size;
                if (availablesLength < minlength) {
                    minlength = availablesLength;
                    result = pos;
                }
            }
        }
    }
    return result;
}

int exploredCounter = 0;

int sudokuBackTracking(Sudoku *sudoku) {
    if (isFull(*sudoku)) {
        return 1;
    }
    Coord pos = getNextMinDomainCell(*sudoku);
    if (pos.x == -1 && pos.y == -1) {
        return 0;
    }
    listCoord availables = getAvailableValues(pos, *sudoku);
    if (availables.size == 0) {
        return 0;
    }

    int j;
    for (j = 0; j < availables.size; j++) {
        int value = indexOf(availables.data[j]);
        sudoku->cells[pos.x][pos.y] = value;
        exploredCounter++;
        if (sudokuBackTracking(sudoku)) {
            return 1;
        }
        sudoku->cells[pos.x][pos.y] = EMPTY;
    }

    return 0;
}

Sudoku solve(Sudoku sudoku) {
    initConstrains(&sudoku.cons);
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLS; j++) {
            listCoord history;
            initListCoord(&history);
            Coord pos = {i, j};
            spreadConstrainsFrom(pos, &sudoku.cons, &history);
        }
    }
    exploredCounter = 0;
    if (sudokuBackTracking(&sudoku)) {
        printf("SOLVED\n");
    } else {
        printf("CAN NOT SOLVE\n");
    }
    printf("Explored %d states\n", exploredCounter);
    return sudoku;
}

int input1[9][9] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9},
};

int main() {
    Sudoku sudoku;
    initSudokuWithValues(&sudoku, input1);
    printSudoku(sudoku);
    Sudoku result = solve(sudoku);
    printSudoku(result);
    return 0;
}
