#Uncomment your choice of compiler
MPICC    =   mpicc
#CC    =   mpiicc
#CC    =   mpigcc
#CC    =   mpipgcc

CC	  =   gcc

#Add your choice of flags
FLAGS   = -Wall -I/usr/local/include -L/usr/local/lib -O2 -w -g
BIN = ask3

all: $(BIN)


clean:  
	/bin/rm -rf \
	*.bin \
	*.o

ask3: ask3.c
	$(MPICC) $(FLAGS) -o $@ $<
