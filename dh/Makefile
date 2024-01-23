CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

# List all C source files in the current directory
SRCS = $(wildcard *.c)

# Name of the executable
TARGET = dh_client dh_server

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean rule to remove generated files
clean:
	rm -f $(TARGET)
