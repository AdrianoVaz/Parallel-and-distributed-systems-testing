This project was made to evaluate evaluation tests completed by students. There are multiple evaluation methods described bellow, the objective of this assignement was to measure the speed of each method.

mpi:
```bash 
compile - mpicc -std=c99 -o mpi mpi.c
run - mpiexec -n 8 ./mpi
```

openmp:
```bash
compile - gcc -Wall -std=c99 -o openmp openmp.c -fopenmp
run - ./openmp
```

avaliacao:
```bash
compile - gcc -Wall -std=c99 -o aval aval.c
run - ./aval R<idquiz><idaluno>.txt
```

formativo:
```bash
compile - gcc -Wall -std=c99 -o formativo formativo.c
run - ./formativo R<idquiz><idaluno>.txt
```

sequencial:
```bash
compile - gcc -Wall -std=c99 -o sequencial sequencial.c
run - ./sequencial
```