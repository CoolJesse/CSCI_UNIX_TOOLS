/*
matrix multiplication:
   3x4        4x3                                        3x3
|a,b,c,d|   |m,n,o|   |(a*m)+(b*q)+(c*t)+(d*w), (a*n)+(b*r)+(c*u)+(d*x), (a*o)+(b*s)+(c*v)+(d*y)|
|e,f,g,h| x |q,r,s| = |(e*m)+(f*q)+(g*t)+(h*w), (e*n)+(f*r)+(g*u)+(h*x), (e*o)+(f*s)+(g*v)+(h*y)|
|i,j,k,l|   |t,u,v|   |(i*m)+(j*q)+(k*t)+(l*w), (i*n)+(j*r)+(k*u)+(l*x), (i*o)+(j*s)+(k*v)+(l*y)|
            |w,x,y|   

*/

#include <stdio.h>

int main(void)
{
	int array_A[3][4] = { {1,2,3,4},{5,6,7,8},{9,10,11,12} };
	int array_B[4][3] = { {2,3,5},{7,11,13},{17,19,23},{29,31,37} };

	int array_C[3][3];

	/*********************************************************************************/
	/** Print contents of matix ******************************************************/
	printf("matrix_A:\n");
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<4; j++)
		{
			printf("%d ", array_A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	printf("matrix_B:\n");
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			printf("%d ", array_B[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	/********************************************************************************/
	/********************************************************************************/

	int sum = 0; 
	/** Multiply array_A[3][4] * array_B[4][3] = array_C[3][3]**/
	/********************************************************************************/
	/** row index for first matrix **************************************************/
	for(int i=0; i<3; i++)
	{
		/** column index for second matrix **/		
		for(int j=0; j<3; j++)
		{
			/** column index for first matrix and row index for second matrix **/
			for(int k=0; k<4; k++)
			{
				sum += (array_A[i][k] * array_B[k][j]);
			}
		
			array_C[i][j] = sum;
			sum = 0;
		}
	}
	/********************************************************************************/
	/** print contents of array_C **/
	printf("Array_C:\n");
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			printf("%d ", array_C[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	return 0;
}
