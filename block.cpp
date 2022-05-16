#include<iostream>
#include<thread>
#include<ctime>

using namespace std;

void valueC(int size, int finro, int sinco, int** temp, int** C)
{
	if (finro == 0)
	{
		if (sinco == 0)
		{
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					C[i][j] += temp[i][j];
				}
			}
		}
		else
		{
			for (int i = 0; i < size; i++)
			{
				for (int j = size; j < 2 * size; j++)
				{
					C[i][j] += temp[i][j - size];
				}
			}
		}
	}
	else
	{
		if (sinco == 0)
		{
			for (int i = size; i < 2 * size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					C[i][j] += temp[i - size][j];
				}
			}
		}
		else
		{
			for (int i = size; i < 2 * size; i++)
			{
				for (int j = size; j < 2 * size; j++)
				{
					C[i][j] += temp[i - size][j - size];
				}
			}
		}
	}
}

void transform(int** result,int sinro,int sinco,int size,int** B)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			result[i][j] = B[sinro+j][sinco+i];
		}
	}
}

void mult2(int** A, int** B, int** C, int size, int finro, int finco, int sinro, int sinco)//转置
{
	int** temp = NULL;
	temp = new int* [size];
	for (int i = 0; i < size; i++)
		temp[i] = new int[size];
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			temp[i][j] = 0;
		}
	}
	int** result = NULL;
	result = new int* [size];
	for (int i = 0; i < size; i++)
		result[i] = new int[size];
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			result[i][j] = 0;
		}
	}
	transform(result, sinro, sinco, size, B);
	for (int i = finro; i < size + finro; i++)
	{
		for (int j = sinco; j < size + sinco; j++)
		{
			int k1 = finco; int k2 = 0;
			while (k1 < finco + size && k2 <size)
			{
				temp[i - finro][j - sinco] += A[i][k1] * result[j-sinco][k2];
				k1++; k2++;
			}
		}
	}
	valueC(size, finro, sinco, temp, C);
}
//f为第一个矩阵，s为第二个矩阵
void mult1(int** A, int** B, int** C, int size,int finro,int finco,int sinro,int sinco)//顺序为ikj
{
	int** temp = NULL;
	temp = new int* [size];
	for (int i = 0; i < size; i++)
		temp[i] = new int[size];
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			temp[i][j] = 0;
		}
	}
    for (int i = finro; i < size + finro; i++)
    {
        int k1 = finco; int k2 = sinro;
        while (k1 < size + finco && k2 < size + sinro)
        {
            int s = A[i][k1];
            for (int j = sinco; j < size + sinco; j++)
            {
                temp[i - finro][j - sinco] += s * B[k2][j];
            }
            k1++; k2++;
        }
    }
	valueC(size, finro, sinco, temp, C);
}

void sinblockmult2(int** A, int** B, int** C,int n)//单线程分块矩阵乘法
{
	int size = n / 2;//每一个分块都是size*size的矩阵
	//矩阵乘法
	//1.计算C00
	mult2(A, B, C, size, 0, 0, 0, 0);
	mult2(A, B, C, size, 0, size, size, 0);
	//2.计算C01
	mult2(A, B, C, size, 0, 0, 0, size);
	mult2(A, B, C, size, 0, size, size, size);
	//3.计算C10
	mult2(A, B, C, size, size, 0, 0, 0);
	mult2(A, B, C, size, size, size, size, 0);
	//4.计算C11
	mult2(A, B, C, size, size, 0, 0, size);
	mult2(A, B, C, size, size, size, size, size);
}
void sinblockmult1(int** A, int** B, int** C, int n)//单线程分块矩阵乘法
{
	int size = n / 2;//每一个分块都是size*size的矩阵
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
	int n;
	cin >> n;//输入规模
	int** A = NULL;
	A = new int* [n];
	for (int i = 0; i < n; i++)
		A[i] = new int[n];
	int** B = NULL;
	B = new int* [n];
	for (int i = 0; i < n; i++)
		B[i] = new int[n];
	for (int i = 0; i < n; i++)//随机生成10以内的数作为A、B矩阵内元素
	{
		for (int j = 0; j < n; j++)
		{
			A[i][j] = int(rand() % 10);
			B[i][j] = int(rand() % 10);
		}
	}
	int** C = NULL;
	C = new int* [n];
	for (int i = 0; i < n; i++)
		C[i] = new int[n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			C[i][j] = 0;
		}
	}
	//====================八线程+ikj=====================//
	double ave0 = 0;
	clock_t start, end;
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		thread t10 = thread(mult2, A, B, C, n / 2, 0, 0, 0, 0);
		thread t11 = thread(mult2, A, B, C, n / 2, 0, n / 2, n / 2, 0);
		thread t12 = thread(mult2, A, B, C, n / 2, 0, 0, 0, n / 2);
		thread t13 = thread(mult2, A, B, C, n / 2, 0, n / 2, n / 2, n / 2);
		thread t14 = thread(mult2, A, B, C, n / 2, n / 2, 0, 0, 0);
		thread t15 = thread(mult2, A, B, C, n / 2, n / 2, n / 2, n / 2, 0);
		thread t16 = thread(mult2, A, B, C, n / 2, n / 2, 0, 0, n / 2);
		thread t17 = thread(mult2, A, B, C, n / 2, n / 2, n / 2, n / 2, n / 2);
		t10.join(); t11.join(); t12.join(); t13.join(); t14.join(); t15.join(); t16.join(); t17.join();
		end = clock();
		ave0 = ave0 + (double)(end - start) / CLOCKS_PER_SEC;
	}
	cout << "10次八线程+ikj平均所需时间:" << (ave0/10) * 1000 << "ms" << endl;
	//====================八线程+转置=====================//
	double ave1 = 0;
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		thread t20 = thread(mult1, A, B, C, n / 2, 0, 0, 0, 0);
		thread t21 = thread(mult1, A, B, C, n / 2, 0, n / 2, n / 2, 0);
		thread t22 = thread(mult1, A, B, C, n / 2, 0, 0, 0, n / 2);
		thread t23 = thread(mult1, A, B, C, n / 2, 0, n / 2, n / 2, n / 2);
		thread t24 = thread(mult1, A, B, C, n / 2, n / 2, 0, 0, 0);
		thread t25 = thread(mult1, A, B, C, n / 2, n / 2, n / 2, n / 2, 0);
		thread t26 = thread(mult1, A, B, C, n / 2, n / 2, 0, 0, n / 2);
		thread t27 = thread(mult1, A, B, C, n / 2, n / 2, n / 2, n / 2, n / 2);
		t20.join(); t21.join(); t22.join(); t23.join(); t24.join(); t25.join(); t26.join(); t27.join();
		end = clock();
		ave1 = ave1 + (double)(end - start) / CLOCKS_PER_SEC;
	}
	cout << "10次八线程+转置平均所需时间:" << (ave1 / 10) * 1000 << "ms" << endl;
	//====================单线程+ikj=====================//
	double ave2 = 0;
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		sinblockmult1(A, B, C, n);
		end = clock();
		ave2 = ave2 + (double)(end - start) / CLOCKS_PER_SEC;
	}
	cout << "10次单线程+ikj平均所需时间" << (ave2 / 10) * 1000 << "ms" << endl;
	//====================单线程+转置=====================//
	double ave3 = 0;
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		sinblockmult2(A, B, C, n);
		end = clock();
		ave3 = ave3 + (double)(end - start) / CLOCKS_PER_SEC;
	}
	cout << "10次单线程+转置平均所需时间" << (ave3 / 10) * 1000 << "ms" << endl;
	//====================四线程+ikj=====================//
	double ave4 = 0;
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		thread t30 = thread(mult2, A, B, C, n / 2, 0, 0, 0, 0);
		thread t31 = thread(mult2, A, B, C, n / 2, 0, n / 2, n / 2, 0);
		thread t32 = thread(mult2, A, B, C, n / 2, 0, 0, 0, n / 2);
		thread t33 = thread(mult2, A, B, C, n / 2, 0, n / 2, n / 2, n / 2);
		t30.join(); t31.join(); t32.join(); t33.join();
		thread t34 = thread(mult2, A, B, C, n / 2, n / 2, 0, 0, 0);
		thread t35 = thread(mult2, A, B, C, n / 2, n / 2, n / 2, n / 2, 0);
		thread t36 = thread(mult2, A, B, C, n / 2, n / 2, 0, 0, n / 2);
		thread t37 = thread(mult2, A, B, C, n / 2, n / 2, n / 2, n / 2, n / 2);
		t34.join(); t35.join(); t36.join(); t37.join();
		end = clock();
		ave4 = ave4 + (double)(end - start) / CLOCKS_PER_SEC;
	}
	cout << "10次四线程+ikj平均所需时间" << (ave4 / 10) * 1000 << "ms" << endl;
	//====================四线程+转置=====================//
	double ave5 = 0;
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		thread t40 = thread(mult1, A, B, C, n / 2, 0, 0, 0, 0);
		thread t41 = thread(mult1, A, B, C, n / 2, 0, n / 2, n / 2, 0);
		thread t42 = thread(mult1, A, B, C, n / 2, 0, 0, 0, n / 2);
		thread t43 = thread(mult1, A, B, C, n / 2, 0, n / 2, n / 2, n / 2);
		t40.join(); t41.join(); t42.join(); t43.join();
		thread t44 = thread(mult1, A, B, C, n / 2, n / 2, 0, 0, 0);
		thread t45 = thread(mult1, A, B, C, n / 2, n / 2, n / 2, n / 2, 0);
		thread t46 = thread(mult1, A, B, C, n / 2, n / 2, 0, 0, n / 2);
		thread t47 = thread(mult1, A, B, C, n / 2, n / 2, n / 2, n / 2, n / 2);
		t44.join(); t45.join(); t46.join(); t47.join();
		end = clock();
		ave5 = ave5 + (double)(end - start) / CLOCKS_PER_SEC;
	}
	cout << "10次四线程+转置平均所需时间" << (ave5 / 10) * 1000 << "ms" << endl;
	//====================二线程+ikj=====================//
	double ave6 = 0;
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		thread t50 = thread(mult2, A, B, C, n / 2, 0, 0, 0, 0);
		thread t51 = thread(mult2, A, B, C, n / 2, 0, n / 2, n / 2, 0);
		t50.join(); t51.join(); 
		thread t52 = thread(mult2, A, B, C, n / 2, 0, 0, 0, n / 2);
		thread t53 = thread(mult2, A, B, C, n / 2, 0, n / 2, n / 2, n / 2);
		t52.join(); t53.join();
		thread t54 = thread(mult2, A, B, C, n / 2, n / 2, 0, 0, 0);
		thread t55 = thread(mult2, A, B, C, n / 2, n / 2, n / 2, n / 2, 0);
		t54.join(); t55.join(); 
		thread t56 = thread(mult2, A, B, C, n / 2, n / 2, 0, 0, n / 2);
		thread t57 = thread(mult2, A, B, C, n / 2, n / 2, n / 2, n / 2, n / 2);
		t56.join(); t57.join();
		end = clock();
		ave6 = ave6 + (double)(end - start) / CLOCKS_PER_SEC;
	}
	cout << "10次二线程+ikj平均所需时间" << (ave6 / 10) * 1000 << "ms" << endl;
	//====================二线程+转置=====================//
	double ave7 = 0;
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		thread t60 = thread(mult1, A, B, C, n / 2, 0, 0, 0, 0);
		thread t61 = thread(mult1, A, B, C, n / 2, 0, n / 2, n / 2, 0);
		t60.join(); t61.join();
		thread t62 = thread(mult1, A, B, C, n / 2, 0, 0, 0, n / 2);
		thread t63 = thread(mult1, A, B, C, n / 2, 0, n / 2, n / 2, n / 2);
		t62.join(); t63.join();
		thread t64 = thread(mult1, A, B, C, n / 2, n / 2, 0, 0, 0);
		thread t65 = thread(mult1, A, B, C, n / 2, n / 2, n / 2, n / 2, 0);
		t64.join(); t65.join();
		thread t66 = thread(mult1, A, B, C, n / 2, n / 2, 0, 0, n / 2);
		thread t67 = thread(mult1, A, B, C, n / 2, n / 2, n / 2, n / 2, n / 2);
		t66.join(); t67.join();
		end = clock();
		ave7 = ave7 + (double)(end - start) / CLOCKS_PER_SEC;
	}
	cout << "10次二线程+转置平均所需时间" << (ave7 / 10) * 1000 << "ms" << endl;
}
