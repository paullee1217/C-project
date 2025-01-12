# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Target executable
TARGET = simple_db

# Source files
SRCS = main.c create.c insert.c select.c delete.c update.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild the project
rebuild: clean all
