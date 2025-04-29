#ifndef _JACOBI_PAR_H_
#define _JACOBI_PAR_H_

typedef double num_t;

num_t jacobi_parallel(num_t *A, num_t *b, int N, int maxIter, num_t tol, num_t *x);
num_t jacobi_serial(num_t *A, num_t *b, int N, int maxIter, num_t tol, num_t *x);

#endif