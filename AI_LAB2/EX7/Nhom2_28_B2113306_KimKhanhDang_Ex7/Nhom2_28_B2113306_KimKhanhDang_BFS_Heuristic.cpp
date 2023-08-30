#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include<vector>
#define ROWS 3
#define COLS 3
#define empty_p 0
#define max_operator 4
#define maxlength 500

using namespace std;

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
		result->puzzle[empRowCurrent-1][empColCurrent] = empty_p;
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
		result->puzzle[empRowCurrent+1][empColCurrent] = empty_p;
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
		result->puzzle[empRowCurrent][empColCurrent-1] = empty_p;
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
		result->puzzle[empRowCurrent][empColCurrent+1] = empty_p;
		return 1;
	}
	return 0; 
}


int callOperators(State state, State *result, int opt){
	switch(opt){
		case 1: return upOperator(state, result);
		case 2: return downOperator(state, result);
		case 3: return leftOperator(state, result);
		case 4: return rightOperator(state, result);
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
			if(state.puzzle[row][col] != empty_p)
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
	int heuristic;
}Node;

//Node* findState(State state, vector<Node*>& v, vector<Node*>::iterator& p){
//	vector<Node*>::iterator it = v.begin();
//	if(v.size() == 0) return NULL;
//	while(it != v.end()){
//		if(compareState((*it)->state, state)){
//			p = it;
//			return *it;
//		}
//		it = v.erase(it);
//	}
//	return NULL;	
//}

Node* findState(State state, vector<Node*>& v, vector<Node*>::iterator& p){
    for (p = v.begin(); p != v.end(); ++p) {
        if (compareState((*p)->state, state)) {
            return *p;
        }
    }
    return NULL;
}


bool compareHeuristic(Node* a, Node* b){
	return a->heuristic > b->heuristic;
}

Node* Best_First_Search(State state, State goal){
	vector<Node*> OPEN_BFS;
	vector<Node*> CLOSE_BFS;	
	Node* root = new Node();
	root->state = state;
	root->parent = NULL;
	root->no_func = 0;
	root->heuristic = heuristic2(root->state, goal);
	OPEN_BFS.push_back(root);
	while(!OPEN_BFS.empty()){
		Node* node = OPEN_BFS.back();
		OPEN_BFS.pop_back();
		if(goalCheck(node->state, goal))
			return node;
		CLOSE_BFS.push_back(node);
		int opt;
		for(opt=1; opt<=max_operator; opt++){
			State newstate;
			newstate = node->state;
			if(callOperators(node->state, &newstate, opt)){
				Node* newNode = new Node();
				newNode->state = newstate;
				newNode->parent = node;
				newNode->no_func = opt;
				newNode->heuristic = heuristic2(newstate, goal);
				vector<Node*>::iterator pos_Open, pos_Close;
				Node* nodeFoundOpen = findState(newstate, OPEN_BFS, pos_Open);
				Node* nodeFoundClose = findState(newstate, CLOSE_BFS, pos_Close);
				if (nodeFoundOpen == NULL && nodeFoundClose == NULL){
					OPEN_BFS.push_back(newNode);
				}else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
					OPEN_BFS.erase(pos_Open);
					OPEN_BFS.push_back(newNode);
				}else if(nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic){
					CLOSE_BFS.erase(pos_Close);
					OPEN_BFS.push_back(newNode);
				}
			}
		}
		sort(OPEN_BFS.begin(), OPEN_BFS.end(), compareHeuristic);
	}
	return NULL;
}

void print_Way(Node* node){
	vector<Node*> vectorPrint;
//	listPrint.clear();
	while(node->parent != NULL){
		vectorPrint.push_back(node);
		node = node->parent;
	}
	vectorPrint.push_back(node);
	int no_act = 0;
	for(int i = vectorPrint.size() - 1; i >= 0; i--){
		printf("\nAction %d: %s", no_act, action[vectorPrint.at(i)->no_func]);
		print_State(vectorPrint.at(i)->state);
		no_act++;
	}
}

int main(){
	State state, goal;
	state.emptyRow = 1;
	state.emptyCol = 1;
	state.puzzle[0][0] = 3;
	state.puzzle[0][1] = 4;
	state.puzzle[0][2] = 5;
	state.puzzle[1][0] = 1;
	state.puzzle[1][1] = 0;
	state.puzzle[1][2] = 2;
	state.puzzle[2][0] = 6;
	state.puzzle[2][1] = 7;
	state.puzzle[2][2] = 8;
	
	goal.emptyRow=0;
	goal.emptyCol=0;
	goal.puzzle[0][0] = 0;
	goal.puzzle[0][1] = 1;
	goal.puzzle[0][2] = 2;
	goal.puzzle[1][0] = 3;
	goal.puzzle[1][1] = 4;
	goal.puzzle[1][2] = 5;
	goal.puzzle[2][0] = 6;
	goal.puzzle[2][1] = 7;
	goal.puzzle[2][2] = 8;
	
	Node* p = Best_First_Search(state, goal);
    if (p != NULL) {
        print_Way(p);

        while (p != NULL) {
            Node* temp = p;
            p = p->parent;
            delete temp;
        }
    } else {
        printf("No solution found.\n");
    }
	
	return 0;
}


