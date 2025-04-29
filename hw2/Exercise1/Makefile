CC := gcc
CFLAGS := -Wall -g -O1 -fopenmp -DJACOBI_MESSAGES
LFLAGS := -lm
BIN := build/ask1
OBJS := build/ask1.o

.PHONY: all clean

all: build $(BIN)

build:
	@mkdir -p build

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -rf build


