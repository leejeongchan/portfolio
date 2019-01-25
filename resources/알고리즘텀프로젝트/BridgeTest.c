#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
/*
*2014154031 ������
*2014154020 ������
Bridge Testing 
Method: DFS
copyright@obst by Lee jeong chan , park hyeun gyu
Computer Engineering by Korea polytechnic University
*/

#define minimum(x, y) (((x) < (y)) ? (x) : (y)) // �ּҰ� ���ϱ� ���� ��ũ���Լ�.

int Size;//������ ����
int count;
int *bridge; //�긴���� ���� ���� ������ ���� 1���� �迭 
int *position; //�湮 ��ġ�� �˾ƺ��� �迭
int *shortest; //�湮��ġ�� ���Ͽ� ���� ��� �긴�� 
int **graph; //�׷��� 2���� �迭
void input_Vertex(); //���� �Է�
void input_Edge(); //���� �Է�
void print_Result(); //��� ���
void bridge_test(); //�긴�� ã�� �Լ�(dfs ȣ��)
void dfs(int, int);//���̿켱Ž��

void main() {

	input_Vertex(); //���� ���� �Է�
	input_Edge();// ������ ���� �Է��ϰ� �糡�� �̿� ���� �Է�

	bridge_test(); //�긴�� ã��(dfs ȣ��)

	print_Result(); //������

	/*�����Ҵ� ����*/
	free(position);
	free(shortest);
	free(bridge);
	free(graph);

}
//���� �Է�
void input_Edge() {
	int a, b;
	int i;
	int edge_length = 0;
	printf("������ ������ �Է��ϼ���: ");
	scanf_s("%d", &edge_length);

	printf("������ �Է��ϼ���(������ 0~%d)\n", Size - 1);
	//������ ���� ��ŭ ������ �� ������ �Է��� �׷����� ä��������.(�׷��� ����)
	for (i = 0; i < edge_length; i++)
	{
		printf("����%d�� �� ������ �Է��Ͻÿ�:", i + 1);
		scanf_s("%d %d", &a, &b);

		graph[a][b] = 1;
		graph[b][a] = 1;
	}
}
void input_Vertex() {
	int i;
	printf("������ ������ �Է��ϼ���: ");

	scanf_s("%d", &Size);
	//�����Ҵ� 
	position = (int *)calloc(Size, sizeof(int));
	shortest = (int *)calloc(Size, sizeof(int));
	bridge = (int *)calloc(Size, sizeof(int));
	//2���迭 �����Ҵ�
	graph = (int **)calloc(Size, sizeof(int *));

	for (i = 0; i < Size; i++) {
		graph[i] = (int *)calloc(Size, sizeof(int));
	}

}
//dfs�� �湮
void dfs(int u, int v) {
	int w;

	position[v] = count++;
	shortest[v] = position[v];

	for (w = 0; w < Size; w++) {
		if (graph[w][v] == 1) {
			if (position[w] == -1) {
				dfs(v, w); //���ȣ�� ��� Ž�� �ʺ�켱Ž��

				shortest[v] = minimum(shortest[v], shortest[w]); //���� v�� w�� �ּ�

				if (shortest[w] == position[w]) { //���� ���ٸ� �긴��
					bridge[v] = w; //<v,w>�� �긴����.
				}
			}
			else if (w != u) {
				shortest[v] = minimum(shortest[v], position[w]);
			}
		}
	}
}

void bridge_test() {
	int i;
	/*�ʱ� ī��Ʈ�� 0*/
	count = 0;
	/*shortest�� position bridge ���� 1���迭 �ʱ� -1�� */
	for (i = 0; i < Size; i++) {
		shortest[i] = -1;
		position[i] = -1;
		bridge[i] = -1;
	}
	/*������ ������ŭ ������ position�� -1�̸� ó�� �湮���� dfsȣ���Ѵ�.*/
	for (i = 0; i < Size; i++) {
		if (position[i] == -1) {
			dfs(i, i); //dfs ȣ��
		}
	}
}
//��� ��� �Լ�
void print_Result() {
	int i, menu;
	int a, b;
	while (1) {
		//����ȭ�� UI
		printf("\n--------------------------------------------------\n");
		printf("|������ �Ͻðڽ��ϱ�?                            |\n");
		printf("|�긮�� ��ü ���� (1)                            |\n");
		printf("|���� �긴�� �׽�Ʈ(2)                           |\n");
		printf("|����(3)                                         |\n");
		printf("--------------------------------------------------\n\n");

		scanf_s("%d", &menu);
		//�޴� �Է�
		switch (menu)
		{
		case 1: //�޴� 1�� ������ �� bridge ���
			for (i = 0; i < Size; i++) {
				if (bridge[i] != -1) {
					printf("< %d %d >\n", i, bridge[i]);
				}
			}
			if (bridge[i] == -1) { 
				printf("bridge�� �����ϴ�.\n");
				for (i = 0; i < Size; i++)
					printf("%d\n", bridge[i]);
			}
			break;
		case 2:
			printf("������ �� ������ �Է��ϰ� ���͸� ��������:> ");
			scanf_s("%d %d", &a, &b);

			if ((bridge[a] != -1 && bridge[a] == b)) {
				printf("< %d %d >�� Bridge �Դϴ�.\n", a, b);
			}
			else {
				printf("< %d %d >�� Bridge�� �ƴմϴ�.\n", a, b);
			}
			break;
		case 3:
			exit(0);
		}
	}

	
}