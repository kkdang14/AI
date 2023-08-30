#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>

#define tankcapacity_X 9
#define tankcapacity_Y 4
#define empty_p 0
#define goal 6
#define Maxlength 100

using namespace std;

typedef struct{
	int x;
	int y;
}State;

void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
}

void printState(State state){
	printf("\n   X: %d --- Y: %d", state.x, state.y);
}

int checkGoal(State state){
	return state.x == goal || state.y == goal;
}

int pourWaterFullX(State cur_state, State *result){
	if(cur_state.x < tankcapacity_X){
		result->x = tankcapacity_X;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourWaterFullY(State cur_state, State *result){
	if(cur_state.y < tankcapacity_Y){
		result->y = tankcapacity_Y;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourWaterEmptyX(State cur_state, State *result){
	if(cur_state.x > 0){
		result->x = empty_p;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourWaterEmptyY(State cur_state, State *result){
	if(cur_state.y > 0){
		result->y = empty_p;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourWaterXY(State cur_state, State *result){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), empty_p);
		result->y = min(cur_state.x + cur_state.y, tankcapacity_Y);
		return 1;
	}
	return 0;
}

int pourWaterYX(State cur_state, State *result){
	if(cur_state.y > 0 && cur_state.x < tankcapacity_X){
		result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), empty_p);
		result->x = min(cur_state.y + cur_state.x, tankcapacity_X);
		return 1;
	}
	return 0;
}


int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1: return pourWaterFullX(cur_state, result);
		case 2: return pourWaterFullY(cur_state, result);
		case 3: return pourWaterEmptyX(cur_state, result);
		case 4: return pourWaterEmptyY(cur_state, result);
		case 5: return pourWaterXY(cur_state, result);
		case 6: return pourWaterYX(cur_state, result);
		default: printf("Erorr!");
			return 0;		
		
	}
}

const char* action[] = {"First State", "pour full water X", "pour full water Y", "pour water empty X", 
						"pour water empty Y", "pour water X  to Y", "pour water Y to X"};

typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;

int compareStates(State openStack, State state){
	return openStack.x == state.x && openStack.y == state.y ? 1 : 0;
}

int find_State(State state, queue<Node*> openQueue){
	while(!openQueue.empty()){
		if(compareStates(openQueue.front()->state, state))
			return 1;
		openQueue.pop();
	}
	return 0;
}

Node* BFS(State state){
	queue<Node*> Open_BFS, Close_BFS;
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	Open_BFS.push(root);
	
	while(!Open_BFS.empty()){
		Node* node = Open_BFS.front();
		Open_BFS.pop();
		Close_BFS.push(node);
		
		if(checkGoal(node->state))
			return node;
			
		int option;
		for(option=1; option<=6; option++){
			State newstate;
			makeNullState(&newstate);
			
			if(call_operator(node->state, &newstate, option)){
				if(find_State(newstate, Close_BFS) || find_State(newstate, Open_BFS))
					continue;
						
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_function = option;
				Open_BFS.push(newNode);
			}
		}
	}
	free(root);
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	stack<Node*> stackPrint;
	while(node->Parent != NULL){
		stackPrint.push(node);
		node = node->Parent;
	}
	stackPrint.push(node);
	int no_action = 0;
	while(!stackPrint.empty()){
		printf("\n Action %d: %s", no_action, action[stackPrint.top()->no_function]);
		printState(stackPrint.top()->state);
		stackPrint.pop();
		no_action++;
	}
}

int main(){
	State cur_state = {0,0};
	Node* p = BFS(cur_state);
	print_WaysToGetGoal(p);
	return 0;
}
