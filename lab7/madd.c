#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define     NUM_THREADS     64

typedef struct {
    //unsigned int id;
	unsigned int start;
	unsigned int end;
    TMatrix *m, *n, *t;
} thread_arg_t;

/* the main function of threads */
static void * thread_main(void *arg)
{
    // TODO
	thread_arg_t *args = (thread_arg_t *)arg;

	for (unsigned int i = args->start; i < args->end; i++)
		for(unsigned int j = 0; j < args->m->ncols; j++)
			args->t->data[i][j] = args->m->data[i][j] + args->n->data[i][j];
    return NULL;
}

/* Return the sum of two matrices. The result is in a newly creaed matrix. 
 *
 * If a pthread function fails, report error and exit. 
 * Return NULL if something else is wrong.
 *
 * Similar to addMatrix, but this function uses 2 threads.
 */
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;

    // TODO
	pthread_t threads[NUM_THREADS];
	thread_arg_t args[NUM_THREADS];

	for (unsigned int i = 0; i < NUM_THREADS; i++){
		args[i] = (thread_arg_t){
			i * m->nrows / NUM_THREADS,
			(i + 1) * m->nrows / NUM_THREADS, 
			m, n, t
		};
	}

	for (int i = 0; i < NUM_THREADS; i++){
		int rc = pthread_create(&threads[i], NULL, thread_main, &args[i]);
		if (rc != 0){
			perror("thread creation fail 3:");
			exit(EXIT_FAILURE);
		}
	}
	for (int i = 0; i < NUM_THREADS; i++){
		int rc = pthread_join(threads[i], NULL);
		if (rc != 0){
			perror("thread join fail 3:");
			exit(EXIT_FAILURE);
		}
	}
	return t;
	
}
