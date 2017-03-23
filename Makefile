CC=gcc
CFLAGS= -fopenmp -O3 -lm

all: omp_bug2 omp_bug3 omp_bug4 omp_bug5 omp_bug6 Jacobi2D-omp gs2D-omp 

omp_bug2: omp_bug2.c
	$(CC) -o omp_solved2  $^ $(CFLAGS)

omp_bug3: omp_bug3.c
	$(CC) -o omp_solved3  $^ $(CFLAGS)

omp_bug4: omp_bug4.c
	$(CC) -o omp_solved4  $^ $(CFLAGS)

omp_bug5: omp_bug5.c
	$(CC) -o omp_solved5  $^ $(CFLAGS)

omp_bug6: omp_bug6.c
	$(CC) -o omp_solved6  $^ $(CFLAGS)

Jacobi2D-omp: Jacobi2D-omp.c
	$(CC) -o jacobi2D-omp $^ $(CFLAGS)
gs2D-omp: gs2D-omp.c
	$(CC) -o gs2D-omp $^ $(CFLAGS)

clean:
	rm -f *_solved*


