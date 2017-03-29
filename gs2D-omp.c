/* Solving linear system use Gauss-Seidel iteration in red-black ordering, 
with criterion 10^-4 to initial residual */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "util.h"
double residual(int m, double h2, double **x, double rhs);

int main (int argc, char **argv)
{
  long  i,j,it,jt;
  double d, l, r, rhs, tmp;// idiagonal, right, left, right-hand-side
  double  **x; //  variable
  int **rb; // red-black mask
  double prod,h,h2,a,b,res,rres; // grid size, initial, end, resdiual
  long m, m2,m1, maxiter, n = 0;// size, max iteration, iteration index
  double crit; // critera 

  if (argc != 3) {
    fprintf(stderr, "Function needs vector size and number of passes as input arguments!\n");
    abort();
  }

        a = 0;
        b = 1;
//	m = atol(argv[1]);
        m = 100;
	m2 = m+2 ;
	m1 = m+1 ;
// 	maxiter = atol(argv[2]);
        maxiter  = 100;
	x = malloc(m2 * sizeof(double *));
        rb = malloc(m2 * sizeof(int *));
	for(i = 0; i < m2; i++)
	{
		x[i] = malloc(m2 * sizeof(double));
		rb[i] = malloc(m2 * sizeof(int));		
	}

	h = (b-a)/((double)m+1.0) ;h2 = h*h; rhs = 1;  
        // initial matrix
  	for (j = 0; j < m2; ++j) {
		for (i = 0; i < m2; ++i) {
			x[j][i] = 0.0;
		}
	}
	// initial red and black mask
	jt = 1 ;
        for (j = 1; j < m1; ++j) {
                jt = -1*jt;it =1 ;
		for (i = 1; i < m1; ++i) {
			it = -1*it;
			if ( it * jt == 1) { 
                        rb[j][i] = 1;
			} else
			{
			rb[j][i] = 0;
			}
                }
        }

  	timestamp_type time1, time2;
  	get_timestamp(&time1);
	int myid = 0;
	#pragma omp parallel shared(x,rb,m,m1,m2,h,h2,rhs,crit,maxiter) private(n,i,j,tmp,rres)
	{
#ifdef _OPENMP
  	myid = omp_get_thread_num();
#endif
        res = 0.0; // boundary condition on (0)
	#pragma omp barrier
	#pragma omp for schedule(dynamic,10) reduction(+:res)
		for (j = 1; j < m1; ++j) {
        		for (i = 1; i < m1; ++i) {
                	        tmp = rhs + (x[j-1][i] + x[j][i-1] - 4*x[j][i] + x[j][i+1] + x[j+1][i])/h2;
        		        res = res + tmp*tmp;
        		}
		}
	rres = sqrt(res); crit = 1.e-4*rres; 
        printf("myid = %li, residul = %10e \n", myid, rres);
	while (n < maxiter && rres > crit ) {
	n++ ; 
	#pragma omp barrier
	res = 0.0;

        #pragma omp for schedule(dynamic,10)
		for (j = 1; j < m1; j++) {
			for (i = 1; i < m1; i++) {
				if (rb[j][i] == 0) {
				x[j][i] = .25*( rhs*h*h + x[j-1][i]+ x[j][i-1] + x[j][i+1]+ x[j+1][i]);
				}	
			}
		}

        #pragma omp for schedule(dynamic,10)
                for (j = 1; j < m1; j++) {
                        for (i = 1; i < m1; i++) {
				if (rb[j][i]== 1){
                                x[j][i] = .25*( rhs*h*h + x[j-1][i]+ x[j][i-1] + x[j][i+1]+ x[j+1][i]);
                        	}	
			}
                }

        #pragma omp for schedule(dynamic,10) reduction(+:res)
	       for (j = 1; j < m1; j++) {
                        for (i = 1; i < m1; i++) {
                                tmp = rhs + (x[j-1][i] + x[j][i-1] - 4*x[j][i] + x[j][i+1] + x[j+1][i])/h2;
                                res = res + tmp*tmp;
                        }
                }

	rres = sqrt(res);
		if (myid == 0 )
	 printf("myid = %i, niter = %li, residul = %10e \n",myid, n, sqrt(res));		
	}

	}

	get_timestamp(&time2);
  	double elapsed = timestamp_diff_in_seconds(time1,time2);
	
	// print final output
	printf("Numer of iteration %li, residual = %10e, Time elapsed is %f secs. \n", n, sqrt(res), elapsed);

        for(i = 0; i < m2; i++)
        {
                free(x[i]);
                free(rb[i]);
        }

	free(x);
	free(rb);

  return 0;
}

double residual (int m, double h2, double **x, double rhs) {

int i,j;
double tmp,res;

res = 0.0; // boundary condition on (0)

for (j = 1; j < m; ++j) {
	for (i = 1; i < m; ++i) {
			tmp = rhs + (x[j-1][i] + x[j][i-1] - 4*x[j][i] + x[j][i+1] + x[j+1][i])/h2;
		res = res + tmp*tmp;
	}
}

res = sqrt(res);

return res;
}
