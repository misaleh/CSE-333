#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>


int aMatrix[100][100] ;
int bMatrix[100][100] ;
int cMatrix[100][100] ;
int row1, col1, row2, col2;

typedef struct  {
int mat2thread_i[100];
int mat2thread_j[100];
int mat2thread_op;
}mat2thread;


//calculate the value of element in the matrix
void* calcElement(void* thread_arg)
{	static int k  = -1; /*it will be incremeted*/
	mat2thread *temp = (mat2thread*)malloc(sizeof(mat2thread));
	temp = (mat2thread*)thread_arg;
	k++;  /*increment the index of the array before processing so it will not be cut by another process*/
	int ii, jj,kk,op,sum = 0;
	ii   = temp->mat2thread_i[k];
	jj = temp->mat2thread_j[k];
	op = temp->mat2thread_op;	
	for(kk = 0 ;kk < op ; kk++)
	{
	sum  += aMatrix[ii][kk]*bMatrix[kk][jj]; 
	}	
	cMatrix[ii][jj] = sum; 

	
	pthread_exit((void*)k);/*dummy*/
	free(temp);
}

void* CalcRow(void* thread_arg)
{
	static int k  = -1; /*it will be incremeted*/
	mat2thread *temp1 = (mat2thread *)malloc(sizeof(mat2thread));
	temp1 = (mat2thread*)thread_arg;
	k++;  /*increment the index of the array before processing so it will not be cut by another process*/
	int ii, jj,kk,mm,op,sum = 0;
	ii   = temp1->mat2thread_i[k];
	jj = temp1->mat2thread_j[0];
	op = temp1->mat2thread_op;	
	for(mm = 0 ; mm < jj ; mm++)
	{
		for(kk  = 0 ; kk < op ; kk++)
		{
			sum  += (aMatrix[ii][kk]*bMatrix[kk][jj]) ;
		}
		cMatrix[ii][jj] = sum;
		sum =  0;
							
	}
	pthread_exit((void*)k);/*dummy*/
}
//this is a version of matrix multiplication that does not use threads
void nonThreadedMatMult (int matrix1[][100],int r1,int c1,int matrix2[][100],int r2,int c2,int Output[][100]){

    struct timeval stop, start;
    gettimeofday(&start, NULL);

   /*******************Code goes here **************************/
	int i = r1; //row
 	int j = c1; //cols
	int k = 0;

	int element = 0;

	if(r2 != c1) printf("error in dimension\n");
	for(i = 0 ; i < r1 ; i++)
	{
			for(j = 0 ; j < c2 ; j++)
			{
				for(k  = 0 ; k < r2 ; k++)
				{
					element  += (matrix1[i][k] * matrix2[k][j]) ;
						//printf("element of i=%d j=%d k=%d   = %d \n",i,j,k,element);
				}
			Output[i][j] = element;
			element =  0;
							
			}
		
				
	}
	

    gettimeofday(&stop, NULL); //end checking time
    
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    
}

//this is a version of matrix multiplication that creates a thread to calculate the value of one element of the output matrix
void threadedMatMultPerElement (int matrix1[][100],int r1,int c1,int matrix2[][100],int r2,int c2,int Output[][100]){

    struct timeval stop, start;
    gettimeofday(&start, NULL);

      /*******************Code goes here **************************/
	pthread_t threads[r1*c2]; /*array of threads of number of the elements*/
    	int j,i;
	int k = 0;
	 mat2thread *thread_argp= (mat2thread *)malloc(sizeof(mat2thread));
	thread_argp->mat2thread_op = c1;
	if(r2 != c1) printf("error in dimension\n");
	for(i = 0 ; i < r1 ; i++)
	{
			for(j = 0 ; j < c2 ; j++)
			{
					thread_argp->mat2thread_i[k]=  i;
					thread_argp->mat2thread_j[k]=  j;
				
					//printf("i = %d , j = %d k= %d\n",i,j,k);
				
					if(pthread_create(&threads[k], NULL, calcElement, (void *)thread_argp))
					{

 						printf("Can not create a thread\n");
 						exit(1);
					}
					k++;
							
			}
		
				
	}
     for (i = 0; i < r1*c2; i++) /* wait for terminate a threads */
     {
 	int value; /* value returned by thread */
 	pthread_join(threads[i], (void **)&value);
     }
printf("element = %d\n",k);	

    gettimeofday(&stop, NULL);
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    
}

//this is a version of matrix multiplication that creates a thread to calculate the values of elements in one row of the output matrix
void threadedMatMultPerRow (int matrix1[][100],int r1,int c1,int matrix2[][100],int r2,int c2,int Output[][100]){


    struct timeval stop, start;
    gettimeofday(&start, NULL);

      /*******************Code goes here **************************/
	pthread_t threads[r1]; /*array of threads of number of the rows*/
    	int i;
	int k = 0;
	mat2thread *thread_argp= (mat2thread *)malloc(sizeof(mat2thread));
	thread_argp->mat2thread_op = c1; 
	thread_argp->mat2thread_j[0]=  c2; /*since it is not used in this function i will use first element only, number of columns*/
	if(r2 != c1) printf("error in dimension\n");
	for(i = 0 ; i < r1 ; i++) /*number of rows*/
		{
			thread_argp->mat2thread_i[i]=  i;
				
			if(pthread_create(&threads[i], NULL, CalcRow, (void *)thread_argp))
				{
 					printf("Can not create a thread\n");
 					exit(1);
				}
			
		}
	printf("row = %d\n",i);
     for (i = 0; i < r1; i++) /* wait for terminate a threads */
     {
 	int value; /* value returned by thread */
 	pthread_join(threads[i], (void **)&value);
     }
	
    gettimeofday(&stop, NULL);

    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    
}

/******** This function reads a file into a matrix ***********/
// n: will contain number of rows of the read matrix
// m: will contain number of columns of the read matrix
//fileName: name of file to r	ead
// arr: will contain the read matrix
// call the function like:
//int r,c;
//readFile(&r,&c,mat1File,aMatrix);

void readFile(int *n , int *m , char *fileName , int arr[][100] ){
    FILE *in= fopen(fileName, "r") ;
    int nn , mm ;
    char line [1000];
    fgets(line ,1000 , in) ;

    int error = sscanf (line , "row=%i col=%i" ,&nn ,&mm) ;

    if (error==0)
        return ;
    
    *n=nn ;
    *m=mm;
    int i , j ;
    for (i=0;i<nn;i++){
        for (j=0;j<mm;j++){
            int tmp ;
            fscanf(in , "%d", &tmp) ;
            arr[i][j] = tmp ;
        }
    }
    fclose(in);
}

/******* This function writes the result matrix 'cMatrix' to a file ****/
// rowsNum: number of rows in cMatrix
// colsNum: number of columns in cMatrix
// fileName: name of output file

void writeToFile(int rowsNum, int colsNum, char *fileName){
	FILE *out=fopen(fileName, "w") ;
	   int i,j=0;
	    for (i=0;i<rowsNum;i++){
		for (j=0;j<colsNum;j++){
		    fprintf (out ,"%d\t" ,cMatrix[i][j]);
		}
		fprintf(out ,"\n");
	    }
	    fclose (out);
}


int main(int argc , char *argv[])
{

      char * mat1File="a.txt" ;
      char * mat2File="b.txt" ;
      char * matOutFile1= "c_1.out" ;
      char * matOutFile2= "c_2.out" ;
      char * matOutFile3= "c_3.out" ;
	
   
	readFile(&row1,&col1,mat1File,aMatrix);
	readFile(&row2,&col2,mat2File,bMatrix);
        nonThreadedMatMult(aMatrix,row1,col1,bMatrix,row2,col2,cMatrix);
	writeToFile(row1,col2,matOutFile1);
        threadedMatMultPerElement(aMatrix,row1,col1,bMatrix,row2,col2,cMatrix);
	writeToFile(row1,col2,matOutFile2);
        threadedMatMultPerRow(aMatrix,row1,col1,bMatrix,row2,col2,cMatrix);
	writeToFile(row1,col2,matOutFile3);

}
