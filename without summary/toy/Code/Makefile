CC = gcc
CFLAGS = -Wall -g
TARGET = toy
SRC = toy.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)