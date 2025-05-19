CC = gcc
CFLAGS = -Wall -std=c99 -pthread
SRC = src/main.c src/util.c
OUT = chatApp
BUILD_DIR = BUILD

all: $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(OUT) $(SRC)

clean:
	rm -rf $(BUILD_DIR)
