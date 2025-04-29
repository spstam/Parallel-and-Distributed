CC := gcc
CFLAGS := -Wall -g -O0
LFLAGS := -lm
BIN1 := build/matrix_norm_1.bin
BININF := build/matrix_norm_inf.bin

OBJS1 := build/matrix_norm_1.o
OBJSINF := build/matrix_norm_inf.o

.PHONY: all norm1 norminf clean

clean:
	rm -rf build

all: norm1 norminf

runall: all plots

run_norm1: norm1
	python3 plot.py --norm=1 --n=32768 --e=20 --rerun

plot_norm1: norm1
	python3 plot.py --norm=1 --n=32768 --e=20

norm1: build $(BIN1)

run_norminf: norminf
	python3 plot.py --norm=inf --n=32768 --e=20 --rerun

plot_norminf: norminf
	python3 plot.py --norm=inf --n=32768 --e=20

norminf: build $(BININF)

plots: plot_norm1 plot_norminf

build:
	@mkdir -p build

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.bin: %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)



