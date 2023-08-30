#include <stdio.h>
#include <stdlib.h>

#define tankcapacity_Monk 3
#define tankcapacity_Devil 3 
#define goal 0
#define MaxLength 200

const char* action[] = {
	"First State","Move One Monk","Move One Devil", "Move Two Monk",
	"Move Two Devil", "Move One Monk And One Devil"};

typedef struct{
	int monks;
	int devils;
	char boat_location;
} State;

void makenullState(State *state){
	state->monks = 3;
	state->devils = 3;
	state->boat_location = 'A';
}

int max(int a, int b){
	return a > b ? a : b;
}

int min(int a, int b){
	return a > b ? b : a;
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
	int no_function;
}Node;

typedef struct{
	Node* Elements[MaxLength];
	int front, rear;
}Queue;

void makenullQueue(Queue *queue){
	queue->front = -1;
	queue->rear = -1;
}

int emptyQueue(Queue queue){
	return queue.front == -1;
}

int fullQueue(Queue queue){
	return ((queue.rear - queue.front + 1) % MaxLength) == 0;
}

void pop_queue(Queue *queue){
	if(!emptyQueue(*queue)){
		if(queue->front == queue->rear)
			makenullQueue(queue);
		else queue->front = (queue->front+1)%MaxLength;
	}
	else printf("Error pop queue");
}

void push_queue(Node* x, Queue *queue){
	if(!fullQueue(*queue)){
		if(emptyQueue(*queue))
			queue->front = 0;
		queue->rear = (queue->rear+1)%MaxLength;
		queue->Elements[queue->rear] = x;
	}
	else printf("Error push queue");
}

Node* top_queue(Queue queue){
	if(emptyQueue(queue))
		printf("Queue is empty");
	else return queue.Elements[queue.front];
}

int compareStateS(State a, State b){
	return a.monks==b.monks && a.devils==b.devils && a.boat_location==b.boat_location;
}

int find_State(State state, Queue openQueue){
	while(!emptyQueue(openQueue)){
		if(compareStateS(top_queue(openQueue)->state, state))
			return 1;
		pop_queue(&openQueue);
	}
	return 0;
}

void printState(State state){
	printf("\n A: [Monks: %d -- Devil: %d ] --- B: [Monks: %d -- Devil: %d] Boat in bank : %c]", state.monks, state.devils, tankcapacity_Monk - state.monks, tankcapacity_Devil -state.devils, state.boat_location);
}

Node* BFS(State state){
	Queue Open_BFS, Close_BFS;
	
	makenullQueue(&Open_BFS);
	makenullQueue(&Close_BFS);
	
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	push_queue(root, &Open_BFS);
	
	while(!emptyQueue(Open_BFS)){
		Node* node = top_queue(Open_BFS);
		pop_queue(&Open_BFS);
		push_queue(node, &Close_BFS);
		
		if(checkGoal(node->state))
			return node;
			
		int option;
		for(option=1; option<=5; option++){
			State newstate;
			makenullState(&newstate);
			
			if(call_operator(node->state, &newstate, option)){
				if(find_State(newstate, Close_BFS))
					continue;
						
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_function = option;
				push_queue(newNode, &Open_BFS);
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	Queue queuePrint;
	makenullQueue(&queuePrint);
	
	while(node->Parent != NULL){
		push_queue(node, &queuePrint);
		node = node->Parent;
	}
	
	push_queue(node, &queuePrint);
	int i;
	for(i = queuePrint.rear; i >= queuePrint.front; i--){
		printf("\n Action %d: %s", queuePrint.rear-i, action[queuePrint.Elements[i]->no_function]);
		printState(queuePrint.Elements[i]->state);
	}
}

int main(){
	State cur_state = {3, 3, 'A'};
	Node* p = BFS(cur_state);
	print_WaysToGetGoal(p);
	return 0;
}
