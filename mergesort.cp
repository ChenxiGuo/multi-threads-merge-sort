#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <math.h>
#include<vector>

using namespace std;

#define N 2  /* # of thread */
#define FNAME "indata.txt"
int depth;
int a[4096];
int id[20];
int subs[20];

struct barrier_t
{
    int count;
    int reached;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
struct barrier_t b;

typedef struct Arr {
    int low;
    int high;
	int round;
} ArrayIndex;


void barrier_wait(struct barrier_t *barrier, int round){
    
/*if the thread is the last thread, he is respobsible to wake all other threads 	up; if the thread is not the last thread,  it shall just wait to be woken up*/
    //barrier->count = subs[round];
    pthread_mutex_lock(&barrier->mutex);
	subs[round]--;
	//printf("subs1: %d\n", subs[round]);
	if (subs[round] == -1)
		;
		//printf("round: %d \n", round);
    while(subs[round]>0){
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
  	    pthread_mutex_unlock(&barrier->mutex);
    }


	if(subs[round]==0){
			//printf("subs: %d\n", subs[round]);
            pthread_cond_broadcast(&barrier->cond);
		    pthread_mutex_unlock(&barrier->mutex);
    }
}


void merge(int low, int high)
{
		//printf("in merge\n");	
        int mid = (low+high)/2;
        int left = low;
        int right = mid+1;

        int b[high-low+1];
        int i, cur = 0;
		
        while(left <= mid && right <= high) {
                if (a[left] > a[right])
                        b[cur++] = a[right++];
                else
                        b[cur++] = a[left++];
        }
        while(left <= mid) b[cur++] = a[left++];
        while(right <= high) b[cur++] = a[right++];
        for (i = 0; i < (high-low+1) ; i++) a[low+i] = b[i];
}

void * mergesort(void *intArray)
{
		//printf("in mergesorting\n");
        ArrayIndex *pa = (ArrayIndex *)intArray;
        int mid = (pa->low + pa->high)/2;
		int round = pa->round;  // the round number
        ArrayIndex aIndex[N];
        pthread_t thread[N];
		
        aIndex[0].low = pa->low;
        aIndex[0].high = mid;
		aIndex[0].round = round +1;
        aIndex[1].low = mid+1;
        aIndex[1].high = pa->high;
		aIndex[1].round = round +1;

        if (pa->low >= pa->high){
			barrier_wait(&b,round);
			printf("the deepest barrier gone.\n");
			return 0;
		}

		int i;
        for(i = 0; i < N; i++) pthread_create(&thread[i], NULL, mergesort, &aIndex[i]);
		barrier_wait(&b,round+1);
		//printf("barrier gone.\n");
        merge(pa->low, pa->high);
		barrier_wait(&b,round);
        return 0;
}

int main()
{		
		//read from file
		
    	pthread_cond_init(&((&b)->cond), NULL);
    	pthread_mutex_init(&((&b)->mutex), NULL);
		FILE *fstream;
    	fstream = fopen(FNAME,"r");                /*打开文件*/
    	if(fstream == NULL)
    	{
		    printf("Error opening file indata.txt\n");
		    exit(1);
		}
		//printf("before loop\n");

		//load in vector a
		int len;
		for(len=0;fscanf(fstream,"%d",&a[len])!=EOF;len++){
			;
		}
		//printf("load in vector a\n");
		int NUM = len;//NUM is rounds number
    	fclose(fstream);

		//for(len=0;len<NUM;len++){
		//	printf("%d ",a[len]);
		//}

        ArrayIndex ai;
		ai.round = 0;
        ai.low = 0;
        ai.high = NUM-1;
		printf("length = %d.\n",ai.high);
		depth = log(ai.high+1)/log(2);
        pthread_t thread;
		
		int bi;
		subs[0]=1;
		//printf("before subs\n");
		for(bi=1;bi<=depth;bi++){
			
			subs[bi]=pow(2,bi) + pow(2, bi-1);
			//printf("\n%d\n",subs[bi]);
		}
		//printf("Finish loading numbers.\n");
        pthread_create(&thread, NULL, mergesort, &ai);
		//printf("Finish threading.\n");
        pthread_join(thread, NULL);
		//printf("Finish joining.\n");
		
        int i;	
        for (i = 0; i < ai.high+1; i++) 
			printf ("%d ", a[i]);
		pthread_cond_destroy(&(b.cond));
		pthread_mutex_destroy(&(b.mutex));

        cout << endl;

        return 0;
}


