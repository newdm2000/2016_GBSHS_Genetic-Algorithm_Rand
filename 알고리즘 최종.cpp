#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <algorithm>

#define INDI_GENE 4						//한 개체의 유전자 수
#define GENER_INDI 10					//한 세대의 개체 수 
#define SURV_GENE 7						//살아남는 개체 수 
#define MUTANT_POS 15					//돌연변이 비율 (1/n) 
#define GET_MAX 100						//유전자 최댓값 
#define GET_MIN 30						//유전자 최솟값 

void get_start_gene();					//처음 세대의 유전자 생성
int *get_gene();						//기존 세대의 유전자 받기 
int *get_sol();							//목표값 받기 
int *get_feedback(int *, int *);		// 보상값 받기 
int *study(int*, int*);					//개체 적자생존 및 개체 교배 
int *tmp;

int main()
{
	int mode, i, j, k, l=1, tou, y;
	int *gene, *sol, *feedback, *next_gene;
	
	printf("mode (start : 1, exist : 2) : ");
	scanf("%d", &mode);
	
	FILE *fpros = fopen("process.txt", "wt");
	
	sol = get_sol();
	srand((unsigned int)time(NULL));
	
	if(mode == 1)														//
	{
		get_start_gene();
		gene = get_gene();
	}
	
	else if(mode == 2)
	{
		gene = get_gene();
	}
	
	//printf("\n\n\nGeneration 0\n\n--------gene---------\n");
	fprintf(fpros, "\n\n\nGeneration 0\n\n--------gene---------\n");
	
	for (i = 0; i<GENER_INDI; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			//printf("%d ", gene[i*INDI_GENE + j]);
			fprintf(fpros, "%d ", gene[i*INDI_GENE + j]);
		}
		//printf("\n");
		fprintf(fpros, "\n");
	}
	
	
	do
	{
	
	feedback = get_feedback(gene, sol);
	next_gene = study(gene, feedback);
	
	
	for (i = 0; i<GENER_INDI; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			gene[i*INDI_GENE + j] = next_gene[i*INDI_GENE + j];
		}
	}
	
	//printf("\n\n\nGeneration %d\n\n--------gene---------\n", l);
	fprintf(fpros, "\n\n\nGeneration %d\n\n--------gene---------\n", l);
	for (i = 0; i<GENER_INDI; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			//printf("%d ", gene[i*INDI_GENE + j]);
			fprintf(fpros, "%d ", gene[i*INDI_GENE + j]);
		}
		//printf("\n");
		fprintf(fpros, "\n");
	}
	//printf("---------------------\n--------feedback--------\n");
	fprintf(fpros, "---------------------\n--------feedback--------\n");
	for (i = 0; i<GENER_INDI; i++)
	{
		//printf("%d ", feedback[i]);
		fprintf(fpros, "%d ", feedback[i]);
	}
	//printf("\n---------------------\n");
	fprintf(fpros, "\n---------------------\n");
	
	free(feedback);
	free(next_gene);
	l++;
} while(feedback[GENER_INDI] >= 5*(GENER_INDI));


printf("Generation : %d", l);
	/*
	for (i = 0; i<GENER_INDI; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			printf("%d ", gene[i*INDI_GENE + j]);
		}
		printf("\n");
	}
	
		printf("\n\n");
		
	for (j = 0; j<INDI_GENE; j++)
	{
		printf("%d ", sol[j]);
	}
	
		printf("\n\n");
	
	for (i = 0; i<GENER_INDI; i++)
	{
		printf("%d ", feedback[i]);
	}
	*/
	fclose(fpros);
	return 0;
}

void get_start_gene()
{
	FILE *fgene = fopen("gene.txt", "wt");
	if(fgene == NULL) 
	{
		printf("failed open file\n"); 
	}
	int *gene_start, i, j;
	gene_start = (int *)malloc(GENER_INDI*INDI_GENE*sizeof(int));
	for (i = 0; i<GENER_INDI; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			gene_start[i*INDI_GENE + j] = (rand() % (GET_MAX - GET_MIN + 1) + GET_MIN);
			fprintf(fgene, "%d ", gene_start[i*INDI_GENE + j]);
		}
		fprintf(fgene, "\n");
	}
	free(gene_start);
	fclose(fgene);
}

int *get_gene()
{
	FILE *fgene = fopen("gene.txt", "rt");
	if(fgene == NULL) 
	{
		printf("failed open file\n");
	}
	
	int *gene, i, j;
	gene = (int *)malloc(GENER_INDI*INDI_GENE*sizeof(int));
	for (i = 0; i<GENER_INDI; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			fscanf(fgene, "%d ", &gene[i*INDI_GENE + j]);
			fflush(stdin);
		}
	}
	fclose(fgene);
	return gene;
}

int *get_sol()
{
	FILE *fsol = fopen("sollution.txt", "rt");
	if(fsol == NULL) 
	{
		printf("failed open file\n");
	}
	
	int *sol, i, j;
	sol = (int *)malloc(INDI_GENE*sizeof(int));
	for (j = 0; j<INDI_GENE; j++)
	{
		fscanf(fsol, "%d ", &sol[j]);
		fflush(stdin);
	}
	fclose(fsol);
	return sol;
}

int *get_feedback(int *gene, int *sol)
{
	int *feedback, i, j, dev;
	
	feedback = (int *)malloc((GENER_INDI+1)*sizeof(int));
	feedback[GENER_INDI] = 0;
	for (i = 0; i<GENER_INDI; i++)
	{
		dev = 0;
		for (j = 0; j<INDI_GENE; j++)
		{
			dev = dev + abs(gene[i*INDI_GENE + j]-sol[j]);
		}
		feedback[i]=dev;
	}
	for (i = 0; i<GENER_INDI; i++)
	{
		feedback[GENER_INDI] = feedback[GENER_INDI] + feedback[i];
	}
	
	return feedback;
}

bool cmp(int i, int j) {
	return tmp[i] < tmp[j];
}

int* study(int *gene, int *feedback)
{
	int *new_gene, *surv_indi, *rank, i, j, k, l=0, ra;
	
	
	surv_indi = (int *)malloc(INDI_GENE*SURV_GENE*sizeof(int));
	rank = (int *)malloc(GENER_INDI*sizeof(int));
	new_gene = (int *)malloc(INDI_GENE*GENER_INDI*sizeof(int));
	
	
	for(i=0;i<GENER_INDI;i++) rank[i]=i;
	tmp=feedback;
	std::sort(rank,rank+GENER_INDI,cmp);
	
	for(i=0; i<SURV_GENE; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			surv_indi[i*INDI_GENE + j] = gene[rank[i]*INDI_GENE + j];
		}
	}
	
	for(i=0; i<GENER_INDI; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			ra = rand() % SURV_GENE;
			new_gene[i*INDI_GENE + j] = surv_indi[ra*INDI_GENE + j];
		}
	}

	/*
	for(i=0; i<SURV_GENE; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			new_gene[(i+GENER_INDI-SURV_GENE)*INDI_GENE + j] = surv_indi[i*INDI_GENE + j];
		}
	}
	*/
	for(i=0; i<GENER_INDI; i++)
	{
		for (j = 0; j<INDI_GENE; j++)
		{
			if(l == (rand() % MUTANT_POS))
			new_gene[i*INDI_GENE + j] = (rand() % (GET_MAX - GET_MIN + 1) + GET_MIN);
		}
	}
	
	
	
	
	
	return new_gene;
}
