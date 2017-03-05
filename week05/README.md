# How to Run

1. cd mypapi
2. set the mat1000.dat path in matMult.c
3. correct your Makefile PAPI path
4. set the 7th line in Makefile to TARGETS_NTHD = matMult
5. make -B
6. ./matMult