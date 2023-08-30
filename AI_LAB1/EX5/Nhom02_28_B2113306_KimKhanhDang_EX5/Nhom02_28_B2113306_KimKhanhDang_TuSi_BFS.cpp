#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>

#define tankcapacity_Monk 3
#define tankcapacity_Devil 3 
#define goal 0
#define MaxLength 200

using namespace std;

const char* action[] = {
	"First State","Move One Monk","Move One Devil", "Move Two Monk",
	"Move Two Devil", "Move One Monk And One Devil"};

typedef struct{
	int monks;
	int devils;
	char boat_location;
} State;

void makeNullState(State *state){
	state->monks = 3;
	state->devils = 3;
	state->boat_location = 'A';
}

int checkGoal(State state){
	return (state.monks == goal && state.devils == goal && state.boat_location == 'B');
}
 
int moveOneMonk(State cur_state, State *result){
	if(cur_state.boat_location == 'A'){
		if(cur_state.monks > 0 && cur_state.monks > cur_state.devils){
			if(tankcapacity_Monk - cur_state.monks >= tankcapacity_Devil - cur_state.devils){
				result->monks = cur_state.monks - 1;
				result->devils = cur_state.devils;
				result->boat_location = 'B';
				return 1;
			}	
		}
	}else{
		if(tankcapacity_Monk - cur_state.monks > 0 && tankcapacity_Monk - cur_state.monks > tankcapacity_Devil - cur_state.devils){
			if(cur_state.monks >= cur_state.devils){
				result->monks = cur_state.monks + 1;
				result->devils = cur_state.devils;
				result->boat_location = 'A';
				return 1;
			}
		}
	}
	
	return 0;
}

int moveOneDevil(State cur_state, State *result){
	if(cur_state.boat_location == 'A'){
		if(cur_state.devils > 0){
			if(tankcapacity_Devil - cur_state.devils < tankcapacity_Monk - cur_state.monks){
				result->monks = cur_state.monks;
				result->devils = cur_state.devils - 1;
				result->boat_location = 'B';
				return 1;
			}	
		}
	}else{
		if(tankcapacity_Devil - cur_state.devils > 0){
			if(cur_state.devils < cur_state.monks || cur_state.monks == 0){
				result->monks = cur_state.monks;
				result->devils = cur_state.devils + 1;
				result->boat_location = 'A';
				return 1;
			}
		}
	}
	
	return 0;
}

int moveTwoMonk(State cur_state, State *result){
	if(cur_state.boat_location == 'A'){
		if(cur_state.monks - 2 >= cur_state.devils || cur_state.monks - 2 == 0){
			if(tankcapacity_Monk - cur_state.monks + 2 >= cur_state.devils){
				result->monks = cur_state.monks - 2;
				result->devils = cur_state.devils;
				result->boat_location = 'B';
				return 1;
			}	
		}
	}else{
		if(tankcapacity_Devil - cur_state.devils > 0){
			if(cur_state.monks + 2 >= cur_state.devils){
				result->monks = cur_state.monks + 2;
				result->devils = cur_state.devils;
				result->boat_location = 'A';
				return 1;
			}
		}
	}
	
	return 0;
}

int moveTwoDevil(State cur_state, State *result){
	if(cur_state.boat_location == 'A'){
		if(cur_state.devils > 1){
			if(tankcapacity_Monk - cur_state.monks >= tankcapacity_Devil - cur_state.devils + 2 || tankcapacity_Monk - cur_state.monks == 0){
				result->monks = cur_state.monks;
				result->devils = cur_state.devils - 2;
				result->boat_location = 'B';
				return 1;
			}		
		}
	}else {
		if(tankcapacity_Devil - cur_state.devils >= 2){
			if(cur_state.monks >= cur_state.devils + 2 || cur_state.monks == 0){
				result->monks = cur_state.monks;
				result->devils = cur_state.monks + 2;
				result->boat_location = 'A';
				return 1;
			}
		}
	}
	
	return 0;
}

int moveMonkAndDevil(State cur_state, State *result){
	if(cur_state.boat_location == 'A'){
		if(cur_state.monks > 0 && cur_state.devils > 0){
			if(tankcapacity_Monk - cur_state.monks >= tankcapacity_Devil - cur_state.devils){
				result->monks = cur_state.monks - 1;
				result->devils = cur_state.devils - 1;
				result->boat_location= 'B';
				return 1;
			}
		}
	}else {
		if(tankcapacity_Monk - cur_state.monks > 0 && tankcapacity_Devil - cur_state.devils > 0){
			if(cur_state.monks >= cur_state.devils){
				result->monks = cur_state.monks + 1;
				result->devils = cur_state.devils + 1;
				result->boat_location = 'A';
				return 1;
			}			
		}
	}
	
	return 0;
}	

int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1: return moveOneMonk(cur_state, result);
		case 2: return moveOneDevil(cur_state, result);
		case 3: return moveTwoMonk(cur_state, result);
		case 4: return moveTwoDevil(cur_state, result);
		case 5: return moveMonkAndDevil(cur_state, result);
		default: printf("Error calls operators");
			return 0;
	}
}

typedef struct Node{
	State state;
	struct Node* Parent;
	int no_func;
}Node;

int compareStates(State a, State b){
	return a.monks==b.monks && a.devils==b.devils && a.boat_location==b.boat_location;
}

int find_State(State state, queue<Node*> openStack){
	if(!openStack.empty()){
		if(compareStates(openStack.front()->state, state))
			return 1;
		openStack.pop();
	}
	return 0;
}

void printState(State state){
	printf("\n A: [Monks: %d -- Devil: %d ] --- B: [Monks: %d -- Devil: %d] Boat in bank : %c]", state.monks, state.devils, tankcapacity_Monk - state.monks, tankcapacity_Devil -state.devils, state.boat_location);
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
        for (opt = 1; opt <= 5; opt++) {
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
	State cur_state = {3, 3, 'A'};
	Node* p = BFS(cur_state);
	print_WaysToGetGoal(p);
	return 0;
}
