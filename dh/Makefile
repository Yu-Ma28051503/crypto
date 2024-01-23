CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

# List all C source files in the current directory
# SRCS = $(wildcard *.c)

# Name of the executable
TARGET = dh_client dh_server

# Default target
all: $(TARGET)

# Rule to build the executable
dh_client: dh_client.c
	$(CC) $(CFLAGS) -o dh_client dh_client.c $(LDFLAGS)

dh_server: dh_server.c
	$(CC) $(CFLAGS) -o dh_server dh_server.c $(LDFLAGS)

# Clean rule to remove generated files
clean:
	rm -f $(TARGET)
