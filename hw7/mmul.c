#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

// Search TODO to find the locations where code needs to be completed

#define     NUM_THREADS     2

typedef struct {
    //unsigned int id;
	unsigned int start;
	unsigned int end;
    TMatrix *m, *n, *t;
} thread_arg_t;

void die(char *s){
	fprintf(stderr, "Error: %s\n", s);
	exit(EXIT_FAILURE);
}

static void * thread_main(void * p_arg)
{
    // TODO
	//cast arguments to argument struct?
	thread_arg_t *args = (thread_arg_t *)p_arg;
	
	for(unsigned int i = args->start; i < args->end; i++){
		for(unsigned int j = 0; j < args->n->ncols; j++){
			TElement sum = (TElement)0;
			for (unsigned int k = 0; k < args->m->ncols; k++)
				sum += args->m->data[i][k] * args->n->data[k][j];
			args->t->data[i][j] = sum;
		}
	}
    return NULL;
}

/* Return the sum of two matrices.
 *
 * If any pthread function fails, report error and exit. 
 * Return NULL if anything else is wrong.
 *
TMatrix * mulMatrix(TMatrix *m, TMatrix *n)
192 {
193     if (     m == NULL || n == NULL
194           || m->ncols != n->nrows   )
195         return NULL;
196 
197     TMatrix * t = newMatrix(m->nrows, n->ncols);
198     if (t == NULL)
199         return t;
#start here
200     for (unsigned int i = 0; i < m->nrows; i++)  {
201         for (unsigned int j = 0; j < n->ncols; j++) {
202             TElement sum = (TElement)0;
203             for (unsigned int k = 0; k < m->ncols; k++)
204                 sum += m->data[i][k] * n->data[k][j];
205             t->data[i][j] = sum;
206         }
207     }
208     return t;
209 }

 * Similar to mulMatrix, but with multi-threading.
 */
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->ncols != n->nrows )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, n->ncols);
    if (t == NULL)
        return t;

    // TODO
	//thread id array
	pthread_t threads[NUM_THREADS];
	//argument structs for function
	thread_arg_t args[NUM_THREADS];

	//fill argument array
	for (unsigned int i = 0; i < NUM_THREADS; i++){
		args[i] = (thread_arg_t){
			i * m->nrows / NUM_THREADS,
			(i + 1) * m->nrows / NUM_THREADS,
			m, n, t
		};
	}

	//create threads loop
	for (int i = 0; i < NUM_THREADS; i++){
		int rc = pthread_create(&threads[i], NULL, thread_main, &args[i]);
		if(rc != 0)
			die("thread creation failed");
	}
	
	//join threads before exit
	for (int i = 0; i < NUM_THREADS; i++){
		int rc = pthread_join(threads[i], NULL);
		if(rc != 0)
			die("thread join fail");
	}
		
    return t;
}
