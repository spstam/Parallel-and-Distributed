#Uncomment your choice of compiler
MPICC    =   mpicc
#CC    =   mpiicc
#CC    =   mpigcc
#CC    =   mpipgcc

CC	  =   gcc

#Add your choice of flags
FLAGS   = -Wall -I/usr/local/include -L/usr/local/lib -O2 -w -g
BIN = ask4_ser.bin ask4_par.bin

all: $(BIN)


clean:  
	/bin/rm -rf \
	*.bin \
	*.o

ask4_ser.bin:  ask4_ser.c
	$(CC) $(FLAGS) -o $@ $<

ask4_par.bin:  ask4_par.c
	$(MPICC) $(FLAGS) -o $@ $<
