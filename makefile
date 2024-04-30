# Compiler
CC = g++

# Compiler and linker flags
CFLAGS = -std=c++17 -Ddebug -Wall -O3
LDFLAGS = -lgtest_main -lgtest -lpthread

# Source files
SRC = $(wildcard src/*.cc)

# Target executable
TARGET = cyclic

$(TARGET):
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET) $(LDFLAGS)
