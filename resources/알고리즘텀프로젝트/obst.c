#include <stdio.h>
#include <stdlib.h>
/*
*2014154031 이정찬
*2014154020 박현규
Optimal Binary Search Tree
Method: POA Not CRLS
POA(page 90) => bst function use
INPUT: n, p, q (n is node's count)
OUTPUT: cost table, weight table, root table
copyright@obst by Lee jeong chan , park hyeun gyu
Computer Engineering by Korea polytechnic University
*/


void init_zero_table(double **weight, double **cost, double **root, int size);//테이블0으로 초기화 함수
void input_p(double *p, int size); // p입력
void input_q(double *q, int size); // q입력
void display_pq(double *p, double *q, int size); // p와 q 입력된 것 을 보여줌
void display(double **weight, double **cost, double **root, int size); //테이블 출력 
void bst();//obst



int main()
{
	int size;//노드 개수 입력 받을 변수
	int i;
	//입력받을 p와 q 와 출력 table 선언
	double *p = NULL;
	double *q = NULL;
	double **weight = NULL;
	double **cost = NULL;
	double **root = NULL;

	//개수 입력 부분
	printf("개수를 입력하세요(size):");
	scanf_s("%d", &size);


	//각 포인터 동적할당 부분
	p = (double *)malloc(sizeof(double)*(size + 1));
	q = (double *)malloc(sizeof(double)*(size + 1));

	//2차원 배열 테이블을 위한 2차원 배열 동적할당
	weight = (double **)malloc((size + 1) * sizeof(double*));
	for (i = 0; i <= size; i++)
	{
		weight[i] = (double *)malloc((size + 1) * sizeof(double));
	}
	cost = (double **)malloc((size + 1) * sizeof(double*));
	for (i = 0; i <= size; i++)
	{
		cost[i] = (double *)malloc((size + 1) * sizeof(double));
	}
	root = (double **)malloc((size + 1) * sizeof(double*));
	for (i = 0; i <= size; i++)
	{
		root[i] = (double *)malloc((size + 1) * sizeof(double));
	}
	//0으로 테이블 초기화
	init_zero_table(weight, cost, root, size);

	//p 입력
	input_p(p, size);
	//q 입력
	input_q(q, size);
	//p q 테이블 확인
	display_pq(p, q, size);
	printf("-----------------------------------------------------------------\n");
	//최적 이진 탐색 트리를 위한 함수 호출
	bst(p, q, size, weight, cost, root);
	display(weight, cost, root, size);
	free(p);
	free(q);
	free(weight);
	free(cost);
	free(root);
	return 0;

}

//테이블 0으로 초기화
void init_zero_table(double **weight, double **cost, double **root, int size)
{
	int i;
	int j;
	//인자로 받은 크기만큼 돌려서 
	//테이블 0으로 우선적으로 초기화 시킨다.
	for (i = 0; i <= size; i++)
	{
		for (j = 0; j <= size; j++)
		{
			weight[i][j] = 0;
			cost[i][j] = 0;
			root[i][j] = 0;
		}
	}
}

//p를 입력
void input_p(double *p, int size)
{
	//크기를 인자로 받고 p를 인자로 받아서
	//동적 할당 된 p 배열에 값을 넣는다.
	int i;
	for (i = 1; i <= size; i++)
	{
		printf("p[%d]입력: ", i);
		scanf_s("%lf", &p[i]);
	}
	printf("\n");
}


//q를 입력 
void input_q(double *q, int size)
{
	//크기를 인자로 받고 q를 인자로 받아서
	//동적 할당 된 q 배열에 값을 넣는다.
	//poa 방식은 q가 0부터 시작한다.
	int i;
	for (i = 0; i <= size; i++)
	{
		printf("q[%d]입력: ", i);
		scanf_s("%lf", &q[i]);
	}
	printf("\n");
}


/*
*bst 입력받은 p와 q n을 통해 테이블 적용
*poa 90page에 나온 수도코드 작성
*/
void bst(double *p, double *q, int size, double **weight, double **cost, double **root)
{
	int i, j, l, k;
	for (i = 0;i <= size;i++) {
		weight[i][i] = q[i];
		cost[i][i] = 0;
	}
	for (l = 1; l <= size; l++)
	{
		for (i = 0; i <= size - l; i++)
		{
			j = i + l;
			weight[i][j] = weight[i][j - 1] + p[j] + q[j];

			for (k = i + 1; k <= j; k++)
			{
				if (k == i + 1)
				{
					cost[i][j] = cost[i][k - 1] + cost[k][j] + weight[i][j];
					root[i][j] = k;
				}
				else {
					//최소값을 찾아서 넣는다. 이때 k값이 root 테이블에 써짐.
					if (cost[i][j] >= (cost[i][k - 1] + cost[k][j] + weight[i][j]))
					{
						cost[i][j] = cost[i][k - 1] + cost[k][j] + weight[i][j];
						root[i][j] = k;
					}
				}
			}
		}
	}
}


//입력 받은 p와 q를 출력
void display_pq(double *p, double *q, int size)
{

	int i;
	printf("p Table \n");
	for (i = 1; i <= size; i++) {
		printf("|%.2lf ", p[i]);  // 배열 p 출력
	}
	printf("|");
	fputs("\n", stdout);

	printf("\n");
	printf("q Table \n");

	for (i = 0; i <= size; i++) {
		printf("|%.2lf ", q[i]);  // 배열 q 출력
	}
	printf("|");
	fputs("\n", stdout);
	printf("\n");
}



//테이블 출력
void display(double **weight, double **cost, double **root, int size)
{
	int i, j;

	printf("------------------weight table---------------------------------------\n");

	for (i = 0; i <= size; i++)
	{
		printf("|");
		for (j = 0; j <= size; j++)
			printf(" %.2lf | ", weight[i][j]);
		printf("\n");
		printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	}
	printf("\n");

	printf("------------------cost table---------------------------------------\n");

	for (i = 0; i <= size; i++)
	{
		printf("|");
		for (j = 0; j <= size; j++)
			printf(" %.2lf | ", cost[i][j]);
		printf("\n");
		printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	}

	printf("\n");

	printf("------------------root table---------------------------------------\n");

	for (i = 0; i <= size; i++)
	{
		printf("|");
		for (j = 0; j <= size; j++)
			printf(" %.2lf | ", root[i][j]);
		printf("\n");
		printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	}

}