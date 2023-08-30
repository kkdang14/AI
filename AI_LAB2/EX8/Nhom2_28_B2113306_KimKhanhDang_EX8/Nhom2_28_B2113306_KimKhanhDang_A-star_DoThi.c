#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTICES 5
#define maxlength 100

typedef struct{
	int neighbor[MAX_VERTICES];
	int h;
}Vertices;

typedef struct{
	int num_vertices;
	Vertices v[MAX_VERTICES];
}Graph;

void initGraph(Graph *G, int n){
	G->num_vertices = n;
	int i,j;
	for(i=0; i<n; i++)
		for(j=0; j<n; j++){
			G->v[i].neighbor[j] = 0;
			G->v[i].h = 0;
		}
}

const char name[] = {'A', 'B', 'C', 'D', 'G'};

typedef struct{
	int vertex;
}State;

void printState(State state){
	printf("%c", name[state.vertex]);
}

int compareStates(State a, State b){
	return a.vertex == b.vertex;
}

typedef struct Node{
	State state;
	struct Node* parent;
	int f,g,h;
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
	for(i=1; i < list.size; i++)
		if(compareStates(element_At(list, i)->state, state)){
			*p = i;
			return element_At(list, i);
		}
	return NULL;
}

int goalCheck(State state, State goal){
	return compareStates(state, goal);
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

Node* A_Star(Graph G, State state, State goal){
	List OPEN_A_STAR;
	List CLOSE_A_STAR;
	makeNullList(&OPEN_A_STAR);
	makeNullList(&CLOSE_A_STAR);
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->h = G.v[state.vertex].h;
	root->g = 0;
	root->f = root->h + root->g;
	push_List(root, OPEN_A_STAR.size+1, &OPEN_A_STAR);
	while(!emptyList(OPEN_A_STAR)){
		Node* node = element_At(OPEN_A_STAR, 1);
		del_List(&OPEN_A_STAR, 1);
		push_List(node, CLOSE_A_STAR.size+1, &CLOSE_A_STAR);
		if(goalCheck(node->state, goal)){
			return node;
		}
		int opt;
		for(opt=0; opt<G.num_vertices; opt++){
			State newState;
			if(G.v[node->state.vertex].neighbor[opt] > 0){
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state.vertex = opt;
				newNode->parent = node;
				newNode->g = node->g + G.v[node->state.vertex].neighbor[opt];
				newNode->h = G.v[opt].h;
				newNode->f = newNode->h + newNode->g;
				int pos_Open, pos_Close;
				Node* nodeFoundOpen = findState(newNode->state, OPEN_A_STAR, &pos_Open);
				Node* nodeFoundClose = findState(newNode->state, CLOSE_A_STAR, &pos_Close);
				if(nodeFoundOpen == NULL && nodeFoundClose == NULL){
					push_List(newNode, OPEN_A_STAR.size+1, &OPEN_A_STAR);
				}
				else if(nodeFoundOpen != NULL && nodeFoundOpen->g > newNode->g){
					del_List(&OPEN_A_STAR, pos_Open);
					push_List(newNode, pos_Open, &OPEN_A_STAR);
				}
				else if(nodeFoundClose != NULL && nodeFoundClose->g > newNode->g){
					del_List(&CLOSE_A_STAR, pos_Close);
					push_List(newNode, OPEN_A_STAR.size+1, &CLOSE_A_STAR);
				}
			}
		}
		sort(&OPEN_A_STAR);
	}
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
		printState(element_At(listPrint, i)->state);
		if(i != 1){
			printf("->");
		}
		no_act++;
	}
}

int main(){
	int i,j;
	Graph graph;
	freopen("ASTAR.txt", "r", stdin);
	initGraph(&graph, MAX_VERTICES);
	int x;
	for(i=0; i<MAX_VERTICES; i++){
		scanf("%d", &x);
		graph.v[i].h = x;
		for(j=0; j<MAX_VERTICES; j++){
			scanf("%d", &x);
			graph.v[i].neighbor[j] = x;	
		}
	}
	State A, G;
	A.vertex = 0;
	G.vertex = 4;
	Node* result = A_Star(graph, A, G);
	if (result != NULL)
		print_Way(result);
	
	return 0;
}


