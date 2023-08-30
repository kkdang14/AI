#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>

#define default_X 10
#define default_Y 5
#define default_Z 6
#define empty_tank 0
//#define goal 8
#define Maxlength 100

using namespace std;

typedef struct{
	int x;
	int y;
	int z;
}State;

void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
	state->z = 0;
}

void printState(State state){
	printf("\n	   X: %d --- Y: %d --- Z: %d", state.x, state.y, state.z);
}

int checkGoal(State state){
	return state.x == 8;
}

int pourMilkXtoY(State cur_state, State *result){
	if(cur_state.x > 0  && 0<= cur_state.y < default_Y){
		result->x = max(cur_state.x - (default_Y - cur_state.y), empty_tank);
		result->y = min(cur_state.x + cur_state.y, default_Y);
		result->z = cur_state.z;
		return 1;
	}
	return 0;
}

int pourMilkXtoZ(State cur_state, State *result){
	if(cur_state.x > 0  && 0<= cur_state.z < default_Z){
		result->x = max(cur_state.x - (default_Z - cur_state.z), empty_tank);
		result->z = min(cur_state.x + cur_state.z, default_Z);
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourMilkYtoZ(State cur_state, State *result){
	if(cur_state.y > 0  && 0<= cur_state.z < default_Z){
		result->y = max(cur_state.y - (default_Z - cur_state.z), empty_tank);
		result->z = min(cur_state.y + cur_state.z, default_Z);
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourMilkZtoY(State cur_state, State *result){
	if(cur_state.z > 0  && 0<= cur_state.y < default_Y){
		result->z = max(cur_state.z - (default_Y - cur_state.y), empty_tank);
		result->y = min(cur_state.z + cur_state.y, default_Y);
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourMilkYtoX(State cur_state, State *result){
	if(cur_state.y > 0  && 0<= cur_state.x < default_X){
		result->y = max(cur_state.y - (default_X - cur_state.x), empty_tank);
		result->x = min(cur_state.y + cur_state.x, default_X);
		result->z = cur_state.z;
		return 1;
	}
	return 0;
}

int pourMilkZtoX(State cur_state, State *result){
	if(cur_state.z > 0  && 0<= cur_state.x < default_X){
		result->z = max(cur_state.z - (default_X - cur_state.x), empty_tank);
		result->x = min(cur_state.z + cur_state.x, default_X);
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

const char* action[] = {
	"First State",
	"Pour Milk X to Y",
	"Pour Milk X to Z",
	"Pour Milk Y to Z",
	"Pour Milk Z to Y",
	"Pour Milk Y to X",
	"Pour Milk Z to X"
};


int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1: return pourMilkXtoY(cur_state, result);
		case 2: return pourMilkXtoZ(cur_state, result);
		case 3: return pourMilkYtoZ(cur_state, result);
		case 4: return pourMilkZtoY(cur_state, result);
		case 5: return pourMilkYtoX(cur_state, result);
		case 6: return pourMilkZtoX(cur_state, result);
		default:
			printf("Error when call operator!!!");
			return 0;
	}
}

int compareStates(State openStack, State state){
	return openStack.x == state.x && openStack.y == state.y && openStack.z == state.z ? 1 : 0;
}

typedef struct Node{
	State state;
	Node* Parent;
	int no_func;
}Node;

int find_State(State state, queue<Node*> openStack){
	if(!openStack.empty()){
		if(compareStates(openStack.front()->state, state))
			return 1;
		openStack.pop();
	}
	return 0;
}

Node* BFS(State state) {
    queue<Node*> BFS_Open; // S? d?ng queue cho BFS
    queue<Node*> BFS_Close; // S? d?ng queue cho BFS
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_func = 0;
    BFS_Open.push(root);

    while (!BFS_Open.empty()) {
        Node* node = BFS_Open.front();
        BFS_Open.pop();
        BFS_Close.push(node);

        if (checkGoal(node->state)) {
            return node;
        }
		int opt;
        for (opt = 1; opt <= 6; opt++) {
            State newstate;
            makeNullState(&newstate);

            if (call_operator(node->state, &newstate, opt))
                if(find_State(newstate, BFS_Close) || find_State(newstate, BFS_Open))
                	continue;
                	
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->Parent = node;
                newNode->no_func = opt;
                BFS_Open.push(newNode);
        }
    }

    free(root);
    return NULL;
}


void print_WaysToGetGoal(Node* node){
	stack<Node*> printStack;
	while(node->Parent != NULL){
		printStack.push(node);
		node = node->Parent;
	}
	printStack.push(node);
	int no_act = 0;
	while(!printStack.empty()){
		printf("\nAction: %d. %s", no_act, action[printStack.top()->no_func]);
		printState(printStack.top()->state);
		printStack.pop();
		no_act++;
	}
} 

int main(){
	State cur_state = {10, 0, 0};
	Node* p = BFS(cur_state);
	print_WaysToGetGoal(p);
	
	return 0;
}



