Fixed omp_bug's' 

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


