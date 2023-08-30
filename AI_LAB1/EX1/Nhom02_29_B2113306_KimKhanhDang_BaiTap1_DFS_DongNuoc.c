#include <stdio.h>
#include <stdlib.h>
#define tankcapacity_X 9
#define tankcapacity_Y 4
#define empty 0
#define goal 6
#define Maxlength 100

typedef struct{
	int x;
	int y;
}State;

void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
}

void print_State(State state){
	printf("\n   X:%d --- Y:%d", state.x, state.y);
}

int goalcheck(State state){
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

int main(){
	State cur_state = {5, 4}, result;
	printf("Trang thai bat dau");
	print_State(cur_state);
	int opt;
	for(opt = 1; opt<=6; opt++){
		int thuchien = call_operator(cur_state, &result, opt);
		if(thuchien == 1){
			printf("\nHanh dong %s thanh cong", action[opt]);
			print_State(result);
		}
		else 
			printf("\nHanh dong %s KHONG thanh cong", action[opt]);
	}
	return 0;
}


