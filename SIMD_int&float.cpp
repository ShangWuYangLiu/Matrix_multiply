#include <intrin.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#define data 3200
using namespace std;

//int版本转置
void swap_i(int** x)
{
	for (int i = 0; i < data; i++)
	{
		for (int j = 0; j < i; j++)
		{
			int temp = x[i][j];
			x[i][j] = x[j][i];
			x[j][i] = temp;
		}
	}
}

//float版本转置
void swap_f(float** x)
{
	for (int i = 0; i < data; i++)
	{
		for (int j = 0; j < i; j++)
		{
			float temp = x[i][j];
			x[i][j] = x[j][i];
			x[j][i] = temp;
		}
	}
}

//普通矩阵乘-int
void mult_i(int **a,int **b,int **c)
{
	for(int i=0;i<data;i++)
	{
		for(int j=0;j<data;j++)
		{
			c[i][j]=0;
			for(int k=0;k<data;k++)
			{
				c[i][j]=a[i][k]*b[k][j];
			}
		}
	}
}

//普通矩阵乘-float
void mult_f(float** a, float** b, float** c)
{
	for (int i = 0; i < data; i++)
	{
		for (int j = 0; j < data; j++)
		{
			c[i][j] = 0.0;
			for (int k = 0; k < data; k++)
			{
				c[i][j] = a[i][k] * b[k][j];
			}
		}
	}
}

//sse-float
void SSE_mult_f(float **A, float **B, float **C)
{
	//1.将b转置
	swap_f(B);
	//2.进行乘法
	__m128 temp1, temp2, temp, sum;
	for (int i = 0; i < data; i++)
	{
		for (int j = 0; j < data; j++)
		{
			C[i][j] = 0.0;
			sum = _mm_setzero_ps();//sum置为0
			//一次性处理4个数，剩余部分单独处理
			for (int k = 0; k < (data/4)*4; k=k+4)
			{
				//将A矩阵第i行和B矩阵第j行的每2个元素存入temp1，temp2中以便运算
				temp1 = _mm_loadu_ps(A[i] + k);
				temp2= _mm_loadu_ps(B[j] + k);
				temp = _mm_mul_ps(temp1, temp2);//前四个数对应相乘
				sum = _mm_add_ps(sum, temp);//将结果放入sum中
			}
			sum = _mm_hadd_ps(sum, sum);
			sum = _mm_hadd_ps(sum, sum);//得到C[i][j]
			_mm_store_ss(C[i] + j, sum);
			for (int k = (data / 4) * 4; k < data;k++)
			{
				C[i][j] += A[i][k] * B[j][k];//处理剩余部分元素
			}
		}
	}
	//3.还原B
	swap_f(B);
}

//sse-int
void SSE_mult_i(int** A, int** B, int** C)
{
	//1.将b转置
	swap_i(B);
	//2.进行乘法
	__m128i temp1,temp2,temp;
	for (int i = 0; i < data; i++)
	{
		for (int j = 0; j < data; j++)
		{
			C[i][j] = 0; int sum = 0;
			//一次性处理4个数，剩余部分单独处理
			for (int k = 0; k < (data / 4) * 4; k = k + 4)
			{
				temp1 = _mm_set_epi32(A[i][k],A[i][k + 1],A[i][k + 2],A[i][k + 3]);
				temp2 = _mm_set_epi32(B[j][k],B[j][k + 1],B[j][k + 2],B[j][k + 3]);
 				temp=_mm_mullo_epi32(temp1,temp2);
				sum = sum + temp.m128i_i32[0] + temp.m128i_i32[1] + temp.m128i_i32[2] + temp.m128i_i32[3];
			}
			C[i][j] = sum;
			for (int k = (data / 4) * 4; k < data; k++)
			{
				C[i][j] += A[i][k] * B[j][k];//处理剩余部分元素
			}
		}
	}
	//3.还原B
	swap_i(B);
}

void main()
{
	int i,j;
	float **a,**b,**c;
	int** A, ** B, ** C;
	clock_t start,end;
	a=new float*[data];
	b=new float*[data];
	c=new float*[data];
	A = new int* [data];
	B = new int* [data];
	C = new int* [data];
	for(i=0;i<data;i++)
	{
		a[i]=new float[data];
		b[i]=new float[data];
		c[i]=new float[data];
		A[i] = new int [data];
		B[i] = new int [data];
		C[i] = new int [data];
	}
	for(i=0;i<data;i++)
	{
		for(j=0;j<data;j++)
		{
			a[i][j]= 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - 1)));
			b[i][j]= 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - 1)));
			A[i][j] = rand() % 10;
			B[i][j] = rand() % 10;
		}
	}
	cout << "begining…" << endl;
	start=clock();
	mult_i(A,B,C);
	end=clock();
	double endtime = double(end - start) / CLOCKS_PER_SEC;
	cout<<"普通乘法（i）耗时："<<endtime*1000<<"ms"<<endl;
	start=clock();
	SSE_mult_i(A,B,C);
	end=clock();
	endtime = double(end - start) / CLOCKS_PER_SEC;
	cout<<"SSE算法（i）耗时:"<<endtime*1000<<"ms"<<endl;
	start = clock();
	mult_f(a, b, c);
	end = clock();
	double endtime = double(end - start) / CLOCKS_PER_SEC;
	cout << "普通乘法（f）耗时：" << endtime * 1000 << "ms" << endl;
	start = clock();
	SSE_mult_f(a, b, c);
	end = clock();
	endtime = double(end - start) / CLOCKS_PER_SEC;
	cout << "SSE算法（f）耗时:" << endtime * 1000 << "ms" << endl;
	cout << "finished";
}
