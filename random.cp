#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctime>

using namespace std;

#define N 4096
#define LIMIT 4096
int main(void)
{
    int a[N];
	int i;
    srand((unsigned)time(NULL)); 

    for(int i=0;i<N;i++) { 

        a[i]=(int)rand()%LIMIT; 
    }
    FILE *fp;
    if((fp=fopen("indata.txt","w"))==NULL){

        printf("\nerror on open data.txt!");

        exit(1);
    }

    for(i=0;i<N;i++)

    fprintf(fp,"%d ",a[i]);

    fclose(fp);
    
    return 0;
}
