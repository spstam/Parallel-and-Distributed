##############################################################################
# FILE: Makefile.MPI.c
# DESCRIPTION:
#   Makefile for all MPI C Language example codes
# AUTHOR: Blaise Barney
# LAST REVISED:  05/25/17
###############################################################################

#Uncomment your choice of compiler
CC    =   mpicc
#CC    =   mpiicc
#CC    =   mpigcc
#CC    =   mpipgcc

#Add your choice of flags
FLAGS   = -I/usr/local/include -L/usr/local/lib -O2 -w
BIN = ask1.bin

all: $(BIN)


clean:  
	/bin/rm -rf \
	ask1.bin \
	*.o

ask1.bin:  ask1.c
	$(CC) $(FLAGS) -o $@ $<

