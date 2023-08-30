#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include <queue>

#define NUM_GLASSES 6

typedef struct {
    char orientations[NUM_GLASSES];
} State;

typedef struct Node {
    State state;
    struct Node *parent;
    int action;  // Index of the action that led to this state
} Node;

Node *createNode(State state, Node *parent, int action) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->state = state;
    node->parent = parent;
    node->action = action;
    return node;
}

bool isGoalState(State state) {
    for (int i = 0; i < NUM_GLASSES; ++i) {
        if (state.orientations[i] != '1') {
            return false;
        }
    }
    return true;
}

State generateSuccessor(State state, int index) {
    State newState = state;
    for (int i = index; i < index + 3; ++i) {
        newState.orientations[i] = (state.orientations[i] == '0') ? '1' : '0';
    }
    return newState;
}

bool stateExistsInPath(Node *node, State state) {
    while (node != NULL) {
        if (memcmp(&(node->state), &state, sizeof(State)) == 0) {
            return true;
        }
        node = node->parent;
    }
    return false;
}

void printSolutionPath(Node *solutionNode) {
    std::stack<Node *> stack;
    while (solutionNode != NULL) {
        stack.push(solutionNode);
        solutionNode = solutionNode->parent;
    }

    int step = 1;
    while (!stack.empty()) {
        Node *node = stack.top();
        stack.pop();

        printf("Action %d: ", step++);
        for (int i = 0; i < NUM_GLASSES; ++i) {
            printf("%c ", node->state.orientations[i]);
        }
        printf("\n");
    }
}


void dfs(State initialState) {
    Node *initialNode = createNode(initialState, NULL, -1);

    printf("DFS:\n");

    std::stack<Node *> stack;
    stack.push(initialNode);

    while (!stack.empty()) {
        Node *currentNode = stack.top();
        stack.pop();

        if (isGoalState(currentNode->state)) {
            printf("Goal state reached.\n");
            printSolutionPath(currentNode);
            break;
        }

        bool hasUnvisitedSuccessor = false;

        for (int i = 0; i <= NUM_GLASSES - 3; ++i) {
            State successorState = generateSuccessor(currentNode->state, i);

            if (!stateExistsInPath(currentNode, successorState)) {
                Node *successorNode = createNode(successorState, currentNode, i);
                stack.push(successorNode);
                hasUnvisitedSuccessor = true;
                break;  // Stop after visiting the first valid successor
            }
        }

        if (!hasUnvisitedSuccessor) {
            free(currentNode);
        }
    }
}


int main() {
    State initialState = {{'1', '0', '1', '0', '1', '0'}};
    dfs(initialState);

    return 0;
}

