#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
/*
*2014154031 이정찬
*2014154020 박현규
Bridge Testing 
Method: DFS
copyright@obst by Lee jeong chan , park hyeun gyu
Computer Engineering by Korea polytechnic University
*/

#define minimum(x, y) (((x) < (y)) ? (x) : (y)) // 최소값 구하기 위한 메크로함수.

int Size;//정점의 개수
int count;
int *bridge; //브릿지의 점에 대한 정보를 담을 1차원 배열 
int *position; //방문 위치를 알아보는 배열
int *shortest; //방문위치와 비교하여 같을 경우 브릿지 
int **graph; //그래프 2차원 배열
void input_Vertex(); //정점 입력
void input_Edge(); //간선 입력
void print_Result(); //결과 출력
void bridge_test(); //브릿지 찾기 함수(dfs 호출)
void dfs(int, int);//깊이우선탐색

void main() {

	input_Vertex(); //정점 개수 입력
	input_Edge();// 간선의 개수 입력하고 양끝점 이용 간선 입력

	bridge_test(); //브릿지 찾기(dfs 호출)

	print_Result(); //결과출력

	/*동적할당 해제*/
	free(position);
	free(shortest);
	free(bridge);
	free(graph);

}
//간선 입력
void input_Edge() {
	int a, b;
	int i;
	int edge_length = 0;
	printf("간선의 갯수를 입력하세요: ");
	scanf_s("%d", &edge_length);

	printf("간선을 입력하세요(정점은 0~%d)\n", Size - 1);
	//간선의 개수 만큼 돌려서 양 끝점들 입력해 그래프를 채워나간다.(그래프 형성)
	for (i = 0; i < edge_length; i++)
	{
		printf("간선%d의 양 끝점을 입력하시오:", i + 1);
		scanf_s("%d %d", &a, &b);

		graph[a][b] = 1;
		graph[b][a] = 1;
	}
}
void input_Vertex() {
	int i;
	printf("정점의 개수를 입력하세요: ");

	scanf_s("%d", &Size);
	//동적할당 
	position = (int *)calloc(Size, sizeof(int));
	shortest = (int *)calloc(Size, sizeof(int));
	bridge = (int *)calloc(Size, sizeof(int));
	//2차배열 동적할당
	graph = (int **)calloc(Size, sizeof(int *));

	for (i = 0; i < Size; i++) {
		graph[i] = (int *)calloc(Size, sizeof(int));
	}

}
//dfs로 방문
void dfs(int u, int v) {
	int w;

	position[v] = count++;
	shortest[v] = position[v];

	for (w = 0; w < Size; w++) {
		if (graph[w][v] == 1) {
			if (position[w] == -1) {
				dfs(v, w); //재귀호출 계속 탐색 너비우선탐색

				shortest[v] = minimum(shortest[v], shortest[w]); //정점 v와 w의 최소

				if (shortest[w] == position[w]) { //만약 같다면 브릿지
					bridge[v] = w; //<v,w>가 브릿지다.
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
	/*초기 카운트는 0*/
	count = 0;
	/*shortest와 position bridge 동적 1차배열 초기 -1로 */
	for (i = 0; i < Size; i++) {
		shortest[i] = -1;
		position[i] = -1;
		bridge[i] = -1;
	}
	/*정점의 개수만큼 돌려서 position이 -1이면 처음 방문으로 dfs호출한다.*/
	for (i = 0; i < Size; i++) {
		if (position[i] == -1) {
			dfs(i, i); //dfs 호출
		}
	}
}
//결과 출력 함수
void print_Result() {
	int i, menu;
	int a, b;
	while (1) {
		//선택화면 UI
		printf("\n--------------------------------------------------\n");
		printf("|무엇을 하시겠습니까?                            |\n");
		printf("|브리지 전체 나열 (1)                            |\n");
		printf("|간선 브릿지 테스트(2)                           |\n");
		printf("|종료(3)                                         |\n");
		printf("--------------------------------------------------\n\n");

		scanf_s("%d", &menu);
		//메뉴 입력
		switch (menu)
		{
		case 1: //메뉴 1번 눌렀을 때 bridge 출력
			for (i = 0; i < Size; i++) {
				if (bridge[i] != -1) {
					printf("< %d %d >\n", i, bridge[i]);
				}
			}
			if (bridge[i] == -1) { 
				printf("bridge가 없습니다.\n");
				for (i = 0; i < Size; i++)
					printf("%d\n", bridge[i]);
			}
			break;
		case 2:
			printf("간선의 양 끝점을 입력하고 엔터를 누르세요:> ");
			scanf_s("%d %d", &a, &b);

			if ((bridge[a] != -1 && bridge[a] == b)) {
				printf("< %d %d >는 Bridge 입니다.\n", a, b);
			}
			else {
				printf("< %d %d >는 Bridge가 아닙니다.\n", a, b);
			}
			break;
		case 3:
			exit(0);
		}
	}

	
}