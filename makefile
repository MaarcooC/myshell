# Compiler
CC = gcc
CFLAGS = -Wall -g

# Source files
# myshell.c + every .c file
SRCS = myshell.c $(wildcard lib/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = myshell

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)