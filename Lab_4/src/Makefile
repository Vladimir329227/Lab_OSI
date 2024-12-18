CC = gcc

CFLAGS = -Wall -Werror -Wextra -std=c11 -pthread

OUT_DIR = out

TARGETS = $(OUT_DIR)/main $(OUT_DIR)/allocator1 $(OUT_DIR)/allocator2

OBJS = main.o allocator1.o allocator2.o

all: $(TARGETS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT_DIR)/main: main.o | $(OUT_DIR)
	$(CC) -o $@ main.o

$(OUT_DIR)/allocator1: allocator1.o | $(OUT_DIR)
	$(CC) -shared -o $(OUT_DIR)/liballocator1.so -fPIC allocator1.c

$(OUT_DIR)/allocator2: allocator2.o | $(OUT_DIR)
	$(CC) -shared -o $(OUT_DIR)/liballocator2.so -fPIC allocator2.c

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) $(OBJS)

distclean: clean
	rm -f *.o

mrproper: distclean
	rm -rf $(OUT_DIR)
