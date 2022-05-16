#include<stdio.h>
#include<stdlib.h>

#define n 32
void valueC(int size, int finro, int sinco, int* temp, int* C)
{
	if (finro == 0)
	{
		if (sinco == 0)
		{
			int i, j;
			for (i = 0; i < size; i++)
			{
				for (j = 0; j < size; j++)
				{
					C[i * n + j] += temp[i * size + j];
				}
			}
		}
		else
		{
			int i, j;
			for (i = 0; i < size; i++)
			{
				for (j = size; j < 2 * size; j++)
				{
					C[i * n + j] += temp[i * size + (j - size)];
				}
			}
		}
	}
	else
	{
		if (sinco == 0)
		{
			int i, j;
			for (i = size; i < 2 * size; i++)
			{
				for (j = 0; j < size; j++)
				{
					C[i * n + j] += temp[(i - size) * size + j];
				}
			}
		}
		else
		{
			int i, j;
			for (i = size; i < 2 * size; i++)
			{
				for (j = size; j < 2 * size; j++)
				{
					C[i * n + j] += temp[(i - size) * size + (j - size)];
				}
			}
		}
	}
}

void mult1(int* A, int* B, int* C, int size, int finro, int finco, int sinro, int sinco)//顺序为ikj
{
	int* temp = NULL;
	temp = (int*)malloc(sizeof(int) * (size * size));
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			temp[i * size + j] = 0;
		}
	}
	for (i = finro; i < size + finro; i++)
	{
		int k1 = finco; int k2 = sinro;
		while (k1 < size + finco && k2 < size + sinro)
		{
			int s = A[i * n + k1];
			for (j = sinco; j < size + sinco; j++)
			{
				temp[(i - finro) * size + (j - sinco)] += s * B[k2 * n + j];
			}
			k1++; k2++;
		}
	}
	valueC(size, finro, sinco, temp, C);
}

void sinblockmult1(int* A, int* B, int* C, int nn)//单线程分块矩阵乘法
{
	int size = nn / 2;//每一个分块都是size*size的矩阵
	//矩阵乘法
	//1.计算C00
	mult1(A, B, C, size, 0, 0, 0, 0);
	mult1(A, B, C, size, 0, size, size, 0);
	//2.计算C01
	mult1(A, B, C, size, 0, 0, 0, size);
	mult1(A, B, C, size, 0, size, size, size);
	//3.计算C10
	mult1(A, B, C, size, size, 0, 0, 0);
	mult1(A, B, C, size, size, size, size, 0);
	//4.计算C11
	mult1(A, B, C, size, size, 0, 0, size);
	mult1(A, B, C, size, size, size, size, size);
}

int main()
{
	int* A = (int*)malloc(sizeof(int) * (n * n));
	int* B = (int*)malloc(sizeof(int) * (n * n));
	int i, j;
	for (int i = 0; i < n; i++)//随机生成10以内的数作为A、B矩阵内元素
	{
		for (int j = 0; j < n; j++)
		{
			A[i * n + j] = (rand() % 10);
			B[i * n + j] = (rand() % 10);
		}
	}
	int* C = (int*)malloc(sizeof(int) * (n * n));
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			C[i * n + j] = 0;
		}
	}
	sinblockmult1(A, B, C, n);
	return 0;
}
