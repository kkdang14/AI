#include <stdio.h>
#include <stdlib.h>
#define tankcapacity_X 9
#define tankcapacity_Y 4
#define empty 0
#define goal 6
#define MaxLength 100

typedef struct{
	int x;
	int y;
}State;

void makenullState(State *state){
	state->x = 0;
	state->y = 0;
}

void print_State(State state){
	printf("\n   X:%d --- Y:%d", state.x, state.y);
}

int checkGoal(State state){
	return (state.x == goal || state.y == goal);
}

int max(int a, int b){
	return a > b ? a:b;
}

int min(int a, int b){
	return a < b ? a:b;
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
		result->x = empty;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourWaterEmptyY(State cur_state, State *result){
	if(cur_state.y > 0){
		result->y = empty;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourWaterXY(State cur_state, State *result){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), empty);
		result->y = min(cur_state.x + cur_state.y, tankcapacity_Y);
		return 1;
	}
	return 0;
}

int pourWaterYX(State cur_state, State *result){
	if(cur_state.y > 0 && cur_state.x < tankcapacity_X){
		result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), empty);
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
		default: printf("Error calls operators");
			return 0;
	}
}

const char* action[] = {"First State", "pour water full X", "pour water full Y", 
						"pour water empty X", "pour water empty Y", "pour water X to Y", "pour water Y to X"};
						
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

int compareStateS(State openStack, State state){
	if(openStack.x == state.x && openStack.y == state.y)
		return 1;
	return 0;
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
	printf("\n   X:%d --- Y:%d", state.x, state.y);
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
	free(root);
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
	State cur_state = {0,0};
	Node* p = BFS(cur_state);
	print_WaysToGetGoal(p);
	return 0;
}
