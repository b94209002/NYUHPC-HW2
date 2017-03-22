/* multiply vector components, write into a vector,
 *  and compute the inner product  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
double residual(int m, double h2, double *x, double *rhs);

int main (int argc, char **argv)
{
  long  i;
  double *d, *l, *r, *rhs, *x; // idiagonal, right, left, right-hand-side, variable
  double prod,h,a,b,res,tmp; // grid size, initial, end, resdiual
  long m, maxiter, n = 0;// size, max iteration, iteration index
  double crit; // critera 

  if (argc != 3) {
    fprintf(stderr, "Function needs vector size and number of passes as input arguments!\n");
    abort();
  }

        a = 0;
        b = 1;
	m = atol(argv[1]);
	maxiter = atol(argv[2]);

	rhs = (double *) malloc(sizeof(double) * m);
	x = (double *) malloc(sizeof(double) * m);

	h = (b-a)/(m+1) ;  
        // initial matrix
  	for (i = 0; i < m; ++i) {
	rhs[i] = 1.0;
	x[i] = 0.0;
	}

   	timestamp_type time1, time2;
   	get_timestamp(&time1);
	
	res = residual(m,h*h,x,rhs);  // initial residual
        crit = 1.e-4*res;

	while (n < maxiter && res > crit ) {
	n++ ;
		x[0] =.5*(rhs[0]*h*h + x[1]);
		for (i = 1; i < m-1; ++i) {
			x[i] = .5*( rhs[i]*h*h + x[i-1] + x[i+1]);
		}
                x[m-1]=.5*( rhs[m-1]*h*h + x[m-2]);
		res = residual(m,h*h,x,rhs); 
                printf("niter = %li, residul = %10e \n", n, res);
	}

 	get_timestamp(&time2);
   	double elapsed = timestamp_diff_in_seconds(time1,time2);
 	printf("Numer of iteration %li, residual = %10e, Time elapsed is %f secs. \n", n, res, elapsed);

	free(rhs);
	free(x);

  return 0;
}

double residual (int m, double h2, double *x, double *rhs) {

int i;
double tmp,res;

tmp = rhs[1] - (2*x[0] - x[1])/h2;
res = tmp*tmp; // boundary condition on (0)
for  (i = 1; i < m-1; ++i) {
	tmp = rhs[i] + (x[i-1] - 2*x[i] + x[i+1])/h2;;
	res = res + tmp*tmp;
}
tmp = rhs[m-1] - (2*x[m-1] - x[m-2])/h2;;
res = res + tmp*tmp; // boundary condition on (1)

res = sqrt(res);

return res;
}
