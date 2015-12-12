#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define THREAD_NUM	27

/* structure for passing data to threads */
typedef struct {
	int row;
	int column;
	int block;
	int **panel;
	
} parameters;

void *validElements(void *param) {

	parameters* params = (parameters*) param;
	//check crectness of each row
	if (params->row != -1) {
		int *ret =(int *)malloc(sizeof(int));
		*ret = 1;
		//initialize a 9-element array
		int *hashSet = (int *) malloc(9 * sizeof(int));
		//insure all of them are 0s

		int i;
		//check for all elements' space
		for (i = 0; i < 9; i++) {
			int val = *(*(params->panel + params->row) + i) - 1;
			if (!*(hashSet + val)) {
				*(hashSet + val) = 1;
			} else {
				//printf("row #%d is invalid because %d has already exists\n",params->row,val);
				*ret = 0;
				
			}
		}
		pthread_exit(ret);
	}
	//check crectness of each column
	if (params->column != -1) {
		//initialize a 9-element array
		int *ret =(int *)malloc(sizeof(int));
		*ret = 1;
		int *hashSet = (int *) malloc(9 * sizeof(int));
		//insure all of them are 0s

		int i;
		//check for all elements' space
		for (i = 0; i < 9; i++) {
			int val = *(*(params->panel + i) + params->column) - 1;
			if (!*(hashSet + val)) {
				*(hashSet + val) = 1;
			} else {
				//printf("column #%d is invalid because %d has already exists\n",params->column,val);
				*ret = 0;
				
			}
		}
		pthread_exit(ret);
	}
	//check crectness of each block
	if (params->block != -1) {
		//initialize a 9-element array
		int *ret =(int *)malloc(sizeof(int));
		*ret = 1;
		int *hashSet = (int *) malloc(9 * sizeof(int));
		int i = 0;
		int j = 0;
		//check for all elements' space
		for (; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				int val = *(*(params->panel + (params->block / 3) * 3 + i)
						+ (params->block % 3) * 3 + j) - 1;
				if (!*(hashSet + val)) {
					*(hashSet + val) = 1;
				} else {
					//printf("block #%d is invalid because %d has already exists\n",params->block,val);
					*ret = 0;
					
				}
			}
			pthread_exit(ret);

		}
	}
	//if(params->row!=-1){
	//	printf("The result for row : %d is : %s with rv : %d\n",params->row , ret? "valid":"invalid", ret);
	//}
	//printf("The result for row : %d column : %d block: %d is : %s with rv : %d\n",params->row,params->column,params->block , ret? "valid":"invalid", ret);
	//pthread_exit(&ret);
}

int main(int argc, char *argv[]) {
	printf("CS149 Sudoku from Jingyuan Chen\n");
	pthread_t thread[THREAD_NUM];
	pthread_attr_t attr;
	void *status;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int sudoku[9][9] = { { 6, 5, 3, 1, 2, 8, 7, 9, 4 }, { 1, 7, 4, 3, 5, 9, 6,
			8, 2 }, { 9, 2, 8, 4, 6, 7, 5, 3, 1 },
			{ 2, 8, 6, 5, 1, 4, 3, 7, 9 }, { 3, 9, 1, 7, 8, 2, 4, 5, 6 }, { 5,
					4, 7, 6, 9, 3, 2, 1, 8 }, { 8, 6, 5, 2, 3, 1, 9, 4, 7 }, {
					4, 1, 2, 9, 7, 5, 8, 6, 3 }, { 7, 3, 9, 8, 4, 6, 1, 2, 5 } };
	int i = 0;
	pthread_t tid;
	pthread_t group[27];
	int **panelVal = (int**) malloc(9 * sizeof(int *));

	for (; i < 9; i++) {
		*(panelVal + i) = (int *) malloc(9 * sizeof(int));
		int j = 0;
		for (; j < 9; j++) {
			*(*(panelVal + i) + j) = sudoku[i][j];
			printf(" %d ", *(*(panelVal + i) + j));
		}
		printf("\n");
	}

	//thread pool to be joined

	pthread_t *pool = (pthread_t *) malloc(27 * sizeof(pthread_t));

	//create 9 threads to check if we have valid rows
	for (i = 0; i < 9; i++) {

		parameters *param = (parameters *) malloc(sizeof(parameters));
		param->row = i;
		param->column = -1;
		param->block = -1;
		param->panel = panelVal;

		pthread_create(&tid, &attr, validElements, param);
		*(pool + i) = tid;
	}
	//create 9 threads to check if we have valid columns
	for (i = 0; i < 9; i++) {
		parameters *param = (parameters *) malloc(sizeof(parameters));
		param->row = -1;
		param->column = i;
		param->block = -1;
		param->panel = panelVal;

		pthread_create(&tid, &attr, validElements, param);
		*(pool + i + 9) = tid;
	}
	//create 9 threads to check if we have valid blocks
	for (i = 0; i < 9; i++) {
		parameters *param = (parameters *) malloc(sizeof(parameters));
		param->row = -1;
		param->column = -1;
		param->block = i;
		param->panel = panelVal;

		pthread_create(&tid, &attr, validElements, param);
		group[i + 18] = tid;
	}

	void *flag;
	int result = 1;
	for (i = 0; i < 27; i++) {
		pthread_join(*(pool + i), &flag);
		int *tmp = (int *) flag;
		result = result *(*tmp);
	}
	printf("The solution of sudoku is: %s\n",result==1?"valid":"not valid");
	return 0;
}
