#include<stdio.h>
#include <conio.h>
#include <math.h>
#include <stdbool.h>
#define MAX 10000
#define MAX_Q 100
struct Process{
	// arrival time, cpu time, respinse time, waiting time, turnaround time
	int arrivalTime, cpuTime, RT, WT, TAT; 
};

//==================================
typedef struct{
	int front, rear;
	int Queue[MAX_Q];
}Queue;

void makenull_queue(Queue *Q)
{
	Q->front = 0;
	Q->rear = -1;
}

int empty_queue(Queue *Q)
{
	return Q->front > Q->rear;
}
void push_queue(Queue *Q, int x)
{
	Q->rear++;
	Q->Queue[Q->rear] = x;
}

int top_queue(Queue *Q)
{
	return Q->Queue[Q->front];
}

void pop_queue(Queue *Q)
{
	Q->front++;
}

//==================================

int maxTime(int n, int endTime[]) {
	int allTime = endTime[1];
	int u;
	for(u = 1; u <= n; u++) {
		if(endTime[u] < endTime[u+1]) {
			if(endTime[u+1] > allTime) {
				allTime = endTime[u+1];
			}
		}
	}
	return allTime;
}

// Gantt chart
void gantt_Chart(struct Process *p, int n, int startTime[], int endTime[]) {
	int i, j;
	int allTime = maxTime(n, endTime);
	for(i = 0; i < n; i++) {
		for(j = 0; j <= allTime; j++) {
			if(j >= startTime[i+1] && j <= endTime[i+1]){
				printf("R");
			}else if(j >= p[i+1].arrivalTime && j < startTime[i+1]) {
					printf("W");
				} else printf("-");
		}
		printf("\n");
	}
}

void gantt_ChartPreemptive(struct Process *p, int n, int startTime[], int endTime[]) {
	int i, j, u, v = 1, check = 0;
	int allTime = maxTime(n, endTime);
	while(v != n+1) {
		for(u = 0; u <= allTime; u++) {
			check = 0;
			for(i = v; i <= n; i++) {
				if(u > endTime[i] || u < startTime[i]){
					printf("-");
					break;
				}
				for(j = i+1; j <= n; j++) {
					if(u >= startTime[j] && u < endTime[j] && u != endTime[i] && u != startTime[i]) {
						printf("W");
						check = 1;
						break;
					} 	
				}
				if(check == 1) break;
				if(u >= startTime[i] && u <= endTime[i]) {
					printf("R");
					check = 1;
					break;
				}
				
			}
		}
		v++;
		printf("\n");
	}
}


void FSFC(struct Process *p, int n){
	
	int is_completed[100];
	int currentTime = 0, completed = 0, prev = 0;
	int i, j;
	int startTime[100], endTime[100];
	
	
	for(i = 1; i <= n; i++) {
		is_completed[i] = 0;
		startTime[i] = 0;
		endTime[i] = 0;
	}
	
	while(completed != n) {
		int idx = -1;
		int min = MAX;
		for(i = 1; i <= n; i++) {
			if(p[i].arrivalTime <= currentTime && is_completed[i] == 0) {
				if(p[i].cpuTime < min) {
					min = p[i].cpuTime;
					idx = i;
				}
			}
		}
		if(idx != -1) {
			startTime[idx] = currentTime;
			endTime[idx] = startTime[idx] + p[idx].cpuTime;
			p[idx].TAT = endTime[idx] - p[idx].arrivalTime;
			p[idx].WT = p[idx].TAT - p[idx].cpuTime;
			p[idx].RT = startTime[i] - p[idx].arrivalTime;
			
			is_completed[idx] = 1;
			completed++;
			currentTime = endTime[idx];
			prev = currentTime;
		} else currentTime++;
	}
	
	// Gantt chart
	gantt_Chart(p, n, startTime, endTime);
}

void in_FCFS(struct Process *p, int n) {
	int i;
	printf("************************************\n");
	printf("Dinh thoi FCFS\n");
	FSFC(p, n);
	int sumW = 0, sumT = 0;
	float AVGW = 0.0, AVGR = 0.0, AVGT = 0.0;
	for(i = 1; i <= n; i++) {
		sumW += p[i].WT;
		sumT += p[i].TAT;
	}
	AVGW = (float)(sumW)/n;
	AVGT = (float)(sumT)/n;
	AVGR = AVGW;
	printf("AVGW= %.2f \t AVGR= %.2f \t AVGT= %.2f\n", AVGW, AVGR, AVGT);
	printf("************************************\n");
}

void SJF_Non_Preemptive(struct Process *p, int n){
	int is_completed[100];
	int startTime[100], endTime[100];
	int currentTime = 0, completed = 0, prev = 0;
	int i, j;
	
	for(i = 1; i <= n; i++) {
		is_completed[i] = 0;
		startTime[i] = 0;
		endTime[i] = 0;
	}
	
	while(completed != n) {
		int idx = -1;
		int min = MAX;
		for(i = 1; i <= n; i++) {
			if(p[i].arrivalTime <= currentTime && is_completed[i] == 0) {
				if(p[i].cpuTime < min) {
					min = p[i].cpuTime;
					idx = i;
				}
				if(p[i].cpuTime == min) {
					if(p[i].arrivalTime < p[idx].arrivalTime) {
						min = p[i].cpuTime;
						idx = i;
					}
				}
			}
		}
		if(idx != -1) {
			startTime[idx] = currentTime;
			endTime[idx] = startTime[idx] + p[idx].cpuTime;
			p[idx].TAT = endTime[idx] - p[idx].arrivalTime;
			p[idx].WT = p[idx].TAT - p[idx].cpuTime;
			p[idx].RT = startTime[idx] - p[idx].arrivalTime;
			
			is_completed[idx] = 1;
			completed++;
			currentTime = endTime[idx];
			prev = currentTime; 
		} else currentTime++;
	}
	
	//Gantt chart
	gantt_Chart(p, n, startTime, endTime);

}

void in_SJF_Non_Preemptive(struct Process *p, int n) {
	int i;
	printf("Dinh thoi SJF non Preeptive\n");
	SJF_Non_Preemptive(p, n);
	int sumW = 0, sumT = 0;
	float AVGW = 0.0, AVGR = 0.0, AVGT = 0.0;
	for(i = 1; i <= n; i++) {
		sumW += p[i].WT;
		sumT += p[i].TAT;
	}
	AVGW = (float)(sumW)/n;
	AVGT = (float)(sumT)/n;
	AVGR = AVGW;
	printf("AVGW= %.2f \t AVGR= %.2f \t AVGT= %.2f\n", AVGW, AVGR, AVGT);
	printf("************************************\n");
}

void SJF_Preemptive(struct Process *p, int n){

	int is_completed[100];
	int startTime[100], endTime[100], reduceTime[100];
	int currentTime = 0, completed = 0, prev = 0;
	int i, j;
	
	for(i = 1; i <= n; i++) {
		is_completed[i] = 0;
		startTime[i] = 0;
		endTime[i] = 0;
		reduceTime[i] = p[i].cpuTime;
	}
	
	while(completed != n) {
		int idx = -1;
		int min = MAX;
		for(i = 1; i <= n; i++) {
			if(p[i].arrivalTime <= currentTime && is_completed[i] == 0) {
				if(reduceTime[i] < min) {
					min = reduceTime[i];
					idx = i;
				}
				if(reduceTime[i] == min) {
					if(p[i].arrivalTime < p[idx].arrivalTime) {
						min = reduceTime[i];
						idx = i;
					}
				}
			}
		}
		if(idx != -1) {
			if(reduceTime[idx] == p[idx].cpuTime) {
				startTime[idx] = currentTime;
			}
			reduceTime[idx]--;
			currentTime++;
			prev = currentTime;
			
			if(reduceTime[idx] == 0) {
				endTime[idx] = currentTime;
				p[idx].TAT = endTime[idx] - p[idx].arrivalTime;
				p[idx].WT = p[idx].TAT - p[idx].cpuTime;
				p[idx].RT = startTime[idx] - p[idx].arrivalTime;
				
				is_completed[idx] = 1;
				completed++;
			}
		} else currentTime++;
	}
	
	// Gantt chart
	gantt_ChartPreemptive(p, n, startTime, endTime);

}
void in_SJF_Preemptive(struct Process *p, int n) {
	int i;
	printf("Dinh thoi SJF Preeptive\n");
	SJF_Preemptive(p, n);
	int sumW = 0,sumR = 0, sumT = 0;
	float AVGW = 0.0, AVGR = 0.0, AVGT = 0.0;
	for(i = 1; i <= n; i++) {
		sumW += p[i].WT;
		sumR += p[i].RT;
		sumT += p[i].TAT;
	}
	AVGW = (float)(sumW)/n;
	AVGR = (float)(sumR)/n;
	AVGT = (float)(sumT)/n;

	printf("AVGW= %.2f \t AVGR= %.2f \t AVGT= %.2f\n", AVGW, AVGR, AVGT);
	printf("************************************\n");
}

int max(int a, int b) {
	if(a > b) {
		return a;
	} else return b;
}
void Round_Bin(struct Process *p, int n, int q) {
	int startTime[100], endTime[100], reduceTime[100], mark[100];
	int currentTime = 0, completed = 0, prev = 0;
	int i, j, idx;
	
	for(i = 1; i <= n; i++) {
		mark[i] = 0;
		startTime[i] = 0;
		endTime[i] = 0;
		reduceTime[i] = p[i].cpuTime;
	}
	Queue Q;
	makenull_queue(&Q);
	mark[1] = 0;
	push_queue(&Q, 1);
	while(completed != n) {
		idx = top_queue(&Q);
		pop_queue(&Q);
		
		if(reduceTime[idx] == p[idx].cpuTime) {
			startTime[idx] = max(currentTime, p[idx].arrivalTime);
			currentTime = startTime[idx];
		}
		
		if(reduceTime[idx] - q > 0) {
			reduceTime[idx] = reduceTime[idx] - q;
			currentTime += q;

		} else {
			currentTime += reduceTime[idx];
			reduceTime[idx] = 0;
			completed++;
			endTime[idx] = currentTime;
			p[idx].TAT = endTime[idx] - p[idx].arrivalTime;
			p[idx].WT = p[idx].TAT - p[idx].cpuTime;
			p[idx].RT = startTime[idx] - p[idx].arrivalTime;
		}
		for(i = 2; i <= n; i++) {
			if(reduceTime[i] > 0 && p[i].arrivalTime <= currentTime && mark[i] == 0) {
				push_queue(&Q, i);
				mark[i] = 1;
			}
		}
		if(reduceTime[idx] > 0) {
			push_queue(&Q, idx);
		}
		if(empty_queue(&Q)) {
			for(i = 2; i <= n; i++) {
				if(reduceTime[i] > 0) {
					push_queue(&Q, i);
					mark[i] = 1;
					break;
				}
			}
		}
	}
	gantt_ChartPreemptive(p, n, startTime, endTime);
}

void in_RR(struct Process *p, int n, int q) {
	int i;
	printf("Dinh thoi Round Bin\n");
	Round_Bin(p, n, q);

	int sumW = 0,sumR = 0, sumT = 0;
	float AVGW = 0.0, AVGR = 0.0, AVGT = 0.0;
	for(i = 1; i <= n; i++) {
		sumW += p[i].WT;
		sumR += p[i].RT;
		sumT += p[i].TAT;
	}
	AVGW = (float)(sumW)/n;
	AVGR = (float)(sumR)/n;
	AVGT = (float)(sumT)/n;

	printf("AVGW= %.2f \t AVGR= %.2f \t AVGT= %.2f\n", AVGW, AVGR, AVGT);
	printf("************************************\n");
}

void main() {
	int n, i, j, q;
	struct Process p[100], temp;
	
	printf("Nhap so tien trinh va quantum\n");
	scanf("%d%d",&n, &q);
	printf("Nhap arrival time va cpu time\n");
	for(i = 1; i <= n; i++) {	
		scanf("%d %d", &p[i].arrivalTime, &p[i].cpuTime);
	}
	
//	 sort arrival time
	for(i = 1; i <= n; i++) {
		for(j = i+1; j < n ; j++) {
			if(p[i].arrivalTime > p[j].arrivalTime) {
				temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}
		}
	}

	in_FCFS(p, n);
	in_SJF_Non_Preemptive(p, n);
	in_SJF_Preemptive(p,n);
	in_RR(p, n, q);
}
