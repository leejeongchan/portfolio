#include <stdio.h>
#include <stdlib.h>
/*
*2014154031 ������
*2014154020 ������
Optimal Binary Search Tree
Method: POA Not CRLS
POA(page 90) => bst function use
INPUT: n, p, q (n is node's count)
OUTPUT: cost table, weight table, root table
copyright@obst by Lee jeong chan , park hyeun gyu
Computer Engineering by Korea polytechnic University
*/


void init_zero_table(double **weight, double **cost, double **root, int size);//���̺�0���� �ʱ�ȭ �Լ�
void input_p(double *p, int size); // p�Է�
void input_q(double *q, int size); // q�Է�
void display_pq(double *p, double *q, int size); // p�� q �Էµ� �� �� ������
void display(double **weight, double **cost, double **root, int size); //���̺� ��� 
void bst();//obst



int main()
{
	int size;//��� ���� �Է� ���� ����
	int i;
	//�Է¹��� p�� q �� ��� table ����
	double *p = NULL;
	double *q = NULL;
	double **weight = NULL;
	double **cost = NULL;
	double **root = NULL;

	//���� �Է� �κ�
	printf("������ �Է��ϼ���(size):");
	scanf_s("%d", &size);


	//�� ������ �����Ҵ� �κ�
	p = (double *)malloc(sizeof(double)*(size + 1));
	q = (double *)malloc(sizeof(double)*(size + 1));

	//2���� �迭 ���̺��� ���� 2���� �迭 �����Ҵ�
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
	//0���� ���̺� �ʱ�ȭ
	init_zero_table(weight, cost, root, size);

	//p �Է�
	input_p(p, size);
	//q �Է�
	input_q(q, size);
	//p q ���̺� Ȯ��
	display_pq(p, q, size);
	printf("-----------------------------------------------------------------\n");
	//���� ���� Ž�� Ʈ���� ���� �Լ� ȣ��
	bst(p, q, size, weight, cost, root);
	display(weight, cost, root, size);
	free(p);
	free(q);
	free(weight);
	free(cost);
	free(root);
	return 0;

}

//���̺� 0���� �ʱ�ȭ
void init_zero_table(double **weight, double **cost, double **root, int size)
{
	int i;
	int j;
	//���ڷ� ���� ũ�⸸ŭ ������ 
	//���̺� 0���� �켱������ �ʱ�ȭ ��Ų��.
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

//p�� �Է�
void input_p(double *p, int size)
{
	//ũ�⸦ ���ڷ� �ް� p�� ���ڷ� �޾Ƽ�
	//���� �Ҵ� �� p �迭�� ���� �ִ´�.
	int i;
	for (i = 1; i <= size; i++)
	{
		printf("p[%d]�Է�: ", i);
		scanf_s("%lf", &p[i]);
	}
	printf("\n");
}


//q�� �Է� 
void input_q(double *q, int size)
{
	//ũ�⸦ ���ڷ� �ް� q�� ���ڷ� �޾Ƽ�
	//���� �Ҵ� �� q �迭�� ���� �ִ´�.
	//poa ����� q�� 0���� �����Ѵ�.
	int i;
	for (i = 0; i <= size; i++)
	{
		printf("q[%d]�Է�: ", i);
		scanf_s("%lf", &q[i]);
	}
	printf("\n");
}


/*
*bst �Է¹��� p�� q n�� ���� ���̺� ����
*poa 90page�� ���� �����ڵ� �ۼ�
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
					//�ּҰ��� ã�Ƽ� �ִ´�. �̶� k���� root ���̺� ����.
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


//�Է� ���� p�� q�� ���
void display_pq(double *p, double *q, int size)
{

	int i;
	printf("p Table \n");
	for (i = 1; i <= size; i++) {
		printf("|%.2lf ", p[i]);  // �迭 p ���
	}
	printf("|");
	fputs("\n", stdout);

	printf("\n");
	printf("q Table \n");

	for (i = 0; i <= size; i++) {
		printf("|%.2lf ", q[i]);  // �迭 q ���
	}
	printf("|");
	fputs("\n", stdout);
	printf("\n");
}



//���̺� ���
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
		printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	}
	printf("\n");

	printf("------------------cost table---------------------------------------\n");

	for (i = 0; i <= size; i++)
	{
		printf("|");
		for (j = 0; j <= size; j++)
			printf(" %.2lf | ", cost[i][j]);
		printf("\n");
		printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	}

	printf("\n");

	printf("------------------root table---------------------------------------\n");

	for (i = 0; i <= size; i++)
	{
		printf("|");
		for (j = 0; j <= size; j++)
			printf(" %.2lf | ", root[i][j]);
		printf("\n");
		printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	}

}