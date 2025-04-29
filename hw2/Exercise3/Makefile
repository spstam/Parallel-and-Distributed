CC := gcc
CFLAGS := -Wall -g -O1 -fopenmp 
# If specified, define SIMPLE_JACOBI
ifdef NESTED_JACOBI
CFLAGS += -DNESTED_JACOBI
endif
ifdef JACOBI_MESSAGES
CFLAGS += -DJACOBI_MESSAGES
endif
ifdef SERIAL
CFLAGS += -DSERIAL
endif
LFLAGS := -lm
BIN := build/ask3
OBJS := build/jacobi_par.o build/ask3.o

.PHONY: all clean

all: build $(BIN)

build:
	@mkdir -p build

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

test: all
	./$(BIN) 100 20 .00005

clean:
	rm -rf build


