## NYU HPC HW2.
### Mu-Hua Chien 
### mhc431@nyu.edu

Part 1, Fixed omp_bug's' 

omp_bug2: 

(i) "tid" is private variable, so it should be defined in the omp region.

(ii) To compute the total with imax = 1000000, we need double precision to save the information. Moreover, we need to apply reduction(+:total) in the omp for, where total is shared by default. 

omp_bug3: 

There is a barrier written in a function, where only the thread doing the section can see this barrier. The thread will stop to wait other threads arrive. However, none of the thread can see the barrier and it will be hang there.

The barrier in the section is deleted. 

omp_bug4:

Since the 2D array is initialized with pointer. It will cause conflict if all the processors use the pointer at the same time. 

To fix the bug, the initialization is relocated inbetween the parallel region, so that they can use the pointers priviately.

omp_bug5:

When initialize a, b must be locked, and when a is added to b, a must be locked. 

Hence, for the first section, lock b at first and initial a. Then unlock b and lock a to add the array. 

Treat section b in the opposite way. 

omp_bug6:

The omp must be defined either in the function "dotprod" or the "main" function. 

Therefore, I delete the omp in the "main" and apply the omp in the function "dotprod".


Part 2, Jacobi and Gauss-Seidel iteration with openmp

jacobi2D-omp:

Test with CIMS login node (AMD Opteron 23xx (Gen 3 Class Opteron))

For N = 1000, with 1000 iteration

Execute with 1 thread, residual = 9.505277e+02, Time elapsed is 20.354608 secs.

Execute with 4 thread, residual = 9.505277e+02, Time elapsed is 8.554077 secs.

Execute with 8 thread, residual = 9.505277e+02, Time elapsed is 9.271495 secs.

Note that the initial residual is 1.000000e+03.

For N = 2000, with 1000 iteration

Execute with 1 thread, residual = 1.950528e+03, Time elapsed is 75.341859 secs.

Execute with 4 thread, residual = 1.950528e+03, Time elapsed is 36.675188 secs.

Execute with 8 thread, residual = 1.950528e+03, Time elapsed is 37.324939 secs.

Note that the initial residual is 2.000000e+03.

gs2D-omp:

Test with CIMS login node (AMD Opteron 23xx (Gen 3 Class Opteron))

For N = 1000, with 1000 iteration

Execute with 1 thread, residual = 1.314705e+03, Time elapsed is 22.444311 secs.

Execute with 4 thread, residual = 1.314705e+03, Time elapsed is 8.817971 secs.

Execute with 8 thread, residual = 1.314705e+03, Time elapsed is 8.681636 secs.

Note that the initial residual is 1.412548e+03.

For N = 2000, with 1000 iteration

Execute with 1 thread, residual = 2.728919e+03, Time elapsed is 82.328207 secs.

Execute with 4 thread, residual = 2.728919e+03, Time elapsed is 34.153191 secs.

Execute with 8 thread, residual = 2.728919e+03, Time elapsed is 35.205348 secs.

Note that the initial residual is 2.826761e+03.


