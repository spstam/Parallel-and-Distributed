CC := gcc
CFLAGS := -Wall -g -O3 -fopenmp
# Run with: make LEVEL=<level>
LEVEL ?= 1
CFLAGS += -DLEVEL=$(LEVEL)
LFLAGS := -lm
BINS := build/ask2_s
BINP := build/ask2_p
OBJS_S := build/ask2_s.o
OBJS_P := build/ask2_p_forA.o

.PHONY: all clean

all: build $(BINS) $(BINP)

build:
	-@mkdir -p build

build/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(BINS): $(OBJS_S)
	@$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(BINP): $(OBJS_P)
	@$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	@rm -rf build