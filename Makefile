# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Target executable name
TARGET = main

# Source directory
SRC_DIR = ./src

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove compiled files
clean:
	rm -rf $(OBJS) $(TARGET) $(SRC_DIR)/*.o output

run_small: clean all
	python3 ./src/experiments.py --run --small
	python3 ./src/experiments.py --plot

run: clean all
	python3 ./src/experiments.py --run
	python3 ./src/experiments.py --plot

plot:
	python3 ./src/experiments.py --plot

plot_precalc:
	python3 ./src/experiments.py --plot_precalc

# Phony targets
.PHONY: all clean run run_small plot plot_precalc