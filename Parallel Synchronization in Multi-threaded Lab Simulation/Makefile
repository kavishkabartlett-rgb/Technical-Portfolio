# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pthread -Wall -g

# Source files
SRC = assigngroups.c  # Replace 'main.c' with your actual file name

# Output file
OUT = program  # Name of the output program

# Rule to compile and link the program
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# Clean command to remove the compiled program
clean:
	rm -f $(OUT)