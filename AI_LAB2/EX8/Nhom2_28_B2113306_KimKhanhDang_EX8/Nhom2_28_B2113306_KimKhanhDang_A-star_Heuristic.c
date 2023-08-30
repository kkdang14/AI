#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define ROWS 3
#define COLS 3
#define empty 0
#define max_operator 4
#define maxlength 500

const char* action[] = {
	"First state",
	"Move cell Empty to Up",
	"Move cell Empty to Down",
	"Move cell Empty to Left",
	"Move cell Empty to Right"
};

typedef struct{
	int puzzle[ROWS][COLS];
	int emptyRow;
	int emptyCol;
}State;

void print_State(State state){
	int row, col;
	printf("\n----------\n");
	for(row=0; row<ROWS; row++){
		for(col=0; col<COLS; col++)
			printf("|%d ",state.puzzle[row][col]);
		printf("|\n");
	}
	printf("----------\n");
}


int compareState(State state1, State state2){
	if(state1.emptyCol != state2.emptyCol || state1.emptyRow != state2.emptyRow)
		return 0;
	int row, col;
	for(row=0; row<ROWS; row++)
		for(col=0; col<COLS; col++)
			if(state1.puzzle[row][col] != state2.puzzle[row][col])
				return 0;
	return 1;		
}

int goalCheck(State state, State goal){
	return compareState(state, goal);
}

int upOperator(State state, State *result){
	*result = state;
	int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
	if(empRowCurrent > 0){
		result->emptyRow = empRowCurrent - 1;
		result->emptyCol = empColCurrent;
		result->puzzle[empRowCurrent][empColCurrent] = state.puzzle[empRowCurrent-1][empColCurrent];
		result->puzzle[empRowCurrent-1][empColCurrent] = empty;
		return 1;
	}
	return 0;
}

int downOperator(State state, State *result){
	*result = state;
	int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
	if(empRowCurrent < 2){
		result->emptyRow = empRowCurrent + 1;
		result->emptyCol = empColCurrent;
		result->puzzle[empRowCurrent][empColCurrent] = state.puzzle[empRowCurrent+1][empColCurrent];
		result->puzzle[empRowCurrent+1][empColCurrent] = empty;
		return 1;
	}
	return 0;
}

int leftOperator(State state, State *result){
	*result = state;
	int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
	if(empColCurrent > 0){
		result->emptyRow = empRowCurrent;
		result->emptyCol = empColCurrent - 1;
		result->puzzle[empRowCurrent][empColCurrent] = state.puzzle[empRowCurrent][empColCurrent-1];
		result->puzzle[empRowCurrent][empColCurrent-1] = empty;
		return 1;
	}
	return 0;
}

int rightOperator(State state, State *result){
	*result = state;
		int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
	if(empColCurrent < 2){
		result->emptyRow = empRowCurrent;
		result->emptyCol = empColCurrent + 1;
		result->puzzle[empRowCurrent][empColCurrent] = state.puzzle[empRowCurrent][empColCurrent+1];
		result->puzzle[empRowCurrent][empColCurrent+1] = empty;
		return 1;
	}
	return 0; 
}


int callOperators(State state, State *result, int opt){
	switch(opt){
		case 1: return upOperator(state, result);
		case 2: return rightOperator(state, result);
		case 3: return leftOperator(state, result);
		case 4: return downOperator(state, result);
		default: 
			printf("Cannot call operators");
			return 0;
	}
}

int heuristic1(State state, State goal){
	int row, col;
	int count = 0;
	for(row=0; row<ROWS; row++)
		for(col=0; col<COLS; col++){
			if(state.puzzle[row][col] != goal.puzzle[row][col])
				count++;
		}
	return count;
}

int heuristic2(State state, State goal){
	int cnt = 0;
	int row, col, row_g, col_g;
	for(row=0; row<ROWS; row++)
		for(col=0; col<COLS; col++)
			if(state.puzzle[row][col] != empty)
				for(row_g=0; row_g<ROWS; row_g++)
					for(col_g=0; col_g<COLS; col_g++)
						if(state.puzzle[row][col] == goal.puzzle[row_g][col_g]){
							cnt+=abs(row-row_g) + abs(col-col_g);
							row_g=ROWS;
							col_g=COLS;
						}
	return cnt;
}

typedef struct Node{
	State state;
	struct Node* parent;
	int no_func;
	int f, g, h; 
}Node;

typedef struct{
	Node* Elements[maxlength];
	int size;
}List;

void makeNullList(List *list){
	list->size = 0;
}

int emptyList(List list){
	return list.size == 0;
}

int fullList(List list){
	return list.size == maxlength;
}

Node* element_At(List list, int p){
	return list.Elements[p-1];
}

void push_List(Node *x, int position, List *list){
	if(!fullList(*list)){
		int q;
		for(q=list->size; q>=position; q--){
			list->Elements[q] = list->Elements[q-1];
		}
		list->Elements[position-1] = x;
		list->size++;
	}
}

void del_List(List *list, int position){
	if(emptyList(*list))
		printf("List is empty\n");
	else if(position < 1 || position > list->size)
		printf("Position is not possible to delete\n");
	else{
		int i;
		for(i=position-1; i<list->size; i++){
			list->Elements[i] = list->Elements[i+1]; 
		}	
		list->size--;
	}
}

Node* findState(State state, List list, int *p){
	int i;
	for(i=0; i<list.size; i++)
		if(compareState(element_At(list, i+1)->state, state)){
			*p = i+1;
			return element_At(list, i+1);
		}
	return NULL;	
}

void sort(List *list){
	int i, j;
	for(i=0; i<list->size-1; i++){
		for(j=i+1; j<list->size; j++)
			if(list->Elements[i]->f > list->Elements[j]->f){
				Node* node = list->Elements[i];
				list->Elements[i] = list->Elements[j];
				list->Elements[j] = node;
			}
	}
}


Node* Best_First_Search(State state, State goal){
	List OPEN_A_STAR;
	List CLOSE_A_STAR;
	makeNullList(&OPEN_A_STAR);
	makeNullList(&CLOSE_A_STAR);
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_func = 0;
	root->g = 0;
	root->h = heuristic2(root->state, goal);
	root->f = root->h + root->g;
	push_List(root, OPEN_A_STAR.size+1, &OPEN_A_STAR);
	while(!emptyList(OPEN_A_STAR)){
		Node* node = element_At(OPEN_A_STAR, 1);
		del_List(&OPEN_A_STAR, 1);
		push_List(node, CLOSE_A_STAR.size+1, &CLOSE_A_STAR);
		if(goalCheck(node->state, goal))
			return node;
		int opt;
		for(opt=1; opt<=max_operator; opt++){
			State newstate;
			newstate = node->state;
			if(callOperators(node->state, &newstate, opt)){
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->parent = node;
				newNode->no_func = opt;
				newNode->h = heuristic2(newstate, goal);
				newNode->g = newNode->g+1;
				newNode->f = newNode->h + newNode->g;
				int pos_Open, pos_Close;
				Node* nodeFoundOpen = findState(newstate, OPEN_A_STAR, &pos_Open);
				Node* nodeFoundClose = findState(newstate, CLOSE_A_STAR, &pos_Close);
				if (nodeFoundOpen == NULL && nodeFoundClose == NULL){
					push_List(newNode, OPEN_A_STAR.size+1, &OPEN_A_STAR);
				}else if(nodeFoundOpen != NULL && nodeFoundOpen->g > newNode->g){
					del_List(&OPEN_A_STAR, pos_Open);
					push_List(newNode, pos_Open, &OPEN_A_STAR);
				}else if(nodeFoundClose != NULL && nodeFoundClose->g > newNode->g){
					del_List(&CLOSE_A_STAR, pos_Close);
					push_List(newNode, OPEN_A_STAR.size+1, &OPEN_A_STAR);
				}
				sort(&OPEN_A_STAR);
			}
		}
	}
	free(root);
	return NULL;
}

void print_Way(Node* node){
	List listPrint;
	makeNullList(&listPrint);
	while(node->parent != NULL){
		push_List(node, listPrint.size+1, &listPrint);
		node = node->parent;
	}
	push_List(node, listPrint.size+1, &listPrint);
	int no_act = 0, i;
	for(i=listPrint.size; i>0; i--){
		printf("\nAction %d: %s", no_act, action[element_At(listPrint, i)->no_func]);
		print_State(element_At(listPrint, i)->state);
		no_act++;
	}
}

int main(){
	State state, goal;
	state.emptyRow = 1;
	state.emptyCol = 1;
	state.puzzle[0][0] = 1;
	state.puzzle[0][1] = 2;
	state.puzzle[0][2] = 3;
	state.puzzle[1][0] = 8;
	state.puzzle[1][1] = 0;
	state.puzzle[1][2] = 4;
	state.puzzle[2][0] = 7;
	state.puzzle[2][1] = 6;
	state.puzzle[2][2] = 5;
	
	goal.emptyRow=1;
	goal.emptyCol=0;
	goal.puzzle[0][0] = 2;
	goal.puzzle[0][1] = 8;
	goal.puzzle[0][2] = 1;
	goal.puzzle[1][0] = 0;
	goal.puzzle[1][1] = 4;
	goal.puzzle[1][2] = 3;
	goal.puzzle[2][0] = 7;
	goal.puzzle[2][1] = 6;
	goal.puzzle[2][2] = 5;
		
	Node* p = Best_First_Search(state, goal);
	print_Way(p);
	
	return 0;
}


