#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

/* the main function of threads */
static void * thread_main(TMatrix *m, TMatrix *n)
{
    // TODO
	thread_arg_t thread1;
	thread_arg_t->m = m;
	thread_arg_t->n = n;
	thread_arg_t->t = newMatrix(m->nrows, m->ncols);
	pthread_crate(&(thread1->id), NULL, thread1_f, NULL);
	//do thread 0 stuff
	for (unsigned int i = 0; i < (thread_arg_t->m->nrows)/(unsigned)2; i++)
		for(unsigned int j = 0; < (thread_arg_t->n->ncols)/(unisgned)2; i++)
			thread_arg_t->t->data[i][j] = m->data
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
	/*for (unsigned int i = 0; i < m->nrows; i++)
		for (unsigned int j = 0; j < m->ncols; j++)
			t->data[i][j] = m->data[i][j] + n->data[i][j];
    return t;*/
	//make two threads, store id, m and n in their thread struct
	//pass to main which divides matrix thread work top down?
	//pass to thread functions
	//return to thread main and wait for each other
	//return result here
	//kill john lennon
	TMatrix *p_arg[] = {m, n};
	t = thread_main(p_arg);
	return t;
}
